/*#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "pthread.h"
#include "sys/syscall.h"

void* client_routine(void *arg){
  int nid, send_result, recv_result;
  nid = *(int*) arg;
  pthread_cond_wait(&wait, &lock);
  printf("client %d started", nid);
  send_result = send(nid,"Connexion au serveur réussi",MESSAGE_SIZE,MSG_EOR);
  if (send_result == -1)
  {
    perror("send");
    //break;
  }
}
*/
