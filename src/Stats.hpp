#ifndef STATS_HPP
#define	STATS_HPP

#include "Heros.hpp"

class Stats
{
public:
	void appliquer(Heros *heros)
	{
		heros->setTailleSprite(tailleSprite);
		heros->setOrigineSprite(origineSprite);
		heros->setVitesse(vitesse);
		heros->chargerInfosAnim(framesSurPlace, framesMarche);
		heros->setDefense(defense);
		heros->setTempsDemiTour(sf::milliseconds(demiTour));
	}

	sf::Vector2i tailleSprite;
	sf::Vector2f origineSprite;
	int vitesse;
	int framesSurPlace, framesMarche, demiTour;
	float defense;
};

#endif