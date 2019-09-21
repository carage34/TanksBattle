#ifndef CARRO_H
#define CARRO_H
#include "jeu.h"
#include "tank.h"
typedef struct carrosserie carrosserie;
struct carrosserie {
  char tab[HAUTEUR_TANK][LARGEUR_TANK];
  carrosserie* suivant;
  int id;
};
typedef struct ListeCarro ListeCarro;
struct ListeCarro
{
    carrosserie *premier;
    int id;
};
#endif
