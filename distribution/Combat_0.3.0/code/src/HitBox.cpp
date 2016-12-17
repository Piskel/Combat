#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "HitBox.h"
#include "fonctions.h"

using namespace sf;

HitBox::HitBox()
{
	m_type = CORPS;
}

HitBox::HitBox(FloatRect hitBox, int type)
{
	m_hitBox = hitBox;
	m_type = type;
}

FloatRect HitBox::hitBox()
{
	return m_hitBox;
}

Vector2f HitBox::getPosition()
{
	return Vector2f(m_hitBox.left, m_hitBox.top);
}

void HitBox::setPosition(sf::Vector2f position)
{
	m_hitBox.left = position.x;
	m_hitBox.top = position.y;
}

int HitBox::getType()
{
	return m_type;
}

void HitBox::afficher(RenderWindow *fenetre)
{
	RectangleShape rect(Vector2f(m_hitBox.width, m_hitBox.height));
	rect.setPosition(Vector2f(m_hitBox.left, m_hitBox.top));
	switch(m_type)
	{
		case CORPS :
			rect.setFillColor(Color(100, 200, 200, 170));
		break;
		case TETE :
			rect.setFillColor(Color(200, 200, 20, 170));
		break;
		case JAMBES :
			rect.setFillColor(Color(100, 255, 50, 170));
		break;
		case OFFENSIF :
			rect.setFillColor(Color(200, 100, 20, 170));
		break;
	}
	fenetre->draw(rect);
}