#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "fonctions.hpp"
#include "Heros.hpp"
#include "Chargement.hpp"

#define TAILLE_MINIATURE 35.0
#define ESPACE_ENTRE_MINIATURES 2
#define SCALE Vector2f(TAILLE_MINIATURE/50.0, TAILLE_MINIATURE/50.0)
#define COLONNES_MAX 5

using namespace sf;

//Ouvre l'écran de sélection des personnage. Retourne le nombre de joueur (c.à.d s'il a une IA ou pas)
int selectionPersonnages(Heros *joueur1, Heros *joueur2, sf::RenderWindow *fenetre, bool *ia)
{
	std::string perso1, perso2;

	View vue;
	vue.setSize(300, 200);
	vue.setCenter(150, 100);

	fenetre->setView(vue);

	Texture fond_text, bouton_retour_text, bouton_retour_active_text, bouton_jouer_text, bouton_jouer_active_text, bouton_ia_on_text;
	Texture bouton_ia_on_active_text, bouton_ia_off_text, bouton_ia_off_active_text;
	fond_text.loadFromFile("ressources/img/menus/selection_personnages.png");
	bouton_retour_text.loadFromFile("ressources/img/menus/bouton_retour.png");
	bouton_retour_active_text.loadFromFile("ressources/img/menus/bouton_retour_active.png");
	bouton_jouer_text.loadFromFile("ressources/img/menus/bouton_jouer.png");
	bouton_jouer_active_text.loadFromFile("ressources/img/menus/bouton_jouer_active.png");
	bouton_ia_on_text.loadFromFile("ressources/img/menus/bouton_ia_on.png");
	bouton_ia_on_active_text.loadFromFile("ressources/img/menus/bouton_ia_on_active.png");
	bouton_ia_off_text.loadFromFile("ressources/img/menus/bouton_ia_off.png");
	bouton_ia_off_active_text.loadFromFile("ressources/img/menus/bouton_ia_off_active.png");

	Texture cadre_text;
	cadre_text.loadFromFile("ressources/img/menus/cadre.png");

	Vector2i posSouris;

	Sprite fond;
	fond.setTexture(fond_text);

	Sprite bouton_retour;
	bouton_retour.setTexture(bouton_retour_text);
	bouton_retour.setPosition(Vector2f(10,10));

	Sprite bouton_jouer;
	bouton_jouer.setPosition(Vector2f(212, 10));

	Sprite bouton_ia;
	bouton_ia.setPosition(Vector2f(112, 10));

	std::vector<Sprite> cadres;
	std::vector<Sprite> combattants;
	std::vector<Texture> textures;
	std::vector<Vector2f> positionPerso;
	std::vector<std::string> nomCombattants = Chargement::getNomCombattants();

	Sprite spriteJ1, spriteJ2;
	Sprite selectionJ1, selectionJ2;

	Texture selectionJ1_text, selectionJ2_text;
	selectionJ1_text.loadFromFile("ressources/img/menus/selection_j1.png");
	selectionJ2_text.loadFromFile("ressources/img/menus/selection_j2.png");

	selectionJ1.setTexture(selectionJ1_text);
	selectionJ1.setScale(Vector2f(SCALE));

	selectionJ2.setTexture(selectionJ2_text);
	selectionJ2.setScale(Vector2f(SCALE));

	spriteJ1.setPosition(Vector2f(232, 134));
	spriteJ2.setPosition(Vector2f(20, 134));

	spriteJ1.setOrigin(Vector2f(50, 0));
	spriteJ1.setScale(Vector2f(-1, 1)); //pour retourner le sprite du J1

	unsigned int positionJ1(0);
	unsigned int positionJ2(0);

	while ((nomCombattants.size() > cadres.size()) && (cadres.size() < COLONNES_MAX * 2))
	{
		cadres.push_back(Sprite());
		textures.push_back(Texture());
		combattants.push_back(Sprite());
		positionPerso.push_back(Vector2f());
	}

	unsigned int j(0), nombreColonnes;
	int posY;

	if (cadres.size() <= COLONNES_MAX)
	{
		nombreColonnes = cadres.size();
		posY = 0;
	}
	else
	{
		posY = -15;
		nombreColonnes = COLONNES_MAX;
	}

	for (unsigned int i = 0; i < cadres.size(); i++)
	{
		if (j >= COLONNES_MAX)
		{
			j -= COLONNES_MAX;
			posY += TAILLE_MINIATURE + ESPACE_ENTRE_MINIATURES;
			nombreColonnes = cadres.size() - COLONNES_MAX;
		}

		positionPerso[i] = Vector2f((j - 0.5*nombreColonnes) * (TAILLE_MINIATURE + ESPACE_ENTRE_MINIATURES) + 150, 60 + posY);

		cadres[i].setTexture(cadre_text);
		cadres[i].setScale(SCALE);
		cadres[i].setPosition(positionPerso[i]);

		textures[i].loadFromFile("ressources/img/sprites/" + nomCombattants[i] + "-miniature.png");
		combattants[i].setTexture(textures[i]);
		combattants[i].setPosition(positionPerso[i]);
		combattants[i].setScale(SCALE);

		j++;
	}

	//positionJ1 : la position du J1 dans le tableau de combattants
	//positionPerso : le tableau qui contient les positions des vignettes
	//positionPerso[positionJ1] : la position de la sélection de J1

	for (unsigned int i = 0; i < nomCombattants.size(); i++)
	{
		if (joueur1->nom == nomCombattants[i])
			positionJ1 = i;

		if (joueur2->nom == nomCombattants[i])
			positionJ2 = i;
	}

	int choix = RIEN;

	while (choix == RIEN)
	{
		posSouris = Mouse::getPosition(*fenetre);
		Event event;
		while(fenetre->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				choix = QUITTER;
			}
			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == Keyboard::Escape)
					choix = RETOUR_MENU;
				else if (event.key.code == Keyboard::Return)
				{
					if (*ia)
						choix = UN_JOUEUR;
					else
						choix = DEUX_JOUEURS;
				}
				else if (event.key.code == Keyboard::Right)
				{
					if ((positionJ1 < cadres.size() - 1) && (positionJ1 != COLONNES_MAX - 1)) //pour ne pas descendre d'un étage qd tout à droite
						positionJ1++;
				}
				else if (event.key.code == Keyboard::Left)
				{
					if ((positionJ1 > 0) && (positionJ1 != COLONNES_MAX))
						positionJ1--;
				}
				else if (event.key.code == Keyboard::Down)
				{
					if ((positionJ1 < COLONNES_MAX) && (cadres.size() > COLONNES_MAX))
						positionJ1 += COLONNES_MAX;

					while (positionJ1 >= cadres.size())
						positionJ1 --;
				}
				else if (event.key.code == Keyboard::Up)
				{
					if (positionJ1 >= COLONNES_MAX)
						positionJ1 -= COLONNES_MAX;
				}

				//J2

				else if (event.key.code == Keyboard::D)
				{
					if ((positionJ2 < cadres.size() - 1) && (positionJ2 != COLONNES_MAX - 1)) //pour ne pas descendre d'un étage qd tout à droite
						positionJ2++;
				}
				else if (event.key.code == Keyboard::Q)
				{
					if ((positionJ2 > 0) && (positionJ2 != COLONNES_MAX))
						positionJ2--;
				}
				else if (event.key.code == Keyboard::S)
				{
					if ((positionJ2 < COLONNES_MAX) && (cadres.size() > COLONNES_MAX))
						positionJ2 += COLONNES_MAX;

					while (positionJ2 >= cadres.size())
						positionJ2 --;
				}
				else if (event.key.code == Keyboard::Z)
				{
					if (positionJ2 >= COLONNES_MAX)
						positionJ2 -= COLONNES_MAX;
				}
			}
			if (event.type == Event::MouseButtonReleased)
			{
				if (bouton_retour.getGlobalBounds().contains(Vector2f(posSouris / 3)))
				{
					choix = RETOUR_MENU;
				}

				if (bouton_jouer.getGlobalBounds().contains(Vector2f(posSouris / 3)))
				{
					if (*ia)
						choix = UN_JOUEUR;
					else
						choix = DEUX_JOUEURS;
				}

				if (bouton_ia.getGlobalBounds().contains(Vector2f(posSouris / 3)))
				{
					if (*ia)
						*ia = false;
					else
						*ia = true;
				}
			}
		}

		//surbrillance quand on survole un bouton
		if (bouton_ia.getGlobalBounds().contains(Vector2f(posSouris / 3)))
		{
			if (*ia)
				bouton_ia.setTexture(bouton_ia_on_active_text);
			else
				bouton_ia.setTexture(bouton_ia_off_active_text);
		}
		else
		{
			if (*ia)
				bouton_ia.setTexture(bouton_ia_on_text);
			else
				bouton_ia.setTexture(bouton_ia_off_text);
		}

		if ((bouton_retour.getGlobalBounds().contains(Vector2f(posSouris / 3)))  || (Keyboard::isKeyPressed(Keyboard::Escape)))
			bouton_retour.setTexture(bouton_retour_active_text);
		else
			bouton_retour.setTexture(bouton_retour_text);

		if ((bouton_jouer.getGlobalBounds().contains(Vector2f(posSouris / 3))) || (Keyboard::isKeyPressed(Keyboard::Return)))
			bouton_jouer.setTexture(bouton_jouer_active_text);
		else
			bouton_jouer.setTexture(bouton_jouer_text);

		selectionJ1.setPosition(positionPerso[positionJ1]);
		selectionJ2.setPosition(positionPerso[positionJ2]);

		spriteJ1.setTexture(textures[positionJ1]);
		joueur1->nom = nomCombattants[positionJ1];

		spriteJ2.setTexture(textures[positionJ2]);
		joueur2->nom = nomCombattants[positionJ2];

		fenetre->draw(fond);
		fenetre->draw(bouton_jouer);
		fenetre->draw(bouton_retour);
		fenetre->draw(bouton_ia);
		fenetre->draw(spriteJ1);
		fenetre->draw(spriteJ2);

		for (unsigned int i = 0; i < cadres.size(); i++)
		{
			fenetre->draw(combattants[i]);
			fenetre->draw(cadres[i]);
		}

		fenetre->draw(selectionJ1);
		fenetre->draw(selectionJ2);

		fenetre->display();
	}

	return choix;
}