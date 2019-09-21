#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "menu.h"
#include "jeu.h"
#include "tank.h"
#include <curses.h>

int main(int argc, char const *argv[])
{
    int i=0, j=0,k, carac, fd;
  printf("\033[1;1f");
  int tab[HAUTEUR_MAP][LARGEUR_MAP];
  fd = open("map.txt", O_RDONLY);
  while(1) {
    k = read(fd, &carac, 1);
    if(k<1) break;
    //if(k==EOF) break;
    tab[i][j] = carac;
    //printf("%c", k);
    j++;
    if(carac=='\n') {
      i++;
      j=0;
    }
  }
  for(i=1;i<HAUTEUR_MAP;i++) {
    for(j=1;j<LARGEUR_MAP;j++) {
      if(tab[i][j]=='A') {
        //printf("\033[1;31m");
        //printf("📺");
        printf("A");
        //printf("\033[0m");
      } else if(tab[i][j] == 'B') {
        //printf("\033[1m\033[37m");
        //printf("📺");
        printf("B");
        //printf("\033[0m");
      } else if(tab[i][j]=='D'){
        //printf("\033[01;33m");
        //printf("🐬");
        printf("D");
        //printf("\033[0m");
      } else {
        printf("%c", tab[i][j]);
      }
    }
  }
    return 0;
}
