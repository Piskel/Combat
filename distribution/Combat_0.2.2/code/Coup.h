#ifndef COUP
#define COUP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "HitBox.h"
#include "fonctions.h"

//La classe Coup permet d'avoir des infos sur le coup porté et facilite la lecture des coups depuis un fichier.

class Coup
{
	public:
		Coup();
		Coup(int degats, sf::Vector2f force = sf::Vector2f(0, 0), sf::Time cooldown = sf::seconds(0));

		int anim(sf::Time tempsEcoule = sf::seconds(0)); //Permet de retourner le n° de l'animation qui doit être jouée en fonction du temps écoulé.

		void ajouterFrame(int numAnim, sf::Time dureeAnim);
		sf::Time tempsTotal();

		//Variables publiques parce que c'est plus pratique :
		int degats;
		sf::Time cooldown; //Le cooldown propre au coup.
		sf::Time KO; //Le temps de KO infligé par ce coup.
		sf::Vector2f force; //La force du coup, en X et Y, qui sert à déterminer à quelle vitesse est projeté l'adversaire (en px/s)
		//Ce truc s'appelle la Force, mais ça n'a rien à voir avec une VRAIE force ou même Star Wars, c'est en réalité la vitesse d'éjection.
		//J'aime les noms de variables inappropriés.
		
	private:
		
		std::vector<int> m_anim; //Tableau qui contient les numéros des animations qui correspondent au coup.
		std::vector<sf::Time> m_dureeAnim; //Pour chaque frame du coup, contient la durée de cette frame.
};

#endif