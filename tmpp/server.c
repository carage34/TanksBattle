#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "pthread.h"
#include "sys/syscall.h"
#define MAX_CLIENTS 2
#define MESSAGE_SIZE 255
int tab[2];
int clt=-1;
char name_j1[MESSAGE_SIZE];
char name_j2[MESSAGE_SIZE];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t p1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t p2 = PTHREAD_COND_INITIALIZER;
void* oneplayer_routine(void *arg){
  int nid, send_result, recv_result, i;
  int x, y;
  char c;
  char buff[255];
  nid = *(int*) arg;
  printf("client %d started\n", nid);
  recv_result = recv(nid,buff,255,0);
  strcpy(name_j1, buff);
	if (recv_result == -1) {
		perror("recv");
		exit(1);
	} else {
		printf("%s\n", buff);
	}
  send_result = send(nid,"j1",2,0);
  if (send_result == -1){
    perror("send");
  }
  send_result = send(nid,"Connexion au serveur réussi",MESSAGE_SIZE,0);
  if (send_result == -1){
    perror("send");
  }
  if(clt!=1) {

  send_result = send(nid,"En attente d'un deuxieme joueur",MESSAGE_SIZE,0);
  if (send_result == -1) {
    perror("send");
  }
}
  pthread_cond_wait(&p1, &lock);
  send_result = send(nid,"go",2,0);
  if (send_result == -1){
    perror("send");
  }
  while(1) {
    recv_result = recv(nid,&x,4,0);
    if (recv_result == -1) {
      perror("recv");
    }
    recv_result = recv(nid,&y,4,0);
    if (recv_result == -1) {
      perror("recv");
    }
    recv_result = recv(nid,&c,1,0);
    if (recv_result == -1) {
      perror("recv");
    }
    send_result = send(tab[1],&x,4,0);
    if (send_result == -1){
      perror("send");
    }
    send_result = send(tab[1],&y,4,0);
    if (send_result == -1){
      perror("send");
    }
    send_result = send(tab[1],&c,1,0);
    if (send_result == -1){
      perror("send");
    }
  }

}

void* twoplayer_routine(void *arg){
  int nid, send_result, recv_result, i;
  int x, y;
  char c;
  char buff[255];
  nid = *(int*) arg;
  printf("client %d started\n", nid);
  recv_result = recv(nid,buff,255,0);
  strcpy(name_j2, buff);
  if(recv_result == -1) {
    perror("recv");
  } else {
    printf("%s\n", buff);
  }
  send_result = send(nid,"j2",2,0);
  if (send_result == -1){
    perror("send");
  }
  send_result = send(nid,"Connexion au serveur réussi",MESSAGE_SIZE,0);
  if (send_result == -1){
    perror("send");
  }
  sprintf(buff, "Joueur en ligne : %s", name_j1);
  send_result = send(nid,buff,MESSAGE_SIZE,0);
  if (send_result == -1){
    perror("send");
  }
  sprintf(buff, "En attente de %s", name_j1);
  send_result = send(nid,buff,MESSAGE_SIZE,0);
  if (send_result == -1){
    perror("send");
  }
  pthread_cond_signal(&p1);
  send_result = send(nid,"go",MESSAGE_SIZE,0);
  if (send_result == -1){
    perror("send");
  }
  while(1) {
    recv_result = recv(nid,&x,4,0);
    if (recv_result == -1) {
      perror("recv");
    }
    printf("%d", x);
    recv_result = recv(nid,&y,4,0);
    if (recv_result == -1) {
      perror("recv");
    }
    recv_result = recv(nid,&c,1,0);
    if (recv_result == -1) {
      perror("recv");
    }
    send_result = send(tab[0],&x,4,0);
    if (send_result == -1){
      perror("send");
    }
    send_result = send(tab[0],&y,4,0);
    if (send_result == -1){
      perror("send");
    }
    send_result = send(tab[0],&c,1,0);
    if (send_result == -1){
      perror("send");
    }
  }
}

int 	main(int argc, char** argv){
	if (argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in 	tcp_socket_address;
	socklen_t 			sockaddr_in_size = sizeof(struct sockaddr_in);
	int 				tcp_socket_descriptor, nid;
	int 				bind_result, listen_result;
	int 				yes = 1;
	pthread_t 			oneplayer;
  pthread_t 			twoplayer;

	tcp_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (tcp_socket_descriptor == -1){
		perror("socket");
		exit(errno);
	}

	setsockopt(tcp_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	memset(&tcp_socket_address, 0, sockaddr_in_size);
	tcp_socket_address.sin_family = AF_INET;
	tcp_socket_address.sin_port = htons((short)strtol(argv[1],NULL,0));
	tcp_socket_address.sin_addr.s_addr = INADDR_ANY;

	bind_result = bind(tcp_socket_descriptor, (struct sockaddr*)&tcp_socket_address,sockaddr_in_size);

	if (bind_result == -1){
		perror("bind");
		exit(errno);
	}

	listen_result = listen(tcp_socket_descriptor, MAX_CLIENTS);

	if (listen_result == -1){
		perror("listen");
		exit(errno);
	}

	while (1){
    if(clt!=1) {

      nid = accept(tcp_socket_descriptor, (struct sockaddr*)&tcp_socket_address, &sockaddr_in_size);
      printf("client accept\n");
    clt++;
    tab[clt] = nid;
		if (nid == -1)
    {
			perror("accept");
			exit(errno);
		}
    if(clt==0) {
      pthread_create(&oneplayer,NULL,oneplayer_routine, (void*)&nid);
    } else {
      pthread_create(&twoplayer,NULL,twoplayer_routine, (void*)&nid);
    }
    if(clt==1) {
      //pthread_cond_signal(&wait);
      }
    }
  }
  return EXIT_SUCCESS;
}
