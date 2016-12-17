#include <SFML/Audio.hpp>
#include "Chargement.hpp"

using std::cout;
using std::endl;

//Variables statiques :
std::vector<sf::Texture> Chargement::m_textures;
std::vector<sf::Texture> Chargement::m_effets; //les effets graphiques (fumée, feu...)
std::vector<sf::Texture> Chargement::m_backgrounds;
std::vector<sf::Texture> Chargement::m_menu;

std::vector<std::string> Chargement::m_nomCombattants;
std::vector<Stats> Chargement::m_stats;
std::vector<std::vector<std::vector<HitBox> > > Chargement::m_hitbox;
std::vector<std::vector<std::vector<Coup> > > Chargement::m_coups;

sf::SoundBuffer Chargement::m_bufferWilhelm;
sf::SoundBuffer Chargement::m_bufferWosh;
std::vector<sf::SoundBuffer> Chargement::m_bufferCoups;

bool Chargement::chargerRessources()
{
	std::vector<std::string> persos = chargerPersonnagesDisponibles();

	cout << "Chargement des ressources...\n";

	cout << "\nChargement des effets sonores :\n";

	if (m_bufferWilhelm.loadFromFile("ressources/son/wilhelm.ogg"))
		cout << "ressources/son/wilhelm.ogg\n";

	std::string str = "coup-";
	sf::SoundBuffer buffer;

	for (int i = 0; i < 2; i++)
	{
		if (buffer.loadFromFile("ressources/son/coup-" + intToString(i+1) + ".ogg"))
		{
			cout << "ressources/son/coup-" << i + 1 << ".ogg\n";
			m_bufferCoups.push_back(buffer);
		}
	}

	if (buffer.loadFromFile("ressources/son/woosh.ogg"))
	{
		cout << "ressources/son/woosh.ogg\n";
		m_bufferWosh = buffer;
	}


	for (unsigned int i = 0; i < persos.size(); i++)
	{
		cout << "\nChargement de " << persos[i] << " :\n";

		m_nomCombattants.push_back(persos[i]);

		m_textures.push_back(sf::Texture());
		m_stats.push_back(Stats());
		m_hitbox.push_back(std::vector<std::vector<HitBox>>());
		m_coups.push_back(std::vector<std::vector<Coup>>());

		if (!m_textures[i].loadFromFile("ressources/img/sprites/" + persos[i] + ".png"))
			cout << "Erreur : impossible de charger ressources/img/sprites/"  + persos[i] + ".png\n";
		else
			cout << "ressources/img/sprites/"  + persos[i] + ".png\n";
		try
		{
			m_stats[i] = chargerStats(persos[i]);
		}
		catch(std::string s)
		{
			cout << "Erreur pendant le chargement de 'ressources/stats/" << persos[i] << "' :\n";
			cout << s << endl;
		}

		m_coups[i] = coupsDepuisUnFichier(persos[i]);
		m_hitbox[i] = HitBoxDepuisUnFichier(persos[i]);
	}

	sf::Texture texture;

	cout << "\nChargement des backgrounds :\n";

	if (texture.loadFromFile("ressources/img/backgrounds/temple-1.png"))
		cout << "ressources/img/backgrounds/temple-1.png\n";
	else
		cout << "ERREUR ASKIP\n";
	m_backgrounds.push_back(texture);

	if (texture.loadFromFile("ressources/img/backgrounds/temple-1-3D.png"))
		cout << "ressources/img/backgrounds/temple-1-3D.png\n";
	m_backgrounds.push_back(texture);

	if (texture.loadFromFile("ressources/img/backgrounds/temple-2.png"))
		cout << "ressources/img/backgrounds/temple-2.png\n";
	m_backgrounds.push_back(texture);

	if (texture.loadFromFile("ressources/img/backgrounds/temple-3.png"))
		cout << "ressources/img/backgrounds/temple-3.png\n";
	m_backgrounds.push_back(texture);

	cout << "\nChargement des particules :\n";

	if (texture.loadFromFile("ressources/img/effets/fumee.png"))
		cout << "ressources/img/effets/fumee.png\n";
	m_effets.push_back(texture);

	if (texture.loadFromFile("ressources/img/effets/feu.png"))
		cout << "ressources/img/effets/feu.png\n";
	m_effets.push_back(texture);

	cout << endl;

	return true;

}

void Chargement::criWilhelm()
{
	static sf::Sound wilhelm;
	wilhelm.setBuffer(m_bufferWilhelm);
	wilhelm.play();
}

void Chargement::sonCoup(int i)
{
	static sf::Sound coup;
	coup.setBuffer(m_bufferCoups[i]);
	coup.play();
}

void Chargement::sonWosh()
{
	static sf::Sound wosh;
	wosh.setBuffer(m_bufferWosh);
	wosh.play();
}

void Chargement::chargerPerso(Heros *perso)
{
	std::string nomPerso = perso->nom;

	sf::Texture texture = Chargement::getTexture(nomPerso);

	Stats stats = Chargement::getStats(nomPerso);
	stats.appliquer(perso);

	perso->chargerHitBox(Chargement::getHitBox(nomPerso));
	perso->chargerCoups(Chargement::getCoups(nomPerso));
	perso->setTexture(texture);
}

const std::vector<std::string> Chargement::getNomCombattants(void)
{
	return m_nomCombattants;
}

const sf::Texture Chargement::getTexture(std::string nomPerso)
{
	sf::Texture texture;
	for (unsigned int i = 0; i < m_nomCombattants.size(); i++)
	{
		if (m_nomCombattants[i] == nomPerso)
			texture = m_textures[i];
	}
	return texture;
}

const Stats Chargement::getStats(std::string nomPerso)
{
	Stats stats;
	for (unsigned int i = 0; i < m_nomCombattants.size(); i++)
	{
		if (m_nomCombattants[i] == nomPerso)
			stats = m_stats[i];
	}

	return stats;
}

const std::vector<std::vector<HitBox>> Chargement::getHitBox(std::string nomPerso)
{
	std::vector<std::vector<HitBox>> hitbox;
	for (unsigned int i = 0; i < m_nomCombattants.size(); i++)
	{
		if (m_nomCombattants[i] == nomPerso)
			hitbox = m_hitbox[i];
	}
	return hitbox;
}

const std::vector<std::vector<Coup>> Chargement::getCoups(std::string nomPerso)
{
	std::vector<std::vector<Coup>> coups;
	for (unsigned int i = 0; i < m_nomCombattants.size(); i++)
	{
		if (m_nomCombattants[i] == nomPerso)
			coups = m_coups[i];
	}
	return coups;
}

const sf::Texture Chargement::getTextureEffet(unsigned int effet)
{
	if (effet >= m_effets.size())
	{
		effet = m_effets.size() - 1;
		throw std::string(effet + " : Effet non disponible");
	}

	return m_effets[effet];
}

const sf::Texture Chargement::getTextureBackground(unsigned int background)
{
	if (background >= m_backgrounds.size())
	{
		background = m_backgrounds.size() - 1;
		//throw std::string(background + " : texture non disponible");
	}

	return m_backgrounds[background];
}