#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "fonctions.hpp"
#include "Heros.hpp"
#include "HitBox.hpp"
#include "Coup.hpp"
#include "Chargement.hpp"

using namespace sf;

Heros::Heros(Vector2f position, int direction)
{
	m_PV = PV_MAX;
	m_defense = 0.9;
	m_anim = 0;
	m_vitesseDash = 500;
	nom = "default";
	m_coup = false;
	m_saut = false;
	m_roulade = false;
	m_air= false;
	m_debug = false;
	m_dash = false;
	m_accroupi = false;
	m_seRetourner = false;
	m_invincible = false;
	m_coupID = NUL;
	m_sprite.setPosition(position);
	m_direction = direction;
	m_vitesse = Vector2f(0,0);
	m_nombreFramesSurPlace = 3;
	m_nombreFramesMarche = 6;
	m_tempsDemiTour = seconds(0.2);
	fumee.setNbParticules(20);
	fumee.setTexture(Chargement::getTextureEffet(FUMEE));
	fumee.nbAnims = 10;
	fumee.dureeDeVie = seconds(0.5);
	feu.setNbParticules(20);
	feu.setTexture(Chargement::getTextureEffet(FEU));
	feu.nbAnims = 10;
	feu.dureeDeVie = seconds(0.65);
	m_ennemi = NULL;
}

Heros::~Heros()
{
	//pas besoin de delete m_ennemi, car *m_ennemi est lui aussi détruit quand on sort du scope
	//D'ailleurs, si on le met, on aune vilaine erreur desegmentation
}

/*********************************
*
* Méthodes d'accès (lecture)*
*
**********************************/

std::vector<HitBox> Heros::getHitBox()
{
	return m_hitboxCourante;
}

const int Heros::getPV()
{
	return m_PV;
}

const bool Heros::estVivant()
{
	if (m_PV > 0)
		return true;
	else
		return false;
}

const sf::Vector2f Heros::getPosition()
{
	return m_sprite.getPosition();
}

const int Heros::getDirection()
{
	return m_direction;
}


const Vector2f Heros::getVitesse()
{
	return m_vitesse;
}

const bool Heros::estAccroupi()
{
	return m_accroupi;
}

const bool Heros::estKO()
{
	if (m_KO != seconds(0))
	{
		if ((!m_roulade) && (!m_seRetourner))
			return true;
	}
	return false;
}

/*********************************
*
* Méthodes d'accès (écriture)
*
**********************************/

void Heros::setTexture(Texture &texture)
{
	m_texture = texture;
	m_sprite.setTexture(m_texture);
	//m_sprite.setOrigin(Vector2f(12,0));
}

void Heros::setTailleSprite(sf::Vector2i tailleSprite)
{
	m_tailleSprite = tailleSprite;
}

void Heros::setOrigineSprite(sf::Vector2f origineSprite)
{
	m_sprite.setOrigin(origineSprite);
}

void Heros::setVitesse(int vitesse)
{
	m_vitesseDeplacement = vitesse;
}

void Heros::setTempsDemiTour(Time temps)
{
	m_tempsDemiTour = temps;
}

void Heros::setDefense(float defense)
{
	m_defense = defense;
}

void Heros::chargerHitBox(std::vector<std::vector<HitBox> > hitbox)
{
	m_hitbox = hitbox;
}

void Heros::chargerCoups(std::vector<std::vector<Coup> > coups)
{
	m_coups = coups;
}

void Heros::chargerInfosAnim(int framesSurPlace, int framesMarche)
{
	m_nombreFramesSurPlace = framesSurPlace;
	m_nombreFramesMarche = framesMarche;
}

void Heros::setPosition(Vector2f position)
{
	m_sprite.setPosition(position);
}

void Heros::setDirection(int direction)
{
	m_direction = direction;
}

void Heros::setEnnemi(Heros *ennemi)
{
	m_ennemi = ennemi;
}

void Heros::ressuciter()
{
	m_PV = PV_MAX;
}

/***********************************************
*
* Méthodes pour le déplacement et les actions
*
************************************************/

void Heros::changerDirection(int direction)
{
	if ((!m_coup) && (m_direction != direction) && (!m_seRetourner))
	{
		clock.restart();
		m_seRetourner = true;
		m_directionChangee = false;
		m_KO = m_tempsDemiTour;
		//On utilise le KO pour que le perso ne puisse pas marcher, sauter ou frapper. Cependant, il n'est pas vraiment KO.
	}
}

void Heros::changerVitesse(Vector2f vitesse)
{
	m_vitesse = vitesse;
}

void Heros::marcher(int direction)
{
	//On ne peut pas marcher si on frappe ou qu'on est en mode KO.
	if ((!m_coup) && (m_KO == seconds(0)))
	{
		if (m_direction != direction) //c.à.d si on se retourne.
		{
			clock.restart();
			m_seRetourner = true;
			m_directionChangee = false;
			m_KO = m_tempsDemiTour;
			//On utilise le KO pour que le perso ne puisse pas marcher, sauter ou frapper. Cependant, il n'est pas vraiment KO.
		}
		else
		{
			m_marche = true;
		}
	}
	
}

void Heros::accroupir()
{
	if ((m_KO == seconds(0)) && (!m_air))
	{
		m_accroupi = true;
	}	
}

void Heros::frapper(int coupID)
{
	if ((m_recover == seconds(0)) && (m_KO == seconds(0)) && (m_coup == false))
	{
		m_coup = true;
		m_coupID = coupID;
		clock.restart();

		//La petite gestion des combos
		if (m_coupPrecedent == m_coupID)
			m_combo++;

		if (clockCombo.getElapsedTime().asMilliseconds() > TEMPS_COMBO)
			m_combo = 0;
	}
}

void Heros::sauter()
{
	if ((m_KO == seconds(0)) && (!m_coup))
	{
		if (!m_air)
		{
			m_saut = true;
			m_doubleSaut = true;
		}
		else if (m_doubleSaut)
		{
			m_saut = true;
			m_doubleSaut = false;
		}

	}
}

void Heros::roulade(int direction)
{
	//On ne peut pas faire de roulade si on est en l'air, qu'on recouvre un coup, qu'on est KO ou qu'on se retourne
	if ((!m_air) && (!m_seRetourner) && (m_recover == seconds(0)) && (m_KO == seconds(0)))
	{
		Chargement::sonWosh();
		m_roulade = true;
		m_directionRoulade = direction;
		clockRoulade.restart();
		m_KO = seconds(TEMPS_ROULADE);
		clock.restart();
		m_invincible = true;
	}
}

void Heros::gravite(float fps)
{
	if (m_saut)
	{
		m_anim = 0; //en attendant de faire des animations pour les sauts

		if (!m_air) //premier saut
			m_vitesse.y = -VITESSE_SAUT;

		if (m_air) //Double saut
			m_vitesse.y = -VITESSE_DOUBLE_SAUT;

		m_saut = false;
		m_air = true;
	}
	
	m_vitesse.y += GRAVITE/fps;

	if (m_sprite.getPosition().y + m_vitesse.y/fps > HAUTEUR_TERRAIN - m_tailleSprite.y)
	{
		m_sprite.setPosition(m_sprite.getPosition().x, HAUTEUR_TERRAIN - m_tailleSprite.y);
		m_air = false;
		m_vitesse.y = 0;
	}
}

void Heros::demitour()
{
	if ((!m_coup) && (m_seRetourner)) //on ne peut pas se retourner en frappant !
	{
		if ((clock.getElapsedTime() > m_tempsDemiTour/2.0f) && (clock.getElapsedTime() < m_tempsDemiTour))
		{
			//On change la direction...

			if (!m_directionChangee)
			{
				if(m_direction == GAUCHE)
					m_direction = DROITE;
				else if(m_direction == DROITE)
					m_direction = GAUCHE;

				m_directionChangee = true;
			}
		}
		else if (clock.getElapsedTime() >= seconds(0.2))
		{
			m_seRetourner = false;
		}
	}
}

void Heros::recevoirCoup(Coup coup, float vitesse, int direction) //HAHA ÇA FAIT COUCOU ON SE BIDONNE LAULE
{
	if (!m_invincible)
	{
		//Application de la défense :
		Vector2f force = coup.force/m_defense;
		int degats = coup.degats/m_defense;

		//On vérifie le temps entre 2 attaques, puisque le héros ne peut être tapé qu'une fois toutes les 0,2 secondes.
		if (clockCoup.getElapsedTime() > seconds(0.2))
		{
			//Quand on est accroupi, on vole moins loin et on subit moins de dégats
			if (m_accroupi)
			{
				force *= 0.6f;
				degats *= 0.75;
			}

			if (degats <= 20)
				Chargement::sonCoup(1);
			else
				Chargement::sonCoup(1);

			m_PV -= degats; //La vérif des PV négatifs se fait dans update.

			//La vitesse d'éjection dépend de la force du coup, mais aussi de la vitesse de l'agresseur.
			if (direction == DROITE)
				force.x += vitesse/2;
			else if (direction == GAUCHE)
				force.x = -force.x + vitesse/2;

			//Plus on a de dégats, plus on vole.
			force += force*(1-m_PV/PV_MAX);

			//Si on se prend un coup très fort
			if (sqrt(pow(force.x, 2) + pow(force.y, 2)) > 320)
			{
				m_feu = true;
			}

			m_vitesse = force;

			if(m_vitesse.y < 0)
				m_air = true;

			//Le héros ne pourra plus rien faire pendant quelque temps.
			m_KO = coup.KO;
			clock.restart();

			m_coup = false; //on ne tape plus si on est KO
			m_combo = 0;

			clockCoup.restart();
		}
	}
}

/*************************************************************************************************
*
*	Grosse grosse méthode de Héros : à appeller chaque frame, pour... Updater le bazard.
*
*	Évidement, la méthode fait plus de 20 lignes : désolé pour ceux/celles qui codent proprement.
*
***************************************************************************************************/

void Heros::update(float fps)
{
	if (m_PV < 0)
		m_PV = 0;

	//Le personnage ouh qu'il est mort
	if(m_PV == 0)
	{
		m_anim = m_hitbox.size() - 1; //L'image de la mort du personnage est la dernière de la texture.

		if (!m_air)
		{
		//on diminue la vitesse, frottements du sol.
			m_vitesse.x *= 0.85;
		}
		else
			m_vitesse.x *= 0.98;

		gravite(fps);
		fumee.update(fps);
		feu.update(fps);

		m_sprite.move(m_vitesse/fps);

		m_hitboxCourante = attribuerHitBox(m_hitbox[m_anim], m_sprite, m_direction);

		//on met au sprite la texture correspondant à l'animation courante.
		m_sprite.setTextureRect(IntRect(m_anim*m_tailleSprite.x, 0, m_tailleSprite.x, m_tailleSprite.y));
		return;
	}
	///******************************************************************************************************************************

	//Pour le dash : on vérifie que le joueur est resté appuyé sur la touche flèche avec ce qui suit :
	if (!m_courir)
		m_dash = false;
		//On désactive le dash si le joueur n'est pas resté appuyé.

	//On vérifie que le héros peut bouger (qu'il n'est pas en KO)
	if (m_KO == seconds(0))
	{
		//S'il n'est pas KO, on va appliquer tout ce qu'il peut faire normalement

		//On applique la marche :
		if (m_marche)
		{
			if(m_direction == DROITE)
			{
				m_vitesse.x += INERTIE/fps;
				if (m_vitesse.x > m_vitesseDeplacement)
					m_vitesse.x = m_vitesseDeplacement;
			}
			else if (m_direction == GAUCHE)
			{
				m_vitesse.x -= INERTIE/fps;
				if (-m_vitesse.x > m_vitesseDeplacement)
					m_vitesse.x = -m_vitesseDeplacement;
			}

			//animations de la marche :
			if (!m_air)
			{
				if(clockAnim.getElapsedTime().asMilliseconds() >= milliseconds(100).asMilliseconds())
				{
					m_anim ++;
					/*if ((m_anim > 0) && (m_anim < 3))
						m_anim = 4;*/

					if ((m_anim < m_nombreFramesSurPlace + 1) || (m_anim > m_nombreFramesMarche + m_nombreFramesSurPlace))
					{
						m_anim = m_nombreFramesSurPlace + 1;
					}
						
					/*if (m_anim > 8)
						m_anim = 0;*/

					clockAnim.restart();
				}
			}
		}
		else if(m_accroupi)
		{
			//Si on tape, l'anim ne sera pas la 3
			if (!m_coup)
				m_anim = m_nombreFramesSurPlace; //l'anim accroupi se trouve juste après le sur-place
		}
		else if (!m_coup)
		{
			//Enfin, s'il ne marche pas, qu'il ne tape pas et n'est pas accroupi, animation sur place
			if(clockAnim.getElapsedTime().asMilliseconds() >= milliseconds(150).asMilliseconds())
			{
				m_anim ++;
				if(m_anim >= m_nombreFramesSurPlace)
					m_anim = 0;

				clockAnim.restart();
			}
		}

		//AANIMATIONS AÉRIENNES
		if (m_air)
		{
			m_anim = m_hitbox.size() - 4;
		}

		//BASTON

		if(m_coup)
		{
			if (m_combo >= m_coups[m_coupID].size())
				m_combo = 0;

			//On change l'animation du perso
			if (m_coupID < m_coups.size())
				m_anim = m_coups[m_coupID][m_combo].anim(clock.getElapsedTime());
			else
				throw std::string("ERREUR FATALE : on tente d'accéder à un coup qui n'est pas défini\n");

			//On attend que l'anim du coup soit terminée.
			if(clock.getElapsedTime() >= m_coups[m_coupID][m_combo].tempsTotal())
			{
				m_coup = false;
				clock.restart();
				m_recover = m_coups[m_coupID][m_combo].cooldown;

				m_coupPrecedent = m_coupID;

				if (m_coupID != COUP_NORMAL_BAS)
					m_anim = 0;
				else
					m_anim = m_nombreFramesSurPlace; //animation du coup bas

				clockCombo.restart();

				m_coupID = NUL;
			}
		}
	}
	else
	{
		//Si le temps de KO ne vaut pas 0, donc qu'on est KO :
		if (m_seRetourner)
		{
			if (!m_air)
				m_anim = m_hitbox.size() - 3; //l'animation de retournement.
			else
				m_anim = m_hitbox.size() - 4; //si on est en l'air, on joue pas l'anim de retournement
		}
		else if (m_roulade)
		{
			m_anim = m_nombreFramesSurPlace; //anim accroupi

			if (m_directionRoulade == GAUCHE)
				m_sprite.move(Vector2f(-VITESSE_ROULADE/fps, 0));
			else
				m_sprite.move(Vector2f(VITESSE_ROULADE/fps, 0));

			if (m_ennemi->getPosition().x - getPosition().x > 0)
				m_direction = DROITE;
			else
				m_direction = GAUCHE;
		}
		else
			m_anim = m_hitbox.size() - 2; //KO normal (image du KO donc)

		//GESTION DU SYSTÈME DE PARTICULES

		//Là on prend feu
		if ((!m_seRetourner) && (m_air) && (m_feu) && (clockParticules.getElapsedTime() > seconds(1/FREQUENCE_PARTICULES)))
		{
			clockParticules.restart();
			feu.addParticule(m_sprite.getPosition(), Vector2f(0, -10));
			m_PV -= 1;
		}

		if ((m_roulade) && (clockParticules.getElapsedTime() > seconds(1/FREQUENCE_PARTICULES)))
		{
			clockParticules.restart();
			fumee.addParticule(Vector2f(m_sprite.getPosition().x, m_sprite.getPosition().y + 5), Vector2f(0, -15));
		}

		if (clock.getElapsedTime() >= m_KO)
		{
			m_KO = seconds(0);
			clock.restart();
		}
	}
	//ROULAAAAAAAAAAAADE !
	if ((clockRoulade.getElapsedTime() > seconds(TEMPS_ROULADE)) && (m_roulade))
	{
		m_roulade = false;
		m_recover = seconds(0.01);
		clock.restart();
		m_invincible = false;
	}

	demitour();	

	//On change l'orientation du sprite en fonction de la direction du personnage :
	if(m_direction == GAUCHE)
		m_sprite.setScale(-1, 1);
	if(m_direction == DROITE)
		m_sprite.setScale(1, 1);



	//Pour le temps de recover des coups
	if(m_recover != seconds(0))
	{
		if (clock.getElapsedTime() >= m_recover)
		{
			m_recover = seconds(0);
			clock.restart();
		}
	}

	//on met au sprite la texture correspondant à l'animation courante.
	m_sprite.setTextureRect(IntRect(m_anim*m_tailleSprite.x, 0, m_tailleSprite.x, m_tailleSprite.y));

	gravite(fps);

	//On perd de la vie si on sort de l'écran
	if ((m_sprite.getPosition().x > LARGEUR_TERRAIN) || (m_sprite.getPosition().x < 0))
	{
		if (clockCoup.getElapsedTime() > seconds(0.03))
		{
			m_PV -= 1;
			clockCoup.restart();
		}
	}

	float distance = getPosition().x - m_ennemi->getPosition().x;

	//on effectue les déplacements

	//Collisions :
	if ((m_vitesse.x != 0) &&
		((abs(distance) < 15) &&
		(abs(getPosition().y - m_ennemi->getPosition().y) < 15)) &&
		(((distance < 0) && (m_vitesse.x > 0)) || ((distance > 0) && (m_vitesse.x < 0))))
	{
		int vitesseX = (m_vitesse.x + m_ennemi->getVitesse().x) / 2;
		m_vitesse.x = vitesseX;
		m_ennemi->changerVitesse(Vector2f(vitesseX, m_ennemi->getVitesse().y));
	}

	m_sprite.move(m_vitesse/fps);

	m_hitboxCourante = attribuerHitBox(m_hitbox[m_anim], m_sprite, m_direction);

	//Effet de glissement, inertie en l'air, tout ça.
	if (!m_air)
	{
		//on diminue la vitesse, frottements du sol.
		if (!m_marche)
			m_vitesse.x *= 0.8; //85
	}
	else
		m_vitesse.x *= 0.99; //98

	//PARTICULES

	if (!m_air)
		m_feu = false;

	fumee.update(fps);
	feu.update(fps);

	//PLACE A LA BASTON !

	//Si on a tapé, on va regarder si on a touché.
	if (m_coup)
	{
		for (unsigned int a = 0; a < getHitBox().size(); a++)
		{
			//On parcourt toutes les hitbox courantes du héros, si on en rencontre une offensive, on tape.
			HitBox coupHitbox = getHitBox()[a];
			if (coupHitbox.getType() == OFFENSIF)
			{
				for(unsigned int i = 0; i < m_ennemi->getHitBox().size(); i++)
				{
					if ((coupHitbox.hitBox().intersects(m_ennemi->getHitBox()[i].hitBox())) && (m_ennemi->getHitBox()[i].getType() != OFFENSIF))
					{
						m_ennemi->recevoirCoup(m_coups[m_coupID][m_combo], m_vitesse.x, m_direction);
					}
				}
			}
		}
	}
}

//Cette méthode est à appeller après update(), et permet de remettre à 0 la marche, et l'accroupissement,
//pour que le héros arrête de courir quand on n'appuie plus sur la touche.
void Heros::init()
{
	m_marche = false;
	//on met marche à false pour que le Héros s'arrête de marcher quand on ne presse plus sur la touche.
	m_courir = false;

	if(!((m_accroupi) && (m_coup)))
		m_accroupi = false;
}

void Heros::debug(bool debug)
{
	m_debug = debug;
}

void Heros::afficher(RenderWindow *fenetre)
{
	fenetre->draw(fumee);
	fenetre->draw(feu);
	fenetre->draw(m_sprite);

	//Pour le débuggage, on affiche les hitbox
	if (m_debug)
	{
		for(unsigned int i = 0; i < m_hitboxCourante.size(); i++)
		{
			m_hitboxCourante[i].afficher(fenetre);
		}

		if (m_KO != seconds(0))
		{
			RectangleShape KO;
			KO.setPosition(Vector2f(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top));
			KO.setSize(Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
			KO.setFillColor(Color(255, 100, 0, 100));
			fenetre->draw(KO);
		}
		if (m_recover != seconds(0))
		{
			RectangleShape recover;
			recover.setPosition(Vector2f(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top));
			recover.setSize(Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
			recover.setFillColor(Color(50, 255, 0, 100));
			fenetre->draw(recover);
		}
	}
}
