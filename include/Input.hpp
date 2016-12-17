#ifndef INPUT_H
#define INPUT_H

#include "Event.hpp"

namespace in {

class Input
{
	public:
		Input(); //constructeur par défaut.
		Input(std::vector<sf::Keyboard::Key> _keys); //constructeur fournissant des touches

		void addKey(sf::Keyboard::Key key); //ajoute une touche aux touches écoutées
		void setKeys(std::vector<sf::Keyboard::Key> keys);
		void initKeys(); // Réinitialise le tableau qui contient les touches surveillées

		void update(); //à faire à chaque frame pour récupérer les évenements.
		// Déjà inclue dans les méthodes pollEvent et waitEvent !!

		in::Event pollEvent(); //renvoie les évenements, s'il n'y en a pas, ne fait rien.
		in::Event waitEvent(); //bloque le programme tant qu'auccun évenement n'est trouvé.
		std::vector<sf::Keyboard::Key> getKeys(); //retourne la liste des touches écoutées

	protected:
		std::vector<sf::Keyboard::Key> keys; //Ce tableau contient toutes les touches survreillées par la classe Input. Les autres touches seront ignorées
		std::vector<bool> isPressed; //Ce tableau indique si une touche est pressée ou non.
		std::vector<sf::Clock> clock; //donne le temps que la touche a été appuyée.
		std::vector<in::Event> events; //tableau qui stocke les évenements

};

} //namespace in

#endif