#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "HitBox.h"
#include "fonctions.h"

using namespace sf;

Coup::Coup()
{

}

Coup::Coup(int _degats, Vector2f _force, Time _cooldown)
{
	degats = _degats;
	force = _force;
	cooldown = _cooldown;
}

//retourn l'anim qui doit être jouée en fonction du temps écoulé depuis le début du coup.
int Coup::anim(Time tempsEcoule)
{
	Time temps = seconds(0);
	
	for (unsigned int i = 0; i < m_dureeAnim.size(); i++)
	{
		if (tempsEcoule <= m_dureeAnim[i] + temps)
		{
			return m_anim[i];
		}

		temps += m_dureeAnim[i];
	}

	return -1;
}

Time Coup::tempsTotal()
{
	Time temps;
	for (unsigned int i = 0; i < m_dureeAnim.size(); i++)
	{
		temps += m_dureeAnim[i];
	}

	return temps;
}

void Coup::ajouterFrame(int numAnim, Time dureeAnim)
{
	m_anim.push_back(numAnim);
	m_dureeAnim.push_back(dureeAnim);
}