#ifndef JOUEUR_H
#define JOUEUR_H

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <Input.hpp>

//#include "fonctions.hpp"
#include "Heros.hpp"

#define LATENCE 60

//Il faut rentrer les touches dans cet ordre-ci dans input !
//enum {GAUCHE, HAUT, DROITE, BAS, COUP1, COUP2};
//(enum dans fonctions.h)

class Joueur : public Heros
{
public:
	Joueur(sf::Vector2f position = sf::Vector2f(400,300), int direction = GAUCHE);

	void setKeys(std::vector<sf::Keyboard::Key> keys);
	void addKey(std::vector<sf::Keyboard::Key> key);

	void update(float fps);

	in::Input input; //en public pour l'initialisation + déjà encapsulé
private:
	in::Event event;
	sf::Clock clockHaut; //contient le temps écoulé depuis le dernier appui de la touche "haut"
	sf::Clock clockCoup;
	sf::Clock clockCote;
	bool coupDeclanche;
	bool sautDeclanche;
	bool rouladeDeclanchee;
};

#endif