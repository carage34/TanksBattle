#include "liste.h"
#include "obus.h"
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
void launch(int** map, tank tankjoueur, ListeObus *list, ListeTank *listetank) {
  obus *obus;
  obus = insertionObus(list, obus);
  if(!init_obus(obus,tankjoueur, map, list, listetank)) {
    supprimerElementObus(list, obus->id);
  } else {
    //printf("\033[20;30f%d", obus->id);
  }
}

void move_obus(ListeObus *list, int statut, int** map, ListeTank *listtank) {
  int ret;
  if(list == NULL) return;
  int oldx, oldy;
  obus *actuel = list->premier->suivant;

  while(actuel!=NULL) {
    if(actuel->timer>4000) {
      oldx = actuel->posx;
      oldy = actuel->posy;
      if(actuel->direction=='N') {
        actuel->posx--;
      }
      if(actuel->direction=='S') {
        actuel->posx++;
      }
      if(actuel->direction=='O') {
        actuel->posy--;
      }
      if(actuel->direction=='E') {
        actuel->posy++;
      }
      ret = check_block(actuel, actuel->statut, map);
      if(ret==0) {
        printf("\033[%d;%df ", oldx, oldy);
        printf("\033[%d;%df ", actuel->posx,actuel->posy);
        supprimerElementObus(list, actuel->id);
      } else if(ret==2)  {
        printf("\033[%d;%df ", oldx,oldy);
        supprimerElementObus(list, actuel->id);
      } else if(ret==1){
        printf("\033[%d;%df ", oldx, oldy);
        printf("\033[%d;%df0", actuel->posx, actuel->posy);
      } else if(ret==4){

      } else {
        printf("\033[%d;%df ", oldx, oldy);
        int iddamaged = map[actuel->posx][actuel->posy];
        damage_tank(iddamaged, listtank, actuel->provenance, map,list);
        supprimerElementObus(list, actuel->id);
      }
      actuel->timer=0;
    }
    actuel->timer++;
    actuel = actuel->suivant;
  }
}

int init_obus(obus *obus, tank tankjoueur, int** map, ListeObus *listobus, ListeTank *listtank) {
  int ret;
  obus->statut = tankjoueur.statut;
  obus->provenance = tankjoueur.id;
    if(tankjoueur.direction=='N') {
      obus->posx = tankjoueur.posx-1;
      obus->posy = tankjoueur.posy+6;
      obus->direction = 'N';
    }
    if(tankjoueur.direction=='S') {
      obus->posx = tankjoueur.posx+5;
      obus->posy = tankjoueur.posy+6;
      obus->direction = 'S';
    }
    if(tankjoueur.direction=='O') {
      obus->posx = tankjoueur.posx+2;
      obus->posy = tankjoueur.posy-1;
      obus->direction = 'O';
    }
    if(tankjoueur.direction=='E') {
      obus->posx = tankjoueur.posx+2;
      obus->posy = tankjoueur.posy+13;
      obus->direction = 'E';
    }

   ret = check_block(obus, tankjoueur.statut, map);
   if(ret==0) {
     printf("\033[%d;%df ", obus->posx,obus->posy);
     return 0;
   } else if(ret==2)  {
     return 0;
   } else if(ret==1){
     printf("\033[%d;%df0", obus->posx, obus->posy);
     return 1;
   } else {
     printf("\033[%d;%df ", obus->posx, obus->posy);
     int damaged = map[obus->posx][obus->posy];
     damage_tank(damaged, listtank, obus->provenance, map, listobus);
     return 0;
   }

}

int check_block(obus *obus, int statut, int** map) {
  switch(map[obus->posx][obus->posy]) {
    case 'A':
    map[obus->posx][obus->posy] = ' ';
    return 0;
    break;
    case 'B':
    if(statut>=BLINDE) {
      map[obus->posx][obus->posy] = ' ';
      return 0;
    }
    return 2;
    break;
    case 'C':
    map[obus->posx][obus->posy] = ' ';
    return 0;
    break;
    case ' ':
    return 1;
    break;
    case '#':
    return 2;
    break;
    case 'D':
    return 4;
    break;
    default:
    return 3;
    break;
  }
}

void deleteAllObus(int iddamaged, ListeObus *list) {
  obus *actuel = list->premier->suivant;
  while(actuel != NULL) {
    if(actuel->provenance == iddamaged) {
      printf("\033[%d;%df ", actuel->posx, actuel->posy);
      supprimerElementObus(list, actuel->id);
    }
    actuel = actuel->suivant;
  }
}
