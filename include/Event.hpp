#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

namespace in {

	class Event
	{
		public:
			Event();
			Event(sf::Keyboard::Key _key, bool _isPressed = true, sf::Time _time = sf::seconds(0));

			sf::Keyboard::Key getKey();
			bool isKeyPressed();
			bool isKeyReleased();
			sf::Time getTime();
			bool isEmpty(); //renvoie true si l'évenement a les valeurs par défaut ou si la touche "Unknown" est appuyée.

			void rapport();

		private:
			sf::Keyboard::Key key;
			bool pressed;
			sf::Time time;
	};

} //fin du namespace in

#endif