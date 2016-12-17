#ifndef HITBOX
#define HITBOX

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

//Petit objet HitBox qui contient deux trois arguments et méthodes utiles.

class HitBox
{
	public:
		HitBox();
		HitBox(sf::FloatRect hitBox, int type);

		void setPosition(sf::Vector2f position);
		sf::Vector2f getPosition();

		sf::FloatRect hitBox(); //retourne le rectangle de la hit-box.
		int getType();
		int estTouchee(); //Dit si la hitbox a été touchée par une hitbox offensive.
		//retourne -1 si pas touchée, sinon retourne l'ID de la hitbox qui touche (entre 0 et n)

		void afficher(sf::RenderWindow *fenetre); //pour le debug : affiche la hitbox par-dessus les combatants

	private:
		sf::FloatRect m_hitBox; //Le rectangle d'action : la hitbox proprement dite.
		int m_type;
};

#endif