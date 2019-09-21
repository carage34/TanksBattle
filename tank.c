#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/types.h>
#include <signal.h>
#include "time.h"
#include "menu.h"
#include "jeu.h"
#include "tank.h"
#include "liste.h"
#include "carrosserie.h"
int mode;
int normal = 10;
int blinde = 10;
int ultrablinde = 10;
int spawn = 0;
int timer=0;
void inittank(tank *tank) {
	int n, i=0, j=0, k, e, s, o, l, m;
	int a,b,c, d, fdo, fds, fde, fdn;
	fdn = open("tank_n.txt", O_RDONLY);
	fde = open("tank_e.txt", O_RDONLY);
	fdo = open("tank_o.txt", O_RDONLY);
	fds = open("tank_s.txt", O_RDONLY);
	if((fdo==-1)||(fds==-1)||(fde==-1)||(fdn==-1)) {
		perror("open");
		exit(-1);
	}
	tank->posx = 0;
	tank->posy = 0;
	tank->statut = 0;
	while(1) {
		n = read(fdn, &a, 1);
		s = read(fds, &b, 1);
		e = read(fde, &c, 1);
		o = read(fdo, &d, 1);
		if(n<1) break;
		if(n==EOF) break;
		tank->carrosserieo[i][j] = d;
		tank->carrosseriee[i][j] = c;
		tank->carrosserien[i][j] = a;
		tank->carrosseries[i][j] = b;
		j++;
		if(c=='\n') {
			i++;
			j=0;
		}
	}
	close(fds);
	close(fdn);
	close(fde);
	close(fdo);
}

void print_tank(tank tank, int i, int j, char tab[][LARGEUR_TANK], int** map, int joueur) {
	int color;
	//if(joueur) {
		//printf("\033[33m");
	//}

	switch(tab[i][j]) {
		case 'A':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df║", tank.posx+i, tank.posy+j);
		break;
		case 'B':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╔", tank.posx+i, tank.posy+j);
		break;
		case 'C':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df═", tank.posx+i, tank.posy+j);
		break;
		case 'D':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╗", tank.posx+i, tank.posy+j);
		break;
		case 'E':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df║", tank.posx+i, tank.posy+j);
		break;
		case '\n':
		//printf("\033[1;1f");
		//printf("%d:%d", i, j);
		//exit(-12);
		break;
		case 'F':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df ", tank.posx+i, tank.posy+j);
		break;
		case 'G':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╝", tank.posx+i, tank.posy+j);
		break;
		case 'H':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╚", tank.posx+i, tank.posy+j);
		break;
		case 'I':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╩", tank.posx+i, tank.posy+j);
		break;
		case 'J':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╠", tank.posx+i, tank.posy+j);
		break;
		case 'K':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╣", tank.posx+i, tank.posy+j);
		break;
		case 'L':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╦", tank.posx+i, tank.posy+j);
		break;
		case 'M':
		map[tank.posx+i][tank.posy+j] = tank.id;
		printf("\033[%d;%df╬", tank.posx+i, tank.posy+j);
		break;
		default:
		break;
	}

}

void move_tank(tank *tank, char tab[][LARGEUR_TANK], int** map, int joueur) {
	int i, j, color;
	if(mode==42) {
	color = rand()%7;
	switch(color) {
		case 0:
		printf("\033[31m");
		break;
		case 1:
		printf("\033[32m");
		break;
		case 2:
		printf("\033[33m");
		break;
		case 3:
		printf("\033[34m");
		break;
		case 4:
		printf("\033[35m");
		break;
		case 5:
		printf("\033[36m");
		break;
		case 6:
		printf("\033[37m");
		break;
	}
}
if(joueur) {
	printf("\033[33m");
}

	for(i=0;i<HAUTEUR_TANK;i++) {
		for(j=0;j<LARGEUR_TANK;j++) {
			print_tank(*tank, i, j, tab, map, joueur);
		}
	}
	printf("\033[0m");
}

void clear_tank(tank *tank, int direction, int** map) {
	int i, j;
	for(i=0;i<HAUTEUR_TANK;i++) {
		for (j=0;j<LARGEUR_TANK;j++) {
			map[tank->posx+i][tank->posy+j] = ' ';
			printf("\033[%d;%df ", tank->posx+i, tank->posy+j);
		}
		//printf("\033[1B");
		//printf("\033[%dD", LARGEUR_TANK+1);
	}
}

int spawn_tank(int** map, ListeTank *list, ListeCarro * listcarro) {
	int i, j,x ,y;
	if(timer>300000){
	if((normal==0)&&(blinde==0)&&(ultrablinde==0)) {
		return 0;
	}
	for(i=0;i<HAUTEUR_TANK;i++) {
		for(j=0;j<LARGEUR_TANK;j++) {
			if(spawn==1) {
				if(map[2+i][190+j]!=' ') {
					return 0;
				}
			} else {
				if(map[3+i][3+j]!=' ') {
					return 0;
				}
			}
		}
	}
	tank *bot = insertionTank(list, bot);
	if(spawn==1) {
		bot->posx = 2;
		bot->posy = 190;
		spawn = 0;
	} else {
		bot->posx = 3;
		bot->posy = 3;
		spawn = 1;
	}
	if(normal>0) {
		load_carro(bot, listcarro, NORMAL);
		normal--;
	} else if(blinde>0) {
		load_carro(bot, listcarro, BLINDE);
		blinde--;
	} else if(ultrablinde>0) {
		load_carro(bot, listcarro, ULTRA_BLINDE);
		ultrablinde--;
	} else {
		return 1;
	}
	for(i=0;i<HAUTEUR_TANK;i++) {
		for(j=0;j<LARGEUR_TANK;j++) {
			print_tank(*bot, i, j, bot->carrosserien, map, 0);
		}
	}

	timer=0;
	}
	timer++;
	return 0;
}

void move_tank_bot(ListeObus *listobus, ListeTank *listtank, int** map) {
	int move;
	tank *actuel = listtank->premier->suivant;

	while(actuel!=NULL) {
		if(actuel->timer>30000) {
			if((actuel->move==0)||(actuel->same==20)) {
				actuel->same = 0;
				move = rand()%6;
				if(move==0) {
					play(listobus, map, actuel, MOVE_UP, 1 ,NULL, 0);
					actuel->move = MOVE_UP;
				}
				if(move==1) {
					play(listobus, map, actuel, MOVE_DOWN, 1, NULL, 0);
					actuel->move = MOVE_DOWN;
				}
				if(move==2) {
					play(listobus, map, actuel, MOVE_LEFT, 1, NULL, 0);
					actuel->move = MOVE_LEFT;
				}
				if(move==3) {
					play(listobus, map, actuel, MOVE_RIGHT, 1, NULL, 0);
					actuel->move = MOVE_RIGHT;
				}
				if(move=4) {
					play(listobus, map, actuel, 32, 1, NULL, 0);
				}
			} else {
				actuel->same++;
				play(listobus, map, actuel, actuel->move, 1, NULL, 0);
			}
			actuel->timer = 0;
		}
		actuel->timer++;
		actuel = actuel->suivant;
	}
	if((mode==3)||(mode==4)) {
		if(listtank->premier->suivant==NULL) {
			if(listtank->premier->alive == 1) {
				print_menu("win.txt");
				system("killall play");
				system("stty echo");
				exit(0);
			}
		}
	}
}

void load_liste_carro(ListeCarro *listcarro, int mod) {
	mode = mod;
	if((mod==3)||(mod==42)) {
		normal=9;
		blinde=10;
		ultrablinde = 0;
	}
	if(mod==4) {
		normal = 0;
		ultrablinde = 10;
		blinde = 9;
	}

	if((mod==2)||(mod==5)) {
		normal = 0;
		ultrablinde = 0;
		blinde = 0;
	}
	carrosserie *n_h = insertionCarro(listcarro, n_h, NORMAL + HAUT);
	carrosserie *n_b = insertionCarro(listcarro, n_b, NORMAL + BAS);
	carrosserie *n_d = insertionCarro(listcarro, n_d, NORMAL + DROIT);
	carrosserie *n_g = insertionCarro(listcarro, n_d, NORMAL + GAUCHE);

	carrosserie *b_h = insertionCarro(listcarro, b_h, BLINDE + HAUT);
	carrosserie *b_b = insertionCarro(listcarro, b_b, BLINDE + BAS);
	carrosserie *b_d = insertionCarro(listcarro, b_d, BLINDE + DROIT);
	carrosserie *b_g = insertionCarro(listcarro, b_d, BLINDE + GAUCHE);

	carrosserie *ub_h = insertionCarro(listcarro, ub_h, ULTRA_BLINDE + HAUT);
	carrosserie *ub_b = insertionCarro(listcarro, ub_b, ULTRA_BLINDE + BAS);
	carrosserie *ub_d = insertionCarro(listcarro, ub_d, ULTRA_BLINDE + DROIT);
	carrosserie *ub_g = insertionCarro(listcarro, ub_d, ULTRA_BLINDE + GAUCHE);
	int fd, fd1,fd2, i, j, k;
	char c;
	fd=open("tank_normal.txt", O_RDONLY);
	for(k=1;k<=4;k++) {
		for(i=0;i<HAUTEUR_TANK;i++) {
			for(j=0;j<LARGEUR_TANK;j++) {
				read(fd, &c, 1);
				if(k==1) {
					n_h->tab[i][j] = c;
				}
				if(k==2) {
					n_d->tab[i][j] = c;
				}
				if(k==3) {
					n_b->tab[i][j] = c;
				}
				if(k==4) {
					n_g->tab[i][j] = c;
				}
			}
			read(fd, &c, 1);
		}
		read(fd, &c, 1);
		read(fd, &c, 1);
	}
	close(fd);
	fd1=open("tank_blinde.txt", O_RDONLY);
	for(k=1;k<=4;k++) {
		for(i=0;i<HAUTEUR_TANK;i++) {
			for(j=0;j<LARGEUR_TANK;j++) {
				read(fd1, &c, 1);
				if(k==1) {
					b_h->tab[i][j] = c;
				}
				if(k==2) {
					b_d->tab[i][j] = c;
				}
				if(k==3) {
					b_b->tab[i][j] = c;
				}
				if(k==4) {
					b_g->tab[i][j] = c;
				}
			}
			read(fd1, &c, 1);
		}
		read(fd1, &c, 1);
		read(fd1, &c, 1);
	}
	close(fd1);

	fd2=open("tank_ultra_blinde.txt", O_RDONLY);
	for(k=1;k<=4;k++) {
		for(i=0;i<HAUTEUR_TANK;i++) {
			for(j=0;j<LARGEUR_TANK;j++) {
				read(fd2, &c, 1);
				if(k==1) {
					ub_h->tab[i][j] = c;
				}
				if(k==2) {
					ub_d->tab[i][j] = c;
				}
				if(k==3) {
					ub_b->tab[i][j] = c;
				}
				if(k==4) {
					ub_g->tab[i][j] = c;
				}
			}
			read(fd1, &c, 1);
		}
		read(fd2, &c, 1);
		read(fd2, &c, 1);
	}
	close(fd2);
}

void load_carro(tank *tank, ListeCarro *listcarro, int id) {
	int i,j;
	carrosserie *haut;
	carrosserie *bas;
	carrosserie *gauche;
	carrosserie *droit;
	haut = getCarroById(listcarro, id + HAUT);
	bas = getCarroById(listcarro, id + BAS);
	gauche = getCarroById(listcarro, id + GAUCHE);
	droit = getCarroById(listcarro, id + DROIT);
	tank->statut = id;
	for(i=0;i<HAUTEUR_TANK;i++) {
		for(j=0;j<LARGEUR_TANK;j++) {
			tank->carrosserien[i][j] = haut->tab[i][j];
			tank->carrosseries[i][j] = bas->tab[i][j];
			tank->carrosseriee[i][j] = droit->tab[i][j];
			tank->carrosserieo[i][j] = gauche->tab[i][j];
		}
	}
}
