#ifndef JEU_H
#define JEU_H
#define MOVE_UP 'A'
#define MOVE_DOWN 'B'
#define MOVE_LEFT 'D'
#define MOVE_RIGHT 'C'
#define HAUTEUR_MAP 63
#define LARGEUR_MAP 206
#define MAX_TANK 0
void jeu(int mode, int player, int socket);
struct data {
  int x;
  int y;
  char c;
};
#endif
