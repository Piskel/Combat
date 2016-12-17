#include "Joueur.hpp"
#include "Heros.hpp"

using std::cout;
using std::endl;

Joueur::Joueur(sf::Vector2f position, int direction) : Heros(position, direction)
{
	coupDeclanche = false;
	sautDeclanche = false;
	rouladeDeclanchee = false;
}

/*
* masquage de la méthode update + la gestion des inputs
*/

void Joueur::update(float fps)
{
	if (estVivant())
	{
		event = input.pollEvent();
		bool marche;
		static bool toucheDroiteRelachee;
		static bool toucheGaucheRelachee;

		marche = false;

		if (event.isKeyPressed())
		{
			if (event.getKey() == input.getKeys()[COUP1])
			{
				clockCoup.restart();
				coupDeclanche = false;
			}
			else if (event.getKey() == input.getKeys()[HAUT])
			{
				clockHaut.restart();
				sautDeclanche = false;
			}
			else if((event.getKey() == input.getKeys()[GAUCHE]) || (event.getKey() == input.getKeys()[DROITE]))
			{
				if ((clockRoulade.getElapsedTime().asMilliseconds() <= LATENCE*5) && (!rouladeDeclanchee))
				{
					if ((sf::Keyboard::isKeyPressed(input.getKeys()[GAUCHE])) && (toucheGaucheRelachee))
					{
						rouladeDeclanchee = true;
						roulade(GAUCHE);
					}
					else if ((sf::Keyboard::isKeyPressed(input.getKeys()[DROITE])) && (toucheDroiteRelachee))
					{
						rouladeDeclanchee = true;
						roulade(DROITE);
					}
				}
				else 
				{
					clockRoulade.restart();
					rouladeDeclanchee = false;
					toucheDroiteRelachee = false;
					toucheGaucheRelachee = false;
				}
			}
		}
		else if (event.isKeyReleased())
		{
			if (event.getKey() == input.getKeys()[GAUCHE])
			{
				toucheGaucheRelachee = true;
			}
			else if (event.getKey() == input.getKeys()[DROITE])
			{
				toucheDroiteRelachee = true;
			}
		}

		//La touche haut
		if ((clockHaut.getElapsedTime().asMilliseconds() > LATENCE) && (!sautDeclanche) && (sf::Keyboard::isKeyPressed(input.getKeys()[HAUT])))
		{
			if (clockCoup.getElapsedTime().asMilliseconds() > LATENCE)
			{
				sautDeclanche = true;
				sauter();
			}
			else
			{
				sautDeclanche = true; //Même si le saut n'a pas réellement été déclanché, on met cette variable à true pour que le jeu
									  //ne compte pas 2 fois la même input (si on le fait pas le personnage saute à chaque coup haut)
				frapper(COUP_NORMAL_HAUT);
			}
		}

		//les touches côtés
		if (clockRoulade.getElapsedTime().asMilliseconds() > LATENCE)
		{
			if (sf::Keyboard::isKeyPressed(input.getKeys()[GAUCHE]))
			{
				marcher(GAUCHE);
				marche = true;
			}
			else if (sf::Keyboard::isKeyPressed(input.getKeys()[DROITE]))
			{
				marcher(DROITE);
				marche = true;
			}
		}
		else
		{
			marche = false;
		}

		if ((sf::Keyboard::isKeyPressed(input.getKeys()[GAUCHE])) || (sf::Keyboard::isKeyPressed(input.getKeys()[DROITE])))
		{
			if ((clockCoup.getElapsedTime().asMilliseconds() < LATENCE) && (!marche))
			{
				rouladeDeclanchee = true;
				marche = false;
				frapper(COUP_NORMAL_COTE_1);
			}
		}

		//la touche frapper
		if ((clockCoup.getElapsedTime().asMilliseconds() > LATENCE) && (!coupDeclanche) && (sf::Keyboard::isKeyPressed(input.getKeys()[COUP1])))
		{
			coupDeclanche = true;
			if (clockHaut.getElapsedTime().asMilliseconds() < LATENCE)
			{
				frapper(COUP_NORMAL_HAUT);
			}
			else if(clockCote.getElapsedTime().asMilliseconds() < LATENCE)
			{
				frapper(COUP_NORMAL_COTE_1);
				rouladeDeclanchee = true;
				marche = false;
				/*
				if (sf::Keyboard::isKeyPressed(input.getKeys()[DROITE]))
					roulade(DROITE);
				if (sf::Keyboard::isKeyPressed(input.getKeys()[GAUCHE]))
					roulade(GAUCHE);*/
			}
			else
			{	
				if (estAccroupi())
					frapper(COUP_NORMAL_BAS);
				else
					frapper(COUP_NORMAL);
			}
		}

	}

	Heros::update(fps);
}