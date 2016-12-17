#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct Particule
{
	Particule(sf::Vector2f _position, sf::Vector2f _vitesse, sf::Time _lifetime);
	sf::Time lifetime;
	sf::Vector2f position;
	sf::Vector2f vitesse;
};

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
	ParticleSystem(unsigned int _nbParticules = 1, sf::Time _dureeDeVie = sf::seconds(0.4));

	void setTexture(sf::Texture _texture);
	void setNbParticules(unsigned int _nbParticules);
	void loadTexture(std::string s_texture);

	void addParticule(sf::Vector2f position, sf::Vector2f vitesse = sf::Vector2f(0,0));

	void update(float fps);

	unsigned int nbAnims;
	sf::Time dureeDeVie;
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	unsigned int nbParticules;
	sf::Texture texture;
	sf::VertexArray vertices;
	std::vector<sf::Sprite> sprites;
	std::vector<Particule> particules;
};

#endif