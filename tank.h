#ifndef TANK_H
#define TANK_H
#define LARGEUR_TANK 13
#define HAUTEUR_TANK 5
#define SPAWN_TANK_X 3
#define SPAWN_TANK_Y 3
#define HAUT 'H'
#define GAUCHE 'G'
#define DROIT 'D'
#define BAS 'B'
#define NORMAL 10
#define BLINDE 20
#define ULTRA_BLINDE 30
#include "carrosserie.h"
typedef struct tank tank;
struct tank {
	int id;
	int posx;
	int posy;
	int statut;
	char direction;
	char carrosserien[HAUTEUR_TANK][LARGEUR_TANK];
	char carrosseriee[HAUTEUR_TANK][LARGEUR_TANK];
  char carrosserieo[HAUTEUR_TANK][LARGEUR_TANK];
	char carrosseries[HAUTEUR_TANK][LARGEUR_TANK];
	tank * suivant;
	int move;
	int same;
	int touche;
	int joueur;
	int timer;
	int alive;
};

typedef struct ListeTank ListeTank;
struct ListeTank
{
    tank *premier;
    int id;
};

void clear_tank(tank *tank, int direction, int** map);
void move_tank(tank *tank, char tab[][LARGEUR_TANK], int** map, int joueur);
void inittank(tank *tank);
void print_tank(tank tank, int i, int j, char tab[][LARGEUR_TANK], int** map, int joueur);
int spawn_tank(int** map, ListeTank *list, ListeCarro * listcarro);
void load_carro(tank *tank, ListeCarro *listecarro, int id);
void load_liste_carro(ListeCarro *listcarro, int mod);
#endif
