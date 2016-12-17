#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "fonctions.hpp"

int credits(sf::RenderWindow *fenetre)
{

	sf::View vue900x600, vue300x200;
	vue900x600.setSize(sf::Vector2f(900, 600));
	vue900x600.setCenter(sf::Vector2f(450, 300));

	fenetre->setView(vue900x600);

	sf::Font freePixel;
	freePixel.loadFromFile("ressources/freePixel.ttf");

	sf::Text texteMoi, texteSFML, texteGabi, texteEtoiles1, texteEtoiles2;
	texteMoi.setFont(freePixel);
	texteSFML.setFont(freePixel);
	texteGabi.setFont(freePixel);
	texteEtoiles1.setFont(freePixel);
	texteEtoiles2.setFont(freePixel);


	texteMoi.setString(L"Réalisé par Sami Lefebvre");
	texteSFML.setString(L"à l'aide de la bibliothèque libre SFML");
	texteGabi.setString(L"Merci à Gabriel Mercier pour le jambon !");
	texteEtoiles1.setString("*******");
	texteEtoiles2.setString("*******");

	texteEtoiles1.setPosition(sf::Vector2f(450 - texteEtoiles1.getGlobalBounds().width/2, 150));
	texteEtoiles2.setPosition(sf::Vector2f(450 - texteEtoiles2.getGlobalBounds().width/2, 380));

	texteMoi.setPosition(sf::Vector2f(450 - texteMoi.getGlobalBounds().width/2, 200));
	texteSFML.setPosition(sf::Vector2f(450 - texteSFML.getGlobalBounds().width/2, 240));
	texteGabi.setPosition(sf::Vector2f(450 - texteGabi.getGlobalBounds().width/2, 320));

	while (true)
	{
		sf::Event event;
		while (fenetre->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				return QUITTER;
			}
			if(event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Escape)
					return RETOUR_MENU;
			}
		}			

		fenetre->clear(sf::Color(0, 10, 20));
		fenetre->draw(texteEtoiles1);
		fenetre->draw(texteMoi);
		fenetre->draw(texteSFML);
		fenetre->draw(texteGabi);
		fenetre->draw(texteEtoiles2);
		fenetre->display();
	}
}