#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "fonctions.h"
#include "Heros.h"

using namespace sf;

int main()
{
	std::cout << "Lancement du programme...\n";

	RenderWindow fenetre(VideoMode(900,600,32), "~ Combat", Style::Close | Style::Titlebar);
	fenetre.setVerticalSyncEnabled(true);

	Texture unJoueur, unJoueurSelect, deuxJoueurs, deuxJoueursSelect;
	unJoueur.loadFromFile("ressources/img/1-joueur.png");
	unJoueurSelect.loadFromFile("ressources/img/1-joueur-select.png");
	deuxJoueurs.loadFromFile("ressources/img/2-joueurs.png");
	deuxJoueursSelect.loadFromFile("ressources/img/2-joueurs-select.png");

	Sprite SpriteUnJoueur, SpriteDeuxJoueurs;
	SpriteUnJoueur.setPosition(Vector2f(100,100));
	SpriteDeuxJoueurs.setPosition(Vector2f(100,300));

	//J'enlève la musique parce que j'ai pas de truc libre de droit convainquant sous la main.

	/*Music musique_menu;
	musique_menu.openFromFile("ressources/musique_menu.ogg");
	musique_menu.setLoop(true);*/
	//musique_menu.play();

	/*Music musique;
	musique.openFromFile("ressources/musique.ogg");
	musique.setLoop(true);*/

	bool quitter = false;

	int choix = RIEN;

	while(!quitter)
	{

		while(choix == RIEN)
		{
			Event event;
			while(fenetre.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					fenetre.close();
					choix = QUITTER;
					quitter = true;
				}
				if (event.type == Event::MouseButtonReleased)
				{
					if (SpriteUnJoueur.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre))))
					{
						choix = UN_JOUEUR;
					}
					if (SpriteDeuxJoueurs.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre))))
					{
						choix = DEUX_JOUEURS;
					}
				}
			}

			if (SpriteUnJoueur.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre))))
				SpriteUnJoueur.setTexture(unJoueurSelect);
			else
				SpriteUnJoueur.setTexture(unJoueur);

			if (SpriteDeuxJoueurs.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre))))
				SpriteDeuxJoueurs.setTexture(deuxJoueursSelect);
			else
				SpriteDeuxJoueurs.setTexture(deuxJoueurs);

			fenetre.clear(Color(200, 220, 220));
			fenetre.draw(SpriteUnJoueur);
			fenetre.draw(SpriteDeuxJoueurs);
			fenetre.display();
		}

		//On lance le jeu

		/*musique_menu.stop();
		musique.play();*/
		if (choix != QUITTER)
		{
			if (Jeu(&fenetre, choix) == QUITTER)
				quitter = true;
		}
		
		choix = RIEN;

		/*musique.stop();
		musique_menu.play();*/
	}

	//musique_menu.stop();

	fenetre.close();

	std::cout << "Programme quitté correctement.\n";

	return 0;
}