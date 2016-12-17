#include <iostream>
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(unsigned int _nbParticules, sf::Time _dureeDeVie)
{
	particules.clear();
	sprites.clear();
	setNbParticules(_nbParticules);
	vertices.setPrimitiveType(sf::LinesStrip);
	vertices.resize(0);
	dureeDeVie = _dureeDeVie;
}

Particule::Particule(sf::Vector2f _position, sf::Vector2f _vitesse, sf::Time _lifetime)
{
	position = _position;
	vitesse = _vitesse;
	lifetime = _lifetime;
}

void ParticleSystem::setNbParticules(unsigned int _nbParticules)
{
	//vertices.resize(0);
	nbParticules = _nbParticules;

	while (sprites.size() > nbParticules)
	{
		sprites.pop_back();
	}

	while (sprites.size() < nbParticules)
	{
		sprites.push_back(sf::Sprite());
	}

	for (unsigned int i = 0; i < nbParticules; i++)
	{
		sprites[i].setTexture(texture);
	}
}

void ParticleSystem::loadTexture(std::string s_texture)
{
	if (!texture.loadFromFile(s_texture))
	{
		std::cout << "Erreur : impossible d'ouvrir '" << s_texture << "' !\n";
	}
	else
		std::cout << "La texture '" << s_texture << "'' a été chargée !\n";

	for (unsigned int i = 0; i < nbParticules; i++)
	{
		sprites[i].setTexture(texture);
	}
}

void ParticleSystem::addParticule(sf::Vector2f position, sf::Vector2f vitesse)
{
	if (particules.size() < nbParticules)
	{
		particules.push_back(Particule(position, vitesse, sf::seconds(0)));
	}
}

void ParticleSystem::update(float fps)
{
	for (unsigned int i = 0; i < particules.size(); i++)
	{
		particules[i].lifetime += sf::seconds(1/fps);
		particules[i].position += particules[i].vitesse/fps;
	}

	
	for (unsigned int i = 0; i < particules.size(); i++)
	{
		if (particules[i].lifetime > dureeDeVie)
		{
			particules.erase(particules.begin() + i);

		}
	}


	for (unsigned int i = 0; i < particules.size(); i++)
	{
		sprites[i].setPosition(sf::Vector2f(particules[i].position.x -10, particules[i].position.y + 10));
		//suivant combien de temps a vécu la particule, on décide de l'animation à mettre.
		int anim = (particules[i].lifetime.asSeconds()/dureeDeVie.asSeconds()) * nbAnims;
		sprites[i].setTextureRect(sf::IntRect(20 * anim, 0, 20, 20));

		/*sf::Vertex* quad = &vertices[i * 4];

		quad[0].position = sf::Vector2f(particules[i].position.x -5, particules[i].position.y - 5);
		quad[1].position = sf::Vector2f(particules[i].position.x +5, particules[i].position.y - 5);
		quad[2].position = sf::Vector2f(particules[i].position.x +5, particules[i].position.y + 5);
		quad[3].position = sf::Vector2f(particules[i].position.x -5, particules[i].position.y + 5);

		vertices[i*4].texCoords = sf::Vector2f(0,0);
		vertices[i*4 + 1].texCoords = sf::Vector2f(20,0);
		vertices[i*4 + 2].texCoords = sf::Vector2f(20,20);
		vertices[i*4 + 3].texCoords = sf::Vector2f(20,0);*/

	}
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	 // on applique la transformation
	states.transform *= getTransform();

	states.texture = &texture;

	//target.draw(vertices, states);

	for (unsigned int i = 0; i < particules.size(); i++)
	{
		target.draw(sprites[i], states);
	}
}