#ifndef FONCTIONS
#define FONCTIONS

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#define GRAVITE 1100 //900
#define INERTIE 800 //350

#define IA_NEWBIE 400
#define IA_FACILE 170
#define IA_MOYEN 115
#define IA_DIFFICILE 85
#define IA_HARDCORE 45

#define PV_MAX 600.0f

#define TEMPS_COMBO 500
#define VITESSE_ROULADE 300
#define TEMPS_ROULADE 0.17
#define VITESSE_SAUT 300
#define VITESSE_DOUBLE_SAUT 280

#define LARGEUR_TERRAIN 700.0 //la largeur du terrain, en px
#define HAUTEUR_TERRAIN 200.0
#define LARGEUR_FENETRE 300.0 //Ce que l'on voit à la fenêtre avec zoom = 1
#define LARGEUR_REELLE 900.0 //la largeur réelle de la fenêtre (px)
#define HAUTEUR_REELLE 600.0

//Un jour, penser à utiliser les enums proprement.
//... Ah non en fait non, vraiment trop le flemme.

enum{CORPS, TETE, JAMBES, OFFENSIF};
//Pour les types de hit-box.

//Sert aussi dans Joueur.cpp
enum{GAUCHE, HAUT, DROITE, BAS, COUP1, COUP2};

enum{COUP_NORMAL, COUP_NORMAL_BAS, COUP_NORMAL_HAUT, COUP_NORMAL_CHARGE, ROULADE, NUL};

//pour le choix de l'acceuil
enum{RIEN, UN_JOUEUR, DEUX_JOUEURS, QUITTER, RETOUR_JEU, RETOUR_MENU};

#include "HitBox.h"
#include "Coup.h"
#include "Heros.h"
#include "Joueur.h"

//Faire d'autres fichiers .h

//Jeu.cpp
int Jeu(sf::RenderWindow *fenetre, int type);
int Pause(sf::RenderWindow *fenetre, sf::View vue);
int choisirNiveauIA(sf::RenderWindow *fenetre);

//SelectionPersonnages.cpp
int selectionPersonnages(Heros *joueur1, Heros *joueur2);

//Fichiers.cpp
std::vector<std::string> chargerPersonnagesDisponibles();
void chargerPerso(Heros *perso, std::string nomPerso = "boxeur");
void chargerStats(Heros *perso, std::string nomPerso);

std::vector<std::vector<HitBox> > HitBoxDepuisUnFichier(std::string nomFichier = "ressources/stats/defaut.hitbox");
std::vector<HitBox> attribuerHitBox(std::vector<HitBox> hitbox, sf::Sprite sprite, int direction);
std::vector<std::vector<Coup> > coupsDepuisUnFichier(std::string nomFichier = "ressources/stats/defaut.coup");

//Divers
std::string intToString(int a);

#endif