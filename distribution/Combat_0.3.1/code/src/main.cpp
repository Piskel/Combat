#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Heros.hpp"
#include "Joueur.hpp"
#include "jeu.hpp"
#include "fonctions.hpp"
#include "Chargement.hpp"
#include "main.hpp"

using namespace sf;

int main()
{
	std::cout << "Lancement du programme...\n\n";

	Chargement::chargerRessources();

	RenderWindow fenetre(VideoMode(900,600,32), "~ Combat", Style::Close | Style::Titlebar);
	fenetre.setVerticalSyncEnabled(true);

	Texture bouton_quitter_text, bouton_quitter_active_text, bouton_combat_text, bouton_combat_active_text, bouton_credits_text, bouton_credits_active_text, fond_text;
	fond_text.loadFromFile("ressources/img/menus/menu_principal.png");
	bouton_quitter_text.loadFromFile("ressources/img/menus/bouton_quitter.png");
	bouton_quitter_active_text.loadFromFile("ressources/img/menus/bouton_quitter_active.png");
	bouton_combat_text.loadFromFile("ressources/img/menus/bouton_combat_vs.png");
	bouton_combat_active_text.loadFromFile("ressources/img/menus/bouton_combat_vs_active.png");
	bouton_credits_text.loadFromFile("ressources/img/menus/bouton_credits.png");
	bouton_credits_active_text.loadFromFile("ressources/img/menus/bouton_credits_active.png");

	Sprite bouton_quitter, bouton_combat, bouton_credits, fond;
	fond.setTexture(fond_text);
	bouton_combat.setPosition(Vector2f(75,80));
	bouton_credits.setPosition(Vector2f(75,110));
	bouton_quitter.setPosition(Vector2f(75, 140));

	std::string j1("default"), j2("default");

	sf::SoundBuffer jambonBuffer;
	jambonBuffer.loadFromFile("ressources/son/jambon.ogg");

	sf::Sound jambon;
	jambon.setBuffer(jambonBuffer);

	bool jambon_active = false;

	// Pas de musique convaincante libre de droit sous la main

	Music musique_menu;
	musique_menu.openFromFile("ressources/musique/musique_menu.ogg");
	musique_menu.setLoop(true);
	musique_menu.setVolume(85);
	musique_menu.play();

	/*Music musique;
	musique.openFromFile("ressources/musique/musique.ogg");
	musique.setLoop(true);*/

	bool quitter = false;

	View vue;
	vue.setSize(300, 200);
	vue.setCenter(150, 100);

	int choix = RIEN;

	while(!quitter)
	{
		fenetre.setView(vue);
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
					if (bouton_combat.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre)/3)))
					{
						choix = DEUX_JOUEURS;
					}
					if (bouton_quitter.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre)/3)))
					{
						choix = QUITTER;
						quitter = true;
					}
					if (bouton_credits.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre)/3)))
					{
						choix = credits(&fenetre);
						if (choix == QUITTER)
							quitter = true;
						else
							choix = RIEN;
						fenetre.setView(vue);
					}
				}
			}

			if (bouton_combat.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre)/3)))
			{
				bouton_combat.setTexture(bouton_combat_active_text);
				if (!jambon_active)
				{
					jambon_active = true;
					jambon.play();
				}
			}
			else
			{
				jambon_active = false;
				bouton_combat.setTexture(bouton_combat_text);
			}

			if (bouton_credits.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre)/3)))
				bouton_credits.setTexture(bouton_credits_active_text);
			else
				bouton_credits.setTexture(bouton_credits_text);

			if (bouton_quitter.getGlobalBounds().contains(Vector2f(Mouse::getPosition(fenetre)/3)))
				bouton_quitter.setTexture(bouton_quitter_active_text);
			else
				bouton_quitter.setTexture(bouton_quitter_text);

			fenetre.draw(fond);
			fenetre.draw(bouton_combat);
			fenetre.draw(bouton_credits);
			fenetre.draw(bouton_quitter);
			fenetre.display();
		}

		//On lance le jeu

		/*musique_menu.stop();
		musique.play();*/
		if (choix != QUITTER)
		{
			int i = combat_vs(&fenetre);

			if (i == QUITTER)
				quitter = true;
		}
		
		choix = RIEN;

		/*musique.stop();
		musique_menu.play();*/
	}

	musique_menu.stop();

	fenetre.close();

	std::cout << "Programme quitté correctement.\n";

	return 0;
}