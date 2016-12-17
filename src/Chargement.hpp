#ifndef CHARGEMENT_HPP
#define CHARGEMENT_HPP

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>

#include "fonctions.hpp"
#include "Heros.hpp"
#include "Stats.hpp"
//#include "fonctions.hpp"

enum {BOXEUR, STICKMAN, LI};
enum {FUMEE, FEU};
enum {BG_1, BG_1_3D, BG_2, BG_3};
enum {FOND_MENU_PRINCIPAL, BOUTON_COMBAT_VS, BOUTON_COMBAT_VS_ACTIVE, BOUTON_QUITTER, BOUTON_QUITTER_ACTIVE,
	  FOND_SELECTION_PERSONNAGES, CADRE, CURSEUR_J1, CURSEUR_J2, BOUTON_RETOUR, BOUTON_RETOUR_ACTIVE, BOUTON_JOUER, BOUTON_JOUER_ACTIVE, BOUTON_IA_ON,
	  BOUTON_IA_ON_ACTIVE, BOUTON_IA_OFF, BOUTON_IA_OFF_ACTIVE};

class Chargement
{
public:
	static bool chargerRessources(void);
	static const sf::Texture getTexture(std::string nomCombattant);
	static const sf::Texture getTextureEffet(unsigned int effet);
	static const sf::Texture getTextureBackground(unsigned int background);
	static const std::vector<std::string> getNomCombattants(void);
	static const Stats getStats(std::string nomPerso);
	static const std::vector<std::vector<HitBox> >getHitBox(std::string nomPerso);
	static const std::vector<std::vector<Coup>> getCoups(std::string nomPerso);
	static void chargerPerso(Heros *heros);
	static void criWilhelm();
	static void sonCoup(int i = 0);
	static void sonWosh();
	/*static const std::vector<std::string> nomCombattants();
	static const std::vector<*/
private:
	//Tous les tableaux ont la même taille : le nombre de combattants.
	static std::vector<std::string> m_nomCombattants;
	static std::vector<sf::Texture> m_textures; //un tableau qui contient les textures des combattants, dans le même ordre que nomsCombattants
	static std::vector<sf::Texture> m_effets;
	static std::vector<sf::Texture> m_backgrounds;
	static std::vector<sf::Texture> m_menu;
	static std::vector<Stats> m_stats;
	static std::vector<std::vector<std::vector<HitBox>>> m_hitbox;
	static std::vector<std::vector<std::vector<Coup>>> m_coups;
	static sf::SoundBuffer m_bufferWilhelm;
	static sf::SoundBuffer m_bufferWosh;
	static std::vector<sf::SoundBuffer> m_bufferCoups;

};

#endif