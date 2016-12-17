#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "HitBox.h"
#include "fonctions.h"
#include "Heros.h"

//Ce fichier regroupes diverses fonctions utiles mais qui ne méritent pas un fichier entier rien que pour elles.

using namespace sf;

void chargerPerso(Heros *perso, std::string nomPerso)
{
	std::cout << "Chargement de " << nomPerso << "...\n";

	Texture texture;
	texture.loadFromFile("ressources/img/sprites/" + nomPerso + ".png");

	chargerStats(perso, nomPerso);
	perso->chargerHitBox(HitBoxDepuisUnFichier("ressources/stats/" + nomPerso + ".hitbox"));
	perso->chargerCoups(coupsDepuisUnFichier("ressources/stats/" + nomPerso + ".coups"));
	perso->setTexture(texture);
}

//Cette fonction va ajouter aux hitbox la position du sprite.

std::vector<HitBox> attribuerHitBox(std::vector<HitBox> hitbox, sf::Sprite sprite, int direction)
{
	unsigned int i;
	if (direction == DROITE)
	{	
		for (i = 0; i < hitbox.size(); i++)
		{
			hitbox[i].setPosition(hitbox[i].getPosition() + sprite.getPosition());
		}
	}
	else if (direction == GAUCHE)
	{
		//Pour retourner la hitbox verticalement quand le perso est retourné...
		for (i = 0; i < hitbox.size(); i++)
		{
			hitbox[i].setPosition(Vector2f(sprite.getPosition().x - hitbox[i].getPosition().x, sprite.getPosition().y + hitbox[i].getPosition().y));
			hitbox[i].setPosition(Vector2f(hitbox[i].getPosition().x - hitbox[i].hitBox().width, hitbox[i].getPosition().y));
		}
	}

	return hitbox;
}

std::string intToString(int a)
{
	std::stringstream ss;
	ss << a;
	return ss.str();
}