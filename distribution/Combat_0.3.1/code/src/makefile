OBJ:=Coup.o Fichiers.o fonctions.o Heros.o HitBox.o Jeu.o Joueur.o main.o SelectionPersonnages.o
EXENAME:=Combat
OPTIONS:=-std=c++14 -Wall -I ../include/
LIB:=libsfml-graphics.so libsfml-window.so libsfml-system.so libinput.so

all:movelibs exe rmlibs

movelibs:
	cp ../*.so ./

rmlibs:
	rm -f *.so

exe:$(EXENAME)

$(EXENAME):$(OBJ)
	g++ $(OBJ) $(LIB) -o $(EXENAME)

Coup.o:Coup.cpp HitBox.h fonctions.h
	g++ $(OPTIONS) -c Coup.cpp -o Coup.o

Fichiers.o:Fichiers.cpp fonctions.h Heros.h HitBox.h Coup.h
	g++ $(OPTIONS) -c Fichiers.cpp -o Fichiers.o

fonctions.o:fonctions.cpp HitBox.h fonctions.h Heros.h
	g++ $(OPTIONS) -c fonctions.cpp -o fonctions.o

Heros.o:Heros.cpp fonctions.h Heros.h HitBox.h Coup.h
	g++ $(OPTIONS) -c Heros.cpp -o Heros.o

HitBox.o:HitBox.cpp HitBox.h fonctions.h
	g++ $(OPTIONS) -c HitBox.cpp -o HitBox.o

Jeu.o:Jeu.cpp fonctions.h Heros.h Joueur.h
	g++ $(OPTIONS) -c Jeu.cpp -o Jeu.o

Joueur.o:Joueur.cpp Joueur.h Heros.h
	g++ $(OPTIONS) -c Joueur.cpp -o Joueur.o

main.o:main.cpp fonctions.h Heros.h
	g++ $(OPTIONS) -c main.cpp -o main.o

SelectionPersonnages.o:SelectionPersonnages.cpp fonctions.h Heros.h
	g++ $(OPTIONS) -c SelectionPersonnages.cpp -o SelectionPersonnages.o

clean:
	rm -f $(OBJ)
	rm -f $(LIB)

install:
	mv $(EXENAME) ../bin
