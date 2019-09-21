#include <stdlib.h>
#include <stdio.h>
#include "liste.h"
#include "tank.h"
#include "obus.h"
#include "carrosserie.h"

int damage_tank(int iddamaged, ListeTank *listtank, int iddamager, int** map, ListeObus *listobus) {
  if((iddamaged!=1)&&(iddamager!=1)) return 0;
  printf("\033[1;1f%d", iddamaged);
  tank* actuel;
  tank* damaged = NULL;
  tank* damager = NULL;
  actuel = listtank->premier;
  while(actuel!=NULL) {
    if(actuel->id == iddamaged) damaged = actuel;
    if(actuel->id == iddamager) damager = actuel;
    actuel = actuel->suivant;
  }
  damaged->touche += damager->statut;
  if(damaged->touche>=damaged->statut) {
    damaged->alive = 0;
    deleteAllObus(iddamaged, listobus);
    supprimerElementTank(listtank, damaged->id, map);
    return 1;
  }
  return 0;
}

ListeCarro *initialisationCarro(carrosserie *carrosserie) {
  ListeCarro *liste = malloc(sizeof(*liste));
  carrosserie = malloc(sizeof(*carrosserie));
  if(liste == NULL || carrosserie == NULL) {
    exit(EXIT_FAILURE);
  }
  carrosserie->id = HAUT + NORMAL;
  liste->id = HAUT + NORMAL;
  liste->premier = carrosserie;
  return liste;

}

carrosserie* insertionCarro(ListeCarro *liste, carrosserie *ins, int id) {
    /* Création du nouvel élément */
    ins = malloc(sizeof(*ins));
    if (liste == NULL || ins == NULL)
    {
        exit(2);
    }

    ins->id = id;
    /* Insertion de l'élément au début de la liste */
    carrosserie *actuel = NULL;
    actuel = liste->premier;
    while(actuel->suivant!=NULL) {
      actuel = actuel->suivant;
    }
    actuel->suivant = ins;
    ins->suivant = NULL;
    return ins;
}

carrosserie* getCarroById(ListeCarro *liste, int id) {
  carrosserie *actuel = NULL;
  actuel = liste->premier->suivant;
  while(actuel!=NULL) {
    if(actuel->id == id) {
      return actuel;
    }
    actuel=actuel->suivant;
  }
}
ListeTank *initialisationTank(tank *tank)
{
    ListeTank *liste = malloc(sizeof(*liste));
    tank = malloc(sizeof(*tank));
    if (liste == NULL || tank == NULL)
    {
        exit(EXIT_FAILURE);
    }
    tank->id = 1;
    tank->statut = NORMAL;
    tank->touche = 0;
    tank->joueur = 1;
    tank->suivant = NULL;
    tank->alive = 1;
    liste->premier = tank;
    liste->id = 1;
    return liste;
}

tank* getFirstTank(ListeTank *list) {
  return list->premier;
}

tank* insertionTank(ListeTank *liste, tank *ins)
{
    /* Création du nouvel élément */
    ins = malloc(sizeof(*ins));
    if (liste == NULL || ins == NULL)
    {
        exit(EXIT_FAILURE);
    }
    liste->id++;
    ins->id = liste->id;
    ins->direction = 'N';
    ins->statut = 1;
    ins->move=0;
    ins->same = 0;
    ins->touche = 0;
    ins->joueur = 0;
    ins->posx = 3;
    ins->posy = 3;
    ins->alive=1;
    tank *actuel = NULL;
    actuel = liste->premier;
    while(actuel->suivant!=NULL) {
      actuel = actuel->suivant;
    }
    actuel->suivant = ins;
    ins->suivant = NULL;
    return ins;
}

void supprimerElementTank(ListeTank *liste, int valeur, int** map) {
  if(liste == NULL)
  {
  return;
  }

  if(liste->premier->id == valeur) {
    tank* suppr = liste->premier;
    clear_tank(suppr, 0, map);
    liste->premier = liste->premier->suivant;
    return;
  }

  tank *suivant;
  tank *actuel = liste->premier;
  while(actuel != NULL) {
    if(actuel->suivant->id==valeur) {
      suivant = actuel->suivant->suivant;
      clear_tank(actuel->suivant, 0, map);
      free(actuel->suivant);
      actuel->suivant = suivant;
      return;
    }
    actuel = actuel->suivant;
  }
}

ListeObus *initialisationObus(obus *obus)
{
    ListeObus *liste = malloc(sizeof(*liste));
    obus = malloc(sizeof(*obus));
    if (liste == NULL || obus == NULL)
    {
      //printf("aaa");
        exit(EXIT_FAILURE);
    }
    obus->id = 1;
    obus->posx = 10;
    obus->posy = 10;
    obus->suivant = NULL;
    liste->premier = obus;
    liste->id = 1;
    return liste;
}

void test(ListeObus *liste) {
  if(liste->premier->direction=='N') {
    exit(-1);
  }
}

obus* insertionObus(ListeObus *liste, obus *ins)
{
    /* Création du nouvel élément */
    ins = malloc(sizeof(*ins));
    if (liste == NULL || ins == NULL)
    {
        exit(2);
    }
    liste->id++;

    ins->id = liste->id;
    ins->timer = 0;
    /* Insertion de l'élément au début de la liste */
    obus *actuel = NULL;
    actuel = liste->premier;
    while(actuel->suivant!=NULL) {
      actuel = actuel->suivant;
    }
    actuel->suivant = ins;
    ins->suivant = NULL;
    return ins;
}

void supprimerElementObus(ListeObus *liste, int valeur) {
  if(liste == NULL)
  {
  return;
  }

  obus *suivant;
  obus *actuel = liste->premier;

  while(actuel->suivant != NULL) {
    //printf("\033[20;20f%d", actuel->id);
    //printf("%d", actuel->id);
    if(actuel->suivant->id==valeur) {
      suivant = actuel->suivant->suivant;
      free(actuel->suivant);
      actuel->suivant = suivant;
      return;
    }
    actuel=actuel->suivant;
  }
}
