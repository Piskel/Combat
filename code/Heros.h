#ifndef HEROS
#define HEROS

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "HitBox.h"
#include "fonctions.h"

/*
*
*	La classe Héros est la classe la plus importante du programme.
*	Elle contient tout ce qui est relatif au Personnage, soit beaucoup de choses.
*	Voilà.
*
*/

class Heros
{
	public:
		Heros();
		Heros(sf::Vector2f position = sf::Vector2f(400,300), int direction = GAUCHE);

		//Méthodes relatives au chargement du héros.
		void setTexture(sf::Texture &texture);
		void chargerHitBox(std::vector<std::vector<HitBox> > hitbox); //A la création du héros, charge les hitbox de chaque frame.
		void chargerCoups(std::vector<std::vector<Coup> > coup); //À la création du héros également, on charge les coups.
		void chargerInfosAnim(int framesSurPlace, int framesMarche); //informations sur le nombre de frame pour l'animation de marche, etc.
		void setVitesse(int vitesse); //Change la vitesse du héros, en px/s
		void setTempsDemiTour(sf::Time temps);
		void setTailleSprite(sf::Vector2i tailleSprite);
		void setOrigineSprite(sf::Vector2f origineSprite);
		void setDefense(float defense);

		//Méthodes constantes permettant de récupérer des informations.
		std::vector<HitBox> getHitBox();
		const sf::Vector2f getPosition();
		const sf::Vector2f getVitesse();
		const int getDirection();
		const int getPV();
		const bool estVivant();

		//Méthodes concernant des actions
		void changerDirection(int direction);
		void marcher(int direction = DROITE); //DROITE ou GAUCHE
		void accroupir();
		void sauter();
		void frapper();
		void recevoirCoup(Coup coup, float vitesse, int direction);

		//Mises à jour  (à faire toutes les frames)
		void update(float fps, Heros *ennemi); //applique les effets de la gravité, collisions, changement d'anim, ...
		void init();

		//Affichage et debug
		void debug(bool debug); //Change le mode de debug (true ou false)
		void afficher(sf::RenderWindow *fenetre);

	private:

		bool m_marche; //vaut true si le héros marche.
		bool m_courir;
		bool m_coup; //vaut true si le héros frappe.
		bool m_saut; //vaut true si le héros appuie sur saut.
		bool m_air; //vaut true si le heros est en l'air.
		bool m_debug; //vaut true pour le mode debug.
		bool m_doubleSaut; //vaut true si le double saut n'a pas été utilisé.
		bool m_dash;
		bool m_accroupi; //Vaut true si le héros est accroupi.
		bool m_seRetourner; //Pour le retournement (eh oui...)
		bool m_directionChangee;
		int m_PV;
		float m_defense; //Indique à quel point le héros est résistant ou non aux coups (dégats + vol) : 1 = normal, 2 = 2*plus résistant.
		int m_coupID; //l'identifiant du coup, pour s'y retrouver. NORMAL, BAS, ... (NUL = aucun coup)
		int m_coupPrecedant;
		unsigned int m_combo; //Vaut 0 puis est incrémenté de 1 à chaque coup du même type, jusqu'au nb max de combo (EN COURS, PAS EN PLACE)
		int m_vitesseDeplacement; // en px/s
		int m_nombreFramesSurPlace;
		int m_nombreFramesMarche;
		int m_vitesseDash;
		int m_direction; //DROITE ou GAUCHE -- La flemme d'utiliser les enums avec un type.
		int m_anim; //numéro de la frame en cours

		//texture et sprite.
		sf::Texture m_texture;
		sf::Sprite m_sprite;
		sf::Vector2i m_tailleSprite;

		sf::Vector2f m_vitesse;//La vitesse du perso

		sf::Time m_recover; //Le temps où le héros ne peut plus attaquer après une attaque. vaut 0s si il n'y a pas de temps de recover.
		sf::Time m_KO; //le temps où le héros sera KO.
		sf::Time m_tempsDemiTour; //Le temps que met le héros à faire un demi tour.

		std::vector<HitBox> m_hitboxCourante; //pointeur vers un tableau de hitbox contenu dans m_hitbox, et qui contient la hitbox
		//pour la frame en cours.
		std::vector<std::vector <HitBox> > m_hitbox; //contient tous les tableaux de HitBox de chaque frame.
		//Chargé lors de la création du héros.

		std::vector<std::vector <Coup> > m_coups; // m_coups[NORMAL][0] => le coup n°0 de normal.

		//une clock pour les animations :
		sf::Clock clockAnim;
		//Et une pour le reste (Ko, recover, ...)
		sf::Clock clock;
		//Et aussi une pour le dash et autres (temps entre deux appuis de touches)
		sf::Clock clockDash;
		//Une pour l'intervalle de temps entre les coups, pour les combos
		sf::Clock clockCombo;
		//Une pour savoir si le héros peut de nouveau recevoir un coup (pas plus d'1 coup toutes les x ms)
		sf::Clock clockCoup;
};

#endif

/*

CoupID & combo :
	
	coupID représente le type du coup : NORMAL, BAS, HAUT, tout ça tout ça...
	combo sert à savoir le nombre de coups du même type assénés à la suite, pour justement faire des combos.

	Exemple :

		m_coups : vector<vector<Coup>>

		État 0 : coupID = NUL
				 combo = 0

		Monsieur Test frappe une fois, normalement (debout).
		coupID passe à NORMAL
		Le coup sera celui trouvé dans m_coups[NORMAL][combo = 0]
		BOUM ! Le coup est appliqué
		combo <- combo + 1

		Monsieur Test frappe à nouveau, rapidement après (intervalle < x secondes)
		coupID reste à NORMAL
		Le coup sera m_coups[NORMAL][combo = 1]
		PIF ! PAF ! (autre coup, possiblement plus puissant)
		Comme m_coups[NORMAL].size() vaut 2, combo passe à 0.
			
*/