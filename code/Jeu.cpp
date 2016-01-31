#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "fonctions.h"
#include "Heros.h"
#include "HitBox.h"

using namespace sf;

int Jeu(RenderWindow *fenetre, int type)
{
	Texture background_text, background2_text;

	background_text.loadFromFile("ressources/img/background.png");
	background2_text.loadFromFile("ressources/img/background2.png");

	Sprite background;
	background.setTexture(background_text);
	background.setPosition(Vector2f(0, 0));

	Sprite background2;
	background2.setTexture(background2_text);
	background2.setPosition(Vector2f(0, 0));

	Heros joueur1(Vector2f(LARGEUR_TERRAIN/2 + 100, 168));
	Heros joueur2(Vector2f(LARGEUR_TERRAIN/2 - 100, 468), DROITE);

	joueur2.changerDirection(DROITE);

	chargerPerso(&joueur1, "boxeur");
	chargerPerso(&joueur2, "boxeur");

	RectangleShape vie1, vie2;

	//victoire et gagnant
	bool victoire = false; //vaut true si un des deux joueur a gagné.
	bool quitter = false;
	bool debug = false; //vaut true si en mode debug
	//int vainqueur; //vaut le numéro du vainqueur : 1 = J1, 2 = J2

	//Si on joue contre l'IA, on choisit son niveau
	int reactiviteIA;

	if (type == UN_JOUEUR)
	{
		reactiviteIA = choisirNiveauIA(fenetre);
		//un peu sale ça... (SALSA LAULE)
		if (reactiviteIA == QUITTER)
			return QUITTER;
		if (reactiviteIA == RETOUR_MENU)
			return RETOUR_MENU;
	}

	RectangleShape fond; //le fond du texte
	fond.setSize(Vector2f(500, 200));
	fond.setPosition(Vector2f(200, 200));
	fond.setFillColor(Color(50, 100, 120, 200));

	//texte et police
	Text victoire_texte, texte_vie1, texte_vie2;
	Font police;
	police.loadFromFile("ressources/freePixel.ttf");
	victoire_texte.setFont(police);
	victoire_texte.setPosition(Vector2f(320,280));
	texte_vie2.setFont(police);
	texte_vie2.setPosition(Vector2f(20,50));
	texte_vie1.setFont(police);
	texte_vie1.setPosition(Vector2f(LARGEUR_REELLE - 60,50));

	//la vue va en fait se trouver au milieu des deux combattants.
	View vue;
	float ratio = 1; //Pour le redimensionnement de la fenêtre. Hauteur de l'image vue = ratio * hauteur du terrain.

	View interface; //pour afficher les barres de vie
	interface.setSize(Vector2f(LARGEUR_REELLE, HAUTEUR_REELLE));
	interface.setCenter(Vector2f(LARGEUR_REELLE/2, HAUTEUR_REELLE/2));


	/*

	0-----------------------------> 300 (LARGEUR_TERRAIN)
	|
	|
	|
	|
	|
	|
	|
	|
	|
	v
	200
	(HAUTEUR_FENETRE)

	*/

	fenetre->setView(vue);

	vie2.setPosition(Vector2f(20,20));
	vie1.setPosition(Vector2f(LARGEUR_REELLE - 20 ,20));

	bool sautRelache, saut2Relache; //pour savoir si les joueurs ont relâchés leur touche de saut avant de faire le double saut.

	//variables IA :

	Vector2f distance;
	int direction;
	Clock clockFrapper;
	Clock teaBag;
	bool frapper = false;

	//Variables pour les mouvements de caméra
	Vector2f posCamera;
	float largeurImage;
	float zoom = 1;

	//pour le menu pause
	int choix;

	//Boucle principale
	while(!quitter)
	{
		Event event;

		while(fenetre->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				return QUITTER;
			}
			if (((event.type == Event::LostFocus) && (!victoire))||
			   ((event.type == Event::KeyReleased) && (event.key.code == Keyboard::Escape) && (!victoire)))
			{
				//on met le jeu en pause
				choix = Pause(fenetre, vue);
				if (choix != RETOUR_JEU)
					return choix;
			}
			if (event.type == Event::KeyReleased)
			{
				if ((event.key.code == Keyboard::RControl) || (event.key.code == Keyboard::RShift))
					joueur1.frapper();

				if ((event.key.code == Keyboard::LShift) && (type == DEUX_JOUEURS))
					joueur2.frapper();

				if (event.key.code == Keyboard::F1)
				{
					debug = !debug;
					joueur1.debug(debug);
					joueur2.debug(debug);
				}
				if (((event.key.code == Keyboard::Return) || (event.key.code == Keyboard::Escape)) && (victoire))
					return RETOUR_MENU;
			}
		}

		//PARTIE ÉVÉNEMENTS CLAVIERS

		//J1
		if ((Keyboard::isKeyPressed(Keyboard::K)) || (Keyboard::isKeyPressed(Keyboard::Left)))
		{
			joueur1.marcher(GAUCHE);
		}
		if ((Keyboard::isKeyPressed(Keyboard::M)) || (Keyboard::isKeyPressed(Keyboard::Right)))
		{
			joueur1.marcher(DROITE);
		}
		if ((Keyboard::isKeyPressed(Keyboard::L)) || (Keyboard::isKeyPressed(Keyboard::Down)))
		{
			joueur1.accroupir();
		}
		if ((Keyboard::isKeyPressed(Keyboard::O)) || (Keyboard::isKeyPressed(Keyboard::Up)))
		{
			if(sautRelache) joueur1.sauter();
			sautRelache = false;
		}
		else
			sautRelache = true;

		if (type == DEUX_JOUEURS)
		{
			//J2
			if (Keyboard::isKeyPressed(Keyboard::Q))
			{
				joueur2.marcher(GAUCHE);
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				joueur2.marcher(DROITE);
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				joueur2.accroupir();
			}
			if (Keyboard::isKeyPressed(Keyboard::Z))
			{
				if(saut2Relache) joueur2.sauter();
				saut2Relache = false;
			}
			else
				saut2Relache = true;
		}
		else if (type == UN_JOUEUR)
		{
			/****************************************************************************

			  ***      ******
			   *	   *    *
			   *       *    *
			   *       ******
			   *       *    *
			   *       *    *
			  ***  *   *    *  *

			*****************************************************************************/

			distance.x = joueur2.getPosition().x - joueur1.getPosition().x;
			distance.y = joueur2.getPosition().y - joueur1.getPosition().y;

			if(distance.x < 0)
				direction = DROITE;
			else
				direction = GAUCHE;

			joueur2.changerDirection(direction);

			if (joueur1.estVivant())
			{
				if (abs(distance.x) > 30 + abs(joueur2.getVitesse().x/15)) //On tient en compte la vitesse (inertie)
				{
					if ((joueur1.getPosition().x > 15) && (joueur1.getPosition().x < LARGEUR_TERRAIN- 15))
						joueur2.marcher(direction);
				}
				else if (!frapper)
				{
					//Si l'IA n'a pas encore frappé, elle va le faire
					if (abs(distance.y) < 25)
					{
						clockFrapper.restart();
						frapper = true;
					}
					if ((abs(distance.x) > 30) && (abs(distance.y < 25)))
					{
						joueur2.accroupir();
						clockFrapper.restart();
						frapper = true;
					}
				}
				if (abs(distance.x) > 30)
					joueur2.accroupir();

				if ((frapper) && (clockFrapper.getElapsedTime() > milliseconds(reactiviteIA)))
				{
					joueur2.frapper();
					frapper = false;
				}

				if ((distance.y > 25) && (distance.x < 80) && (joueur1.getDirection() != joueur2.getDirection()))
					//On saute si la distance en Y est assez grande, si la distance en X est assez petite,
					//Et si les deux persos ne vont pas dans le même sens
					joueur2.sauter();
			}
			else //Si l'adversaire est mort
			{
				//... tea bag !
				if ((abs(distance.x) > 10) && (joueur1.getPosition().x > 0) && (joueur1.getPosition().x < LARGEUR_TERRAIN))
				{
					joueur2.marcher(direction);	
				}
				else if (abs(joueur2.getVitesse().x) < 0.01)
				{
					//Humilation
					joueur2.sauter();
				}
			}
		}

		vie2.setSize(Vector2f(joueur2.getPV()/2., 30));
		vie1.setSize(Vector2f(-joueur1.getPV()/2., 30));

		vie1.setFillColor(Color(128 + 127*joueur1.getPV()/PV_MAX, 255*joueur1.getPV()/PV_MAX, 255*joueur1.getPV()/PV_MAX));
		vie2.setFillColor(Color(128 + 127*joueur2.getPV()/PV_MAX, 255*joueur2.getPV()/PV_MAX, 255*joueur2.getPV()/PV_MAX));

		texte_vie1.setString(intToString(joueur1.getPV()));
		texte_vie2.setString(intToString(joueur2.getPV()));

		joueur2.update(60., &joueur1);
		joueur1.update(60., &joueur2);

		//////GESTION DE LA VICTOIRE

		if (!victoire)
		{
			if(!joueur1.estVivant())
			{
				victoire = true;
				//vainqueur = 2;
				victoire_texte.setString("Victoire de J2 !");
			}
			else if (!joueur2.estVivant())
			{
				victoire = true;
				//vainqueur = 1;
				victoire_texte.setString("Victoire de J1 !");
			}
		}

		//////GESTION DE LA CAMÉRA

		//on réutilise la variable qui sert à l'IA ici
		distance.x = joueur2.getPosition().x - joueur1.getPosition().x;

		//Si les personnages sont trop éloignés, on réajuste le zoom.
		if (abs(distance.x) > 200)
			zoom = 300.0/abs(distance.x);
		else
			zoom = 3.0/2;

		//pour pas qu'on ne dézoomme pas trop
		if (zoom < (4.0/3) * LARGEUR_FENETRE/(LARGEUR_TERRAIN) )
			zoom = (4.0/3) * LARGEUR_FENETRE/(LARGEUR_TERRAIN);

		posCamera = Vector2f(((joueur1.getPosition().x + joueur2.getPosition().x)/2), (HAUTEUR_TERRAIN) - (HAUTEUR_TERRAIN)/(1.5*zoom) );
		//posCamera = Vector2f(((joueur1.getPosition().x + joueur2.getPosition().x)/2), HAUTEUR_FENETRE - (HAUTEUR_FENETRE)/(1.5*zoom) - abs((joueur1.getPosition().y - joueur2.getPosition().y)/2));


		//4/3 * zoom : pour que le zoom ne s'arrête pas au centre des persos mais qu'on les voie en entier.
		largeurImage = abs((LARGEUR_FENETRE*(4.0/3))/(zoom));

		//On vérifie que la caméra s'arrête si on est au bord du terrain
		if (posCamera.x < largeurImage/2)
		{
			posCamera.x = largeurImage/2;
		}
		else if (posCamera.x > LARGEUR_TERRAIN - largeurImage/2)
		{
			posCamera.x = LARGEUR_TERRAIN - largeurImage/2;
		}

		vue.setCenter(posCamera);
		vue.setSize(largeurImage, abs((HAUTEUR_TERRAIN*(4.0/3))/zoom) * ratio);

		fenetre->setView(vue);


		///Gestion du background 2

		background2.setPosition(vue.getCenter().x/2 - LARGEUR_FENETRE/1.5, 0);


		//////AFFICHAGE

		fenetre->clear(Color(161, 203, 214));
		fenetre->draw(background2);
		fenetre->draw(background);
		joueur2.afficher(fenetre);
		joueur1.afficher(fenetre);

		fenetre->setView(interface);

		fenetre->draw(vie1);
		fenetre->draw(vie2);
		fenetre->draw(texte_vie1);
		fenetre->draw(texte_vie2);

		if (victoire)
		{
			fenetre->draw(fond);
			fenetre->draw(victoire_texte);
		}

		fenetre->display();

		//initialisation de la marche, etc...
		joueur1.init();
		joueur2.init();
	}

	return QUITTER; //sert à compiler sans warning, et en cas de problème.
}

//La fonction de pause est DÉGUEULASSE.
//Penser à faire un truc potable parce que là, franchement, c'est de l'abus (une classe Bouton serait appréciable.)

int Pause(RenderWindow *fenetre, View vue)
{
	RectangleShape cadre;
	cadre.setFillColor(Color(50, 100, 120, 200));
	cadre.setSize(Vector2f(300,200));
	cadre.setPosition(Vector2f(300, 220));

	RectangleShape fond;
	fond.setFillColor(Color(0, 0, 0, 30));
	fond.setSize(Vector2f(900,600));
	fond.setPosition(Vector2f(0, 0));

	Vector2i posSouris;

	Text texte_pause, retour_jeu, retour_menu, quitter;

	Font police;
	police.loadFromFile("ressources/freePixel.ttf");

	texte_pause.setFont(police);
	texte_pause.setString("Pause");
	texte_pause.setPosition(Vector2f(410,240));

	retour_jeu.setFont(police);
	retour_jeu.setString("Retour au jeu");
	retour_jeu.setPosition(Vector2f(330,300));

	retour_menu.setFont(police);
	retour_menu.setString("Retour au menu");
	retour_menu.setPosition(Vector2f(330,330));

	quitter.setFont(police);
	quitter.setString("Quitter");
	quitter.setPosition(Vector2f(330,360));

	View vue_menu;
	vue_menu.setSize(900, 600);
	vue_menu.setCenter(450,300);

	fenetre->setView(vue_menu);

	fenetre->draw(fond);
	fenetre->draw(cadre);

	int choix = RIEN;

	while(choix == RIEN)
	{
		posSouris = Mouse::getPosition(*fenetre);
		Event event;
		while(fenetre->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				choix = QUITTER;
			}
			if ((event.type == Event::KeyReleased) && (event.key.code == Keyboard::Escape))
				choix = RETOUR_JEU;
			if (event.type == Event::MouseButtonReleased)
			{
				if (retour_jeu.getGlobalBounds().contains(Vector2f(posSouris)))
					choix = RETOUR_JEU;
				if (retour_menu.getGlobalBounds().contains(Vector2f(posSouris)))
					choix = RETOUR_MENU;
				if (quitter.getGlobalBounds().contains(Vector2f(posSouris)))
					choix = QUITTER;
			}
		}

		if(retour_jeu.getGlobalBounds().contains(Vector2f(posSouris)))
			retour_jeu.setColor(Color(150, 200, 230));
		else
			retour_jeu.setColor(Color(255, 255, 255));

		if(retour_menu.getGlobalBounds().contains(Vector2f(posSouris)))
			retour_menu.setColor(Color(150, 200, 230));
		else
			retour_menu.setColor(Color(255, 255, 255));

		if(quitter.getGlobalBounds().contains(Vector2f(posSouris)))
			quitter.setColor(Color(150, 200, 230));
		else
			quitter.setColor(Color(255, 255, 255));

		fenetre->draw(retour_jeu);
		fenetre->draw(retour_menu);
		fenetre->draw(quitter);
		fenetre->draw(texte_pause);

		fenetre->display();
	}
	if (choix != RETOUR_MENU)
		fenetre->setView(vue);

	return choix;
}

int choisirNiveauIA(RenderWindow *fenetre)
{
	RectangleShape fond;
	fond.setFillColor(Color(50, 100, 120));
	fond.setSize(Vector2f(900,600));
	fond.setPosition(Vector2f(0, 0));

	Vector2i posSouris;

	Text texte, facile, newbie, moyen, difficile, hardcore;

	Font police;
	police.loadFromFile("ressources/freePixel.ttf");

	texte.setFont(police);
	texte.setString("Choisissez le niveau de l'IA :");
	texte.setPosition(Vector2f(250,170));

	newbie.setFont(police);
	newbie.setString("Newbie");
	newbie.setPosition(Vector2f(300,230));

	facile.setFont(police);
	facile.setString("Facile");
	facile.setPosition(Vector2f(300,260));

	moyen.setFont(police);
	moyen.setString("Normal");
	moyen.setPosition(Vector2f(300,290));

	difficile.setFont(police);
	difficile.setString("Difficile");
	difficile.setPosition(Vector2f(300,320));

	hardcore.setFont(police);
	hardcore.setString("HARDCORE !!!");
	hardcore.setPosition(Vector2f(300,350));

	fenetre->draw(fond);

	int niveauIA = 0;

	while(niveauIA == 0)
	{
		posSouris = Mouse::getPosition(*fenetre);
		Event event;
		while(fenetre->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				return QUITTER;
			}
			if ((event.type == Event::KeyReleased) && (event.key.code == Keyboard::Escape))
				return RETOUR_MENU;
			if (event.type == Event::MouseButtonReleased)
			{
				if (newbie.getGlobalBounds().contains(Vector2f(posSouris)))
					return IA_NEWBIE;
				if (facile.getGlobalBounds().contains(Vector2f(posSouris)))
					return IA_FACILE;
				if (moyen.getGlobalBounds().contains(Vector2f(posSouris)))
					return IA_MOYEN;
				if (difficile.getGlobalBounds().contains(Vector2f(posSouris)))
					return IA_DIFFICILE;
				if (hardcore.getGlobalBounds().contains(Vector2f(posSouris)))
					return IA_HARDCORE;
			}
		}

		if(newbie.getGlobalBounds().contains(Vector2f(posSouris)))
			newbie.setColor(Color(150, 200, 230));
		else
			newbie.setColor(Color(255, 255, 255));

		if(facile.getGlobalBounds().contains(Vector2f(posSouris)))
			facile.setColor(Color(150, 200, 230));
		else
			facile.setColor(Color(255, 255, 255));

		if(moyen.getGlobalBounds().contains(Vector2f(posSouris)))
			moyen.setColor(Color(150, 200, 230));
		else
			moyen.setColor(Color(255, 255, 255));

		if(difficile.getGlobalBounds().contains(Vector2f(posSouris)))
			difficile.setColor(Color(150, 200, 230));
		else
			difficile.setColor(Color(255, 255, 255));

		if(hardcore.getGlobalBounds().contains(Vector2f(posSouris)))
			hardcore.setColor(Color(150, 200, 230));
		else
			hardcore.setColor(Color(255, 255, 255));

		fenetre->draw(newbie);
		fenetre->draw(facile);
		fenetre->draw(moyen);
		fenetre->draw(difficile);
		fenetre->draw(hardcore);
		fenetre->draw(texte);

		fenetre->display();
	}
	return -1;
}
