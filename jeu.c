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
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include "errno.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "menu.h"
#include "jeu.h"
#include "tank.h"
#include "map.h"
#include "liste.h"
#include "obus.h"
#include "carrosserie.h"
#include "ctype.h"
int col;
int row;
ListeObus *listobus;
ListeTank *listtank;
ListeCarro *listcarro;
int** map = NULL;
tank *tankjoueur;
tank *tankj2;
int currtank = 0;
int sock;

void* player_routine(void *arg){
	int nid;
	int x, y;
	char c;
	nid = *(int*) arg;
	int recv_result;
	while(1) {
		recv_result = recv(nid,&x,4,0);
		if (recv_result == -1) {
			perror("recv");
		}
		recv_result = recv(nid,&y,4,0);
		if (recv_result == -1) {
			perror("recv");
		}
		recv_result = recv(nid,&c,1,0);
		if (recv_result == -1) {
			perror("recv");
		}
		play(listobus, map, tankj2, c, 1, NULL, 0);
	}
}

void* send_data(void *arg) {
	struct data *data = arg;
	int send_result;
	send_result = send(sock,&(data->x),4,0);
	if (send_result == -1) {
		perror("send");
	}
	send_result = send(sock,&(data->y),4,0);
	if (send_result == -1) {
		perror("send");
	}
	send_result = send(sock,&(data->c),1,0);
	if (send_result == -1) {
		perror("send");
	}
	pthread_exit(NULL);
}

void jeu(int mode, int player, int socket) {
	system("clear");
	int i;
	map = malloc(HAUTEUR_MAP*sizeof(int**));
    for(i=0;i<HAUTEUR_MAP;i++) {
    	map[i] = malloc(LARGEUR_MAP*sizeof(int*));
    }
	print_map(map, HAUTEUR_MAP, LARGEUR_MAP);

	system("play -q son/skyrim.wav -t alsa&");
	sock = socket;
	pthread_t client;
	pthread_t send;
	char c,k, l;
	int twoplayer=0;
	int nbobus=0;
	int nbtank=0;
	
	int j, pidobus, pidtank, status;
	obus obusfactice;
	carrosserie factice;
	listtank = initialisationTank(tankjoueur);
	tankjoueur = getFirstTank(listtank);
	listobus = initialisationObus(&obusfactice);
	listcarro = initialisationCarro(&factice);
	load_liste_carro(listcarro, mode);
	load_carro(tankjoueur, listcarro, ULTRA_BLINDE);
	tankjoueur->statut = ULTRA_BLINDE;
	tank *nose = insertionTank(listtank, nose);
	nose->posx = 2;
	nose->posy = 190;
	


	if(mode==3) {
		load_carro(nose, listcarro, NORMAL);
	}

	if(mode==4) {
		load_carro(nose, listcarro, BLINDE);
	}
	if((mode==2)||mode==5) {
		tankj2 = insertionTank(listtank, tankj2);
		load_carro(tankj2, listcarro, ULTRA_BLINDE);
		load_carro(tankjoueur, listcarro, ULTRA_BLINDE);


		twoplayer=1;
		tankj2->statut = ULTRA_BLINDE;
		tankj2->posx = 3;
		tankj2->posy = 3;
		tankj2->alive = 1;
		tankjoueur->statut = ULTRA_BLINDE;
	}
	if(mode==5) {
		if(player==2) {
			tankj2->posx = 50;
			tankj2->posy = 100;
			tankjoueur->posx = 3;
			tankjoueur->posy = 3;
		} else {
			tankjoueur->posx=50;
			tankjoueur->posy=100;
		}
	} else {
		tankjoueur->posx=50;
		tankjoueur->posy=100;
	}
	tankjoueur->id = 1;
	tankjoueur->joueur = 1;
	if(mode==3) {
		tankjoueur->statut = ULTRA_BLINDE;
		load_carro(tankjoueur, listcarro, ULTRA_BLINDE);
	}
	if(mode==4) {
		tankjoueur->statut = BLINDE;
		load_carro(tankjoueur, listcarro, BLINDE);
	}


		printf("\033[1;1f");
		clear_tank(tankjoueur, MOVE_UP, map);
		for(i=0;i<HAUTEUR_TANK;i++) {
			for(j=0;j<LARGEUR_TANK;j++) {
				printf("\033[33m");
				print_tank(*tankjoueur, i, j, tankjoueur->carrosserien, map, 1);
				printf("\033[0m");
				if((mode==2)||(mode==5)) {
					print_tank(*tankj2, i, j, tankj2->carrosserien, map, 0);
				}
			}
		}
		if(mode==5) {
			pthread_create(&client,NULL,player_routine, (void*)&socket);
		}
    while(1) {
			int send_result;
			spawn_tank(map, listtank, listcarro);
    	c = key_pressed();
			play(listobus, map, tankjoueur, c, mode, tankj2, 1);
			move_obus(listobus, tankjoueur->statut, map, listtank);
			if((mode==5)) {
				if((c==MOVE_UP)||(c==MOVE_DOWN)||(c==MOVE_LEFT)||(c==MOVE_RIGHT)||(c==32)) {
					struct data data;
					data.x = tankjoueur->posx;
					data.y = tankjoueur->posy;
					data.c = c;
					pthread_create(&send, NULL, send_data, &data);
				}
			}
			if((mode!=2)&&(mode!=5)) {
				move_tank_bot(listobus, listtank, map);
			}
		}
	}

void play(ListeObus *listobus, int** map, tank *tankjoueur, char c, int mode, tank *tankj2, int joueur) {
	if(tankjoueur->alive==0) {
		print_menu("lose.txt");
		exit(0);
	}
	if((mode==2)||(mode==5)) {
		if(tankj2->alive==0) {
			print_menu("win.txt");
			exit(0);
		}
		if(tankjoueur->alive==0) {
			print_menu("lose.txt");
			exit(0);
		}
	}

	if((mode==2)&&(tankj2->alive==1)) {
		if(tolower(c)=='t') {
			if(tankj2->posx>1) {
				if(!(checkdirection(map, HAUTEUR_MAP, LARGEUR_MAP, *tankj2, MOVE_UP))) {
					tankj2->move = 0;
					return;
				}
				tankj2->direction = 'N';
				clear_tank(tankj2, MOVE_UP, map);
				tankj2->posx--;
				move_tank(tankj2, tankj2->carrosserien, map, joueur);
			}
			return;
		}
		if(tolower(c)=='g') {
			if(tankj2->posx+HAUTEUR_TANK<HAUTEUR_MAP) {
				if(!(checkdirection(map, HAUTEUR_MAP, LARGEUR_MAP, *tankj2, MOVE_DOWN))) {
					tankj2->move = 0;
					return;
				}
				tankj2->direction = 'S';
				clear_tank(tankj2, MOVE_DOWN, map);
				tankj2->posx++;

				move_tank(tankj2, tankj2->carrosseries, map, joueur);
			}
			return;
		}
		if(tolower(c)=='h') {
			if(tankj2->posy+LARGEUR_TANK<LARGEUR_MAP) {
				if(!(checkdirection(map, HAUTEUR_MAP, LARGEUR_MAP, *tankj2, MOVE_RIGHT))) {
					tankj2->move = 0;
					return;
				}
				tankj2->direction = 'E';
				clear_tank(tankj2, MOVE_RIGHT, map);
				tankj2->posy++;

				move_tank(tankj2, tankj2->carrosseriee, map, joueur);
			}
			return;
		}
		if(tolower(c)=='f') {
			if(tankj2->posy>1) {
				if(!(checkdirection(map, HAUTEUR_MAP, LARGEUR_MAP, *tankj2, MOVE_LEFT))) {
					tankj2->move = 0;
					return;
				}
				tankj2->direction = 'O';
			clear_tank(tankj2, MOVE_LEFT, map);
			tankj2->posy--;

			move_tank(tankj2, tankj2->carrosserieo, map, joueur);
		}
		return;
		}
		if(tolower(c)=='j') {
			launch(map, *tankj2, listobus, listtank);
		}
	}
	if(tankjoueur->alive==1) {
	switch(c) {
		case MOVE_UP:
		if(tankjoueur->posx>1) {
			if(!(checkdirection(map, HAUTEUR_MAP, LARGEUR_MAP, *tankjoueur, MOVE_UP))) {
				tankjoueur->move = 0;
				break;
			}
			tankjoueur->direction = 'N';
			clear_tank(tankjoueur, MOVE_UP, map);
			tankjoueur->posx--;
			move_tank(tankjoueur, tankjoueur->carrosserien, map, joueur);

		}
		break;
		case MOVE_DOWN:
		if(tankjoueur->posx+HAUTEUR_TANK<HAUTEUR_MAP) {
			if(!(checkdirection(map, HAUTEUR_MAP, LARGEUR_MAP, *tankjoueur, MOVE_DOWN))) {
				tankjoueur->move = 0;
				break;
			}
			tankjoueur->direction = 'S';
			clear_tank(tankjoueur, MOVE_DOWN, map);
			tankjoueur->posx++;

			move_tank(tankjoueur, tankjoueur->carrosseries, map, joueur);
		}
		break;
		case MOVE_RIGHT:
		if(tankjoueur->posy+LARGEUR_TANK<LARGEUR_MAP) {
			if(!(checkdirection(map, HAUTEUR_MAP, LARGEUR_MAP, *tankjoueur, MOVE_RIGHT))) {
				tankjoueur->move = 0;
				break;
			}
			tankjoueur->direction = 'E';
			clear_tank(tankjoueur, MOVE_RIGHT, map);
			tankjoueur->posy++;

			move_tank(tankjoueur, tankjoueur->carrosseriee, map, joueur);
		}
		break;
		case MOVE_LEFT:
		if(tankjoueur->posy>1) {
			if(!(checkdirection(map, HAUTEUR_MAP, LARGEUR_MAP, *tankjoueur, MOVE_LEFT))) {
				tankjoueur->move = 0;
				break;
			}
			tankjoueur->direction = 'O';
		clear_tank(tankjoueur, MOVE_LEFT, map);
		tankjoueur->posy--;

		move_tank(tankjoueur, tankjoueur->carrosserieo, map, joueur);
	}
	break;
	case 32:
	launch(map, *tankjoueur, listobus, listtank);
	if(joueur) system("play -q son/tir2.wav -t alsa&");
	break;
	default:
	break;
	}
}
}
