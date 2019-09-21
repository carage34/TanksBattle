#ifndef OBUS_H
#define OBUS_H
#include "liste.h"

typedef struct obus obus;
struct obus {
  int id;
  int posx;
  int posy;
  char direction;
  obus * suivant;
  int provenance;
  int timer;
  int statut;
};

typedef struct ListeObus ListeObus;
struct ListeObus
{
    obus *premier;
    int id;
};
void launch(int **map,tank tankjoueur, ListeObus *liste, ListeTank *listetank);
void move_obus(ListeObus *list, int statut, int** map, ListeTank *listtank);
int init_obus(obus *obus, tank tankjoueur, int** map, ListeObus *listeobus, ListeTank *listetank);
int check_block(obus *obus, int statut, int** map);
void deleteAllObus(int iddamaged, ListeObus *list);
#endif
