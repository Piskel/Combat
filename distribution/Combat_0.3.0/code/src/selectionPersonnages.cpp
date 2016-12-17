#include <iostream>
#include <SFML/Graphics.hpp>

#include "fonctions.h"
#include "Heros.h"

using namespace sf;

//Ouvre l'écran de sélection des personnage. Retourne le nombre de joueur (c.à.d s'il a une IA ou pas)
int selectionPersonnages(Heros *joueur1, Heros *joueur2)
{
	std::string perso1, perso2;

	
	
	chargerPerso(joueur1, "boxeur");
	chargerPerso(joueur2, "boxeur");

	return 1;
}