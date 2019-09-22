#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include "menu.h"
#include "jeu.h"
int proc=0;
char* server_adress;
int port;
char key_pressed() {
	struct termios oldterm, newterm;
	int oldfd; char c, result = 0;
	tcgetattr (STDIN_FILENO, &oldterm);
	newterm = oldterm; newterm.c_lflag &= ~(ICANON | ECHO);
	tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
	oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
	c = getchar();
	tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
	fcntl (STDIN_FILENO, F_SETFL, oldfd);
	if (c != EOF) {ungetc(c, stdin); result = getchar();}
	return result;
}


int print_menu(char* filename) {
	system("clear");
	int fd,choice=1, curx, cury, i, col, offset, tmp=0;
	struct winsize ws;
	ioctl(0, TIOCGWINSZ , &ws);
	col = ws.ws_col;
	offset = (col/2) - (115/2);
	curx = 16;
	cury = 37+offset;
	char c, k;
	fd = open(filename, O_RDONLY);
	while(1) {
		if(tmp==0) {
			for(i=0;i<offset;i++) {
				printf(" ");
			}
		}
		k = read(fd, &c, 1);
		if(k<1) break;
		if(k==EOF) break;
		printf("%c", c);
		if(c=='\n') {
			for(i=0;i<offset;i++) {
				printf(" ");
			}
		}
		tmp++;
	}
	return offset;
}


void menu(int offset, int pid, char* serveradress, char* portt) {
	server_adress = serveradress;
	port = atoi(portt);
	if(proc==0) {
		proc = pid;
	}
	int curx=16, cury=37+offset, choice=1;
	char c;
	while(1) {
		printf("\033[%d;%df ", curx, cury);
		usleep(2000);
		printf("\033[%d;%df►", curx, cury);
		usleep(2000);
		c = key_pressed();
		switch(c) {
			case MOVE_DOWN:
			if(curx == 34) break;
			printf("\033[%d;%df ", curx, cury);
			curx+=6;
			choice++;
			break;
			case MOVE_UP:
			if(curx==16) break;
			printf("\033[%d;%df ", curx, cury);
			curx-=6;
			choice--;
			break;
			case '\n':
			if(choice == 1) {
				print_menu("menu_mode.txt");
				choose_battle_mode(offset);
			} else if(choice==3) {
				print_menu("menu_online.txt");
				choose_nickname(offset);
			} else {
				system("killall play");
				jeu(choice, 0, 0);
			}
		}
	}
}

int choose_nickname(int offset) {
	int player;
	char buff[255];
	bzero(buff,255);
	char response[255];
	int socket, recv_result, send_result, x=18;
	printf("\033[%d;%df", 17, 62+offset);
	system("setterm -cursor on");
	system("stty echo");
	fgets(buff, 255, stdin);
	system("setterm -cursor off");
	system("stty -echo");
	socket = connect_to_server();
	send_result = send(socket,buff,255,0);
  if (send_result == -1) {
    perror("send");
    //break;
  }
	recv_result = recv(socket,response,2,0);
	if (recv_result == -1) {
		perror("recv");
		exit(1);
	} else {
		if(strncmp(response, "j1", 2)==0) {
			player=1;
		} else {
			player=2;
		}
		bzero(response, 255);
	}
	while(1) {
		printf("\n");
		recv_result = recv(socket,response,255,0);
		if (recv_result == -1) {
			perror("recv");
			exit(1);
		}else {
			if(strncmp(response, "go", 2)==0) {
				system("killall play");
				jeu(5, player, socket);
			}
			printf("\033[%d;%df%s", x, 33+offset, response);
		}
		x++;
	}
}

int connect_to_server() {
struct sockaddr_in 	tcp_socket_address;
socklen_t 			sockaddr_in_size = sizeof(struct sockaddr_in);
int 				tcp_socket_descriptor, read_result, send_result, recv_result;
int 				yes = 1;

tcp_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

if (tcp_socket_descriptor == -1)
{
	perror("socket");
	exit(errno);
}

setsockopt(tcp_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

memset(&tcp_socket_address, 0, sockaddr_in_size);
tcp_socket_address.sin_family = AF_INET;
tcp_socket_address.sin_port = htons((short)strtol("5555",NULL,0));
tcp_socket_address.sin_addr.s_addr = inet_addr("127.0.0.1");

if (connect(tcp_socket_descriptor, (struct sockaddr*)&tcp_socket_address, sockaddr_in_size) != 0)
{
	perror("connect");
	exit(errno);
}
return tcp_socket_descriptor;
}

int choose_battle_mode(int offset) {
	int curx=22, cury=16+offset,choice=3, col;
	curx = 22;
	cury = 16+offset;
	char c, k;
	while(1) {
		printf("\033[%d;%df ", curx, cury);
		usleep(2000);
		printf("\033[%d;%df►", curx, cury);
		usleep(2000);
		c = key_pressed();
		switch(c) {
			case MOVE_LEFT:
			if(cury == 16+offset) break;
			printf("\033[%d;%df ", curx, cury);
			cury-=49;
			choice--;
			break;
			case MOVE_RIGHT:
			if(cury>=65+offset) break;
			printf("\033[%d;%df ", curx, cury);
			cury+=49;
			choice++;
			break;
			case '\n':
			system("killall play");
			jeu(choice, 0, 0);
			break;
		}
	}
}

int battle_choice(int posxt, int posyt, int max, int min) {
	char c;
	int i;
	int posxeppe = posxt;
	int posyeppe = posyt;
	while(1) {
		c = key_pressed();
		switch(c) {
			case 'A':
			if(posxeppe==min) {
				break;
			}

			printf("\033[%d;%df", posxeppe, posyeppe);
			printf(" ");
			posxeppe--;
			printf("\033[1D");
			printf("\033[1A");
			printf("⚔");
				break;
			case 'B' :
			if(posxeppe==max) {
				break;
			}

			printf("\033[%d;%df", posxeppe, posyeppe);
			printf(" ");
			posxeppe++;
			printf("\033[1D");
			printf("\033[1B");
			printf("⚔");
				break;
			case '\n' :
				if(posxeppe == min+1)
					return 1;
				if(posxeppe == min+2)
					return 2;
				break;
			default:
				break;
		}
		printf("\033[1;1f");

	}
}
