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



int main(int argc, char** argv) {
	int choice, offset;
	int x, pid;
	if(argc != 3) {
		printf("Usage: ./%s <server adress> <port>", argv[0]);
		exit(1);
	}
	system("setterm -cursor off");
	system("stty -echo");
	system("clear");
	system("play -q son/rayman.wav& -t alsa");

	menu(print_menu("menu_screen.txt"), pid, argv[1], argv[2]);




	//choice = battle_choice(posxeppe, posyeppe, max, min);
	//jeu(choice);
}
