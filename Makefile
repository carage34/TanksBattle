but : PROJET

OFILES = jeu.o \
         liste.o \
         main.o \
         map.o \
         menu.o \
         obus.o \
         tank.o


CC = gcc

CFLAGS = -Wall

# CHAPITRE 3 : DEPENDANCES (REGLES IMPLICITES)

jeu.o : jeu.c tank.h map.h liste.h obus.h carrosserie.h
		gcc -c $(CFLAGS) jeu.c

liste.o : liste.c tank.h obus.h carrosserie.h
		gcc -c $(CFLAGS) liste.c

main.o : main.c menu.h jeu.h tank.h
		gcc -c $(CFLAGS) main.c

map.o : map.c menu.h jeu.h tank.h
		gcc -c $(CFLAGS) map.c

menu.o : menu.c jeu.h
		gcc -c $(CFLAGS) menu.c

obus.o : obus.c liste.h obus.h jeu.h
		gcc -c $(CFLAGS) obus.c

tank.o : tank.c liste.h obus.h jeu.h
		gcc -c $(CFLAGS) tank.c

PROJET : jeu.o liste.o main.o map.o obus.o tank.o
	$(CC) $(CFLAGS) -o PROJET $(OFILES) -lpthread

clean :
	-rm -f $(OFILES) PROJET

run : ./PROJET

.PHONY : but clean
