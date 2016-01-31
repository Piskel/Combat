#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "fonctions.h"
#include "Heros.h"
#include "HitBox.h"
#include "Coup.h"

using namespace sf;

//ATTENTION : CE FICHIER N'EST PAS BIEN CODÉ. PENSER À OPTIMISER TOUT ÇA.
// (Mais comme ça fonctionne, ça a des chances de rester comme ça pas mal de temps.)

//Cette fonction permet de charger les stats générales d'un personnage depuis un fichier.
void chargerStats(Heros *perso, std::string nomPerso)
{
	std::string nomFichier = "ressources/stats/" + nomPerso;

	std::string str = ""; ///string poubelle (tempo)
	std::ifstream flux(nomFichier);

	Vector2i tailleSprite;
	Vector2f origineSprite;
	int vitesse;
	int framesSurPlace, framesMarche, demiTour;
	float defense;

	if (flux)
	{
		while (str != "DEBUT")
			flux >> str;

		while (str != "FIN_FICHIER")
		{
			flux >> str;
			if (str == "TAILLE_SPRITE")
			{
				flux >> str;
				if (str == "=")
				{
					flux >> tailleSprite.x;
					flux >> tailleSprite.y;
					perso->setTailleSprite(tailleSprite);
				}
				else
					std::cout << "Erreur : le symbole '=' est attendu après TAILLE_SPRITE, au lieu de '" + str + "'.\n";
			}
			if (str == "ORIGINE_SPRITE")
			{
				flux >> str;
				if (str == "=")
				{
					flux >> origineSprite.x;
					flux >> origineSprite.y;
					perso->setOrigineSprite(origineSprite);
				}
				else
					std::cout << "Erreur : le symbole '=' est attendu après ORIGINE_SPRITE, au lieu de '" + str + "'.\n";
			}
			if (str == "VITESSE")
			{
				flux >> str;
				if (str == "=")
				{
					flux >> vitesse;
					perso->setVitesse(vitesse);
				}
				else
					std::cout << "Erreur : le symbole '=' est attendu après VITESSE, au lieu de '" + str + "'.\n";
			}
			if (str == "DEFENSE")
			{
				flux >> str;
				if (str == "=")
				{
					flux >> defense;
					perso->setDefense(defense);
				}
				else
					std::cout << "Erreur : le symbole '=' est attendu après DEFENSE, au lieu de '" + str + "'.\n";
			}
			if (str == "SURPLACE")
			{
				flux >> str;
				if (str == "=")
				{
					flux >> framesSurPlace;
				}
				else
					std::cout << "Erreur : le symbole '=' est attendu après SURPLACE, au lieu de '" + str + "'.\n";
			}
			if (str == "MARCHE")
			{
				flux >> str;
				if (str == "=")
				{
					flux >> framesMarche;
				}
				else
					std::cout << "Erreur : le symbole '=' est attendu après MARCHE, au lieu de '" + str + "'.\n";
			}
			if (str == "DEMI-TOUR")
			{
				flux >> str;
				if (str == "=")
				{
					flux >> demiTour;
					perso->setTempsDemiTour(milliseconds(demiTour));
				}
				else
					std::cout << "Erreur : le symbole '=' est attendu après DEMI-TOUR, au lieu de '" + str + "'.\n";
			}
		}
		//Cettefonction permet de gérer les différences entre les textures des persos : le nombre de framepour la marche...
		perso->chargerInfosAnim(framesSurPlace, framesMarche);

		std::cout << "Fichier '" << nomFichier <<"' chargé !\n";
	}
	else
	{
		std::cout << "Erreur : Impossible d'ouvrir " << nomFichier << " en lecture !\n";
	}
}
/*
 * Cette fonction charge les coups d'un perso depuis un fichier.
 *
 * Les durées sont exprimées en millisecondes.
 *
 * Organisation du fichier texte :
 * 
 * DEBUT
 *
 * COUP 0 :
 *
 *	COMBO 0 :
 *
 *		COOLDOWN = 120
 *		KO = 200
 *		DEGATS = 20
 *		FORCE_X = 60
 *		FORCE_Y = -50
 *
 *		FRAME : ANIM = 5 ; DUREE = 100
 *		FRAME : ANIM = 6 ; DUREE = 120
 *
 *	FIN_COMBO
 *
 * FIN_COUP
 *
 * FIN_FICHIER
 *
 *
 * À noter : COUP 0 = Coup Normal
 *			 COUP 1 = Coup Bas
 */

std::vector<std::vector<Coup> > coupsDepuisUnFichier(std::string nomFichier)
{
	std::vector<std::vector<Coup> > coups;

	unsigned int coup, combo;
	//Pour les frames :
	int anim;
	Time dureeAnim;

	int temps;//varaible tempo pour passer de str à Time

	std::string str = ""; ///string poubelle (tempo)
	std::ifstream flux(nomFichier);

	if (flux)
	{
		//On ne lit pas ce qu'il y a avant début
		while (str != "DEBUT")
			flux >> str;
		while (str != "FIN_FICHIER")
		{
			while ((str != "COUP") && (str != "FIN_FICHIER"))
				flux >> str;

			if (str == "COUP")
			{
				flux >> coup;

				while (coup >= coups.size())
					coups.push_back(std::vector<Coup>()); //On adapte la taille du tableau au N° du coup

				flux >> str;

				if (str != ":")
					std::cout << "Attention : écrire ':' au lieu de '" << str << "'.\n";

				while (str != "FIN_COUP")
				{
					flux >> str;

					while ((str != "COMBO") && (str != "FIN_COUP"))
					{
						flux >> str;
					}

					if (str == "COMBO")
					{
						flux >> combo;

						while (combo >= coups[coup].size())
							coups[coup].push_back(Coup()); //On adapte la taille du tableau au N° du coup

						flux >> str;
						if (str != ":")
							std::cout << "Attention : écrire ':' au lieu de '" << str << "'.\n";

						while (str != "FIN_COMBO")
						{
							flux >> str;

							if (str == "COOLDOWN")
							{
								flux >> str;
								if (str == "=")
								{
									flux >> temps;
									coups[coup][combo].cooldown = milliseconds(temps);
								}
								else
									std::cout << "Erreur : le caractère '=' est attendu après COOLDOWN, au lieu de '" << str << "'.\n";
							}
							else if (str == "KO")
							{
								flux >> str;
								if (str == "=")
								{
									flux >> temps;
									coups[coup][combo].KO = milliseconds(temps);
								}
								else
									std::cout << "Erreur : le caractère '=' est attendu après KO, au lieu de '" << str << "'.\n";
							}
							else if (str == "DEGATS")
							{
								flux >> str;
								if (str == "=")
								{
									flux >> coups[coup][combo].degats;
								}
								else
									std::cout << "Erreur : le caractère '=' est attendu après DEGATS, au lieu de '" << str << "'.\n";
							}
							else if (str == "FORCE_X")
							{
								flux >> str;
								if (str == "=")
								{
									flux >> coups[coup][combo].force.x;
								}
								else
									std::cout << "Erreur : le caractère '=' est attendu après FORCE_X, au lieu de '" << str << "'.\n";
							}
							else if (str == "FORCE_Y")
							{
								flux >> str;
								if (str == "=")
								{
									flux >> coups[coup][combo].force.y;
								}
								else
									std::cout << "Erreur : le caractère '=' est attendu après FORCE_Y, au lieu de '" << str << "'.\n";
							}
							//La gestion des erreurs est laborieuse et un peu sale, mais ça aide à ne pas se tromper dans l'écriture du fichier.
							else if (str == "FRAME")
							{
								flux >> str;
								if (str == ":")
								{
									flux >> str;
									if (str == "ANIM")
									{
										flux >> str;
										if (str == "=")
										{
											flux >> anim;
											flux >> str;
											if (str == ";")
											{
												flux >> str;
												if (str == "DUREE")
												{
													flux >> str;
													if (str == "=")
													{
														flux >> temps;
														dureeAnim = milliseconds(temps);

														coups[coup][combo].ajouterFrame(anim, dureeAnim);
													}
													else
														std::cout << "Erreur : le caractère '=' est attendu après DUREE, au lieu de '" << str << "'.\n";
												}
												else
													std::cout << "Erreur : la commande 'DUREE' est attendue après 'FRAME : ANIM = <valeur> ;'.\n";
											}
											else
												std::cout << "Erreur : le caractère ';' est attendu après 'ANIM = <valeur>', au lieu de '" << str << "'.\n";
										}
										else
											std::cout << "Erreur : le caractère '=' est attendu après ANIM, au lieu de '" << str << "'.\n";
									}
									else
										std::cout << "Erreur : la commande 'ANIM' est attendue après 'FRAME :'.\n";
								}
								else
									std::cout << "Erreur : le caractère ':' est attendu après FRAME, au lieu de '" << str << "'.\n";
							}
							else if (str != "FIN_COMBO")
								std::cout << str << " : commande inconnue ou inattendue.\n";
						}
					}
				}
			}
		}

		std::cout << "Fichier '" << nomFichier <<"' chargé !\n";
	}
	else
	{
		std::cout << "Erreur : Impossible d'ouvrir " << nomFichier << " en lecture !\n";
	}

	return coups;
}

/************************************************************************
 * Sert à charger toutes les hitbox d'un perso depuis un fichier texte.	*
 *																		*
 * Le fichier texte sera organisé comme ceci :							*
 *																		*
 * DEBUT																*
 *																		*
 * FRAME X :															*
 * <Type> <posX> <posY> <tailleX> <tailleY>								*
 * <Type> <posX> <posY> <tailleX> <tailleY>								*
 * FIN_FRAME															*
 *																		*
 * FRAME (X + 1) :														*
 * <Type> <posX> <posY> <tailleX> <tailleY>								*
 * <Type> <posX> <posY> <tailleX> <tailleY>								*
 * FIN_FRAME															*
 *																		*
 * FIN_FICHIER															*
 *																		*
 *																		*
 * <Type>																*
 * 0 : CORPS, 1 : TETE, 2 : JAMBES, 3 : OFFENSIF;						*
 *																		*
 *TOUJOURS mettre les hitbox de type offensif en premier.				*
 *																		*
 ************************************************************************/
std::vector<std::vector<HitBox> > HitBoxDepuisUnFichier(std::string nomFichier)
{
	std::vector<std::vector<HitBox> > hitbox;
	std::vector<HitBox> hitboxFrame;

	std::string str = ""; ///string poubelle (tempo)
	unsigned int frame = 0; //indique la frame qu'on lit.
	int type, posX, posY, tailleX, tailleY;

	std::ifstream flux(nomFichier);

	if (flux)
	{
		//On ne lit pas ce qu'il y a avant début
		while (str != "DEBUT")
			flux >> str;
		while (str != "FIN_FICHIER")
		{
			while ((str != "FRAME") && (str != "FIN_FICHIER"))
			{
				flux >> str;
			}

			if (str == "FRAME")
			{
				flux >> frame;
				flux >> str; //Les ':' sont inutiles.
				if (str != ":")
					std::cout << "Attention : écrire ':' au lieu de '" << str << "'.\n";

				//On agrandit le tableau si la frame n'est pas dedans.
				while (hitbox.size() < frame + 1)
				{
					hitbox.push_back(std::vector<HitBox>());
				}

				hitboxFrame.clear();

				while (str != "FIN_FRAME")
				{
					flux >> str;
					if (str == "HITBOX")
					{
						flux >> type;
						flux >> posX;
						flux >> posY;
						flux >> tailleX;
						flux >> tailleY;
						hitboxFrame.push_back(HitBox(FloatRect(posX, posY, tailleX, tailleY), type));
					}
				}

				hitbox[frame] = hitboxFrame;


			}
			else if (str != "FIN_FICHIER")
			{
				std::cout << "Erreur : fichier mal écrit !\n";
			}
		}
		std::cout << "Fichier '" << nomFichier << "' chargé !\n";
	}
	else
	{
		std::cout << "Erreur : Impossible d'ouvrir " << nomFichier << " en lecture !\n";
	}

	return hitbox;
}