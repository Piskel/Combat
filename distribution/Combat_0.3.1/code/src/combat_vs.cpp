#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "fonctions.hpp"
#include "jeu.hpp"
#include "Heros.hpp"
#include "Joueur.hpp"
#include "Chargement.hpp"

int combat_vs(sf::RenderWindow *fenetre)
{
	Joueur joueur1(sf::Vector2f(0,0));
	Joueur joueur2(sf::Vector2f(0,0));

	//On définit ici les touches de chaque joueur
	//L'ordre dans lequel on entre les touches est important ! (voir Joueur.hpp)
	joueur1.input.addKey(sf::Keyboard::Left);
	joueur1.input.addKey(sf::Keyboard::Up);
	joueur1.input.addKey(sf::Keyboard::Right);
	joueur1.input.addKey(sf::Keyboard::Down);
	joueur1.input.addKey(sf::Keyboard::RControl);

	joueur1.setEnnemi(&joueur2);
	joueur2.setEnnemi(&joueur1);

	//Boucle qui contient l'écran de sélection, pour faire plusieurs parties.

	//écran de sélection des personnages.

	int i, choix_menu;

	bool ia = false; //sert à se rappeller si le joueur joue avec ou sans IA;

	do
	{
		choix_menu = selectionPersonnages(&joueur1, &joueur2, fenetre, &ia);

		joueur2.input.initKeys();

		if (!ia)
		{
			joueur2.input.addKey(sf::Keyboard::Q);
			joueur2.input.addKey(sf::Keyboard::Z);
			joueur2.input.addKey(sf::Keyboard::D);
			joueur2.input.addKey(sf::Keyboard::S);
			joueur2.input.addKey(sf::Keyboard::LShift);
		}
		else
		{
			//C'est saaaaaale mais j'ai pas encore d'autre moyen pour l'IA. Ça viendra avec la classe IA
			for (unsigned int i = 0; i < 5; i++)
				joueur2.input.addKey(sf::Keyboard::Unknown);
		}

		Chargement::chargerPerso(&joueur1);
		Chargement::chargerPerso(&joueur2);

		if ((choix_menu != UN_JOUEUR) && (choix_menu != DEUX_JOUEURS))
		{
			return choix_menu;
		}

		joueur1.setPosition(sf::Vector2f(LARGEUR_TERRAIN/2 + 100, 168));
		joueur2.setPosition(sf::Vector2f(LARGEUR_TERRAIN/2 - 100, 468));

		joueur1.setDirection(GAUCHE);
		joueur2.setDirection(DROITE);

		joueur1.ressuciter();
		joueur2.ressuciter();

		i = jeu(fenetre, choix_menu, &joueur1, &joueur2);
	} while (i == REJOUER);

	return i;
}