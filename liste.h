#ifndef LISTE_H
#define LISTE_H
#include "tank.h"
#include "obus.h"
#include "carrosserie.h"

ListeTank *initialisationTank(tank *tank);
tank* insertionTank(ListeTank *liste, tank *tank);
void supprimerElementTank(ListeTank *liste, int valeur, int** map);
tank* getFirstTank(ListeTank *list);

ListeCarro *initialisationCarro(carrosserie *carrosserie);
carrosserie* insertionCarro(ListeCarro *liste, carrosserie *ins, int id);
carrosserie* getCarroById(ListeCarro *liste, int id);

ListeObus *initialisationObus(obus *obus);
obus* insertionObus(ListeObus *liste, obus *obus);
void supprimerElementObus(ListeObus *liste, int valeur);

void move_tank_bot(ListeObus *listobus, ListeTank *listtank, int** map);
void play(ListeObus *listobus, int** map, tank *tankjoueur, char c, int mode, tank *tankj2, int joueur);
int damage_tank(int iddamaged, ListeTank *listtank, int iddamager, int** map, ListeObus *listobus);
void test(ListeObus *liste);
#endif
