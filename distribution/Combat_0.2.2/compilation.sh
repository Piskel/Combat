#!/bin/bash

#permet de recompiler le projet.

compilerTout="FALSE"
noWarnings="FALSE"

if [ $# -gt 2 ]
	then echo -e "Trop d'arguments !\nArguments possibles : [-r] ou [-nw]"
	exit 1
fi

if test $# -ne 0
then
	for i in $*
	do
		if [ $i = "-nw" ]
		then
			echo "Les avertissements seront ignorés pendant cette compilation."
			noWarnings="TRUE"
		elif [ $i = "-r" ]
		then
			echo "Le projet sera entièrement recompilé."
			compilerTout="TRUE"
		else
			echo -e "$i : Argument inconnu.\nArguments possibles : [-r] ou [-nw]"
			exit 1
		fi

	done
fi

if [ ! -e compil ]
	then mkdir compil
fi

if [ ! -e code ]
	then echo "Le répertoire code/ n'existe pas."
	exit 2
fi

fichiers=$(echo "code/*.cpp")

echec_compilation="FALSE"

for i in code/*.cpp
do
	nomFichier=${i:5} #on enlève le "code/" du nom du fichier

	l=$(expr length $nomFichier)
	l=$(expr $l - 4)
	nomFichier=${nomFichier:0:$l} #on en lève l'extension du fichier

	#maintenant, nomFichier vaut le nom du fichier... Par exemple, code/Heros.cpp devient Heros

	if [ "$nomFichier" = "*" ]
	then 
		echo "Aucun fichier .cpp sous le répertoire code/"
		exit 2
	fi

	echo -e "\033[01;32mCompilation de $i...\033[00;00m"

	compiler="TRUE"

	if [ -e compil/${nomFichier}.date ]
		then date=$(cat compil/${nomFichier}.date)
	else date="0"
	fi

	#On vérifie que le fichier ne soit pas déjà compilé
	if [ "$date" = $(stat -c%Y code/${nomFichier}.cpp) ]
	then
		echo -e "Le fichier code/${nomFichier}.cpp a déjà été compilé."

		if [ $compilerTout = "TRUE" ]
			then compiler="TRUE"
			else compiler="FALSE"
		fi
	fi

	if [ $compiler = "TRUE" ]
	then
		echec="TRUE"
		if [ $noWarnings = "FALSE" ]
			then g++ -std=c++11 -Wall -c $i -o compil/${nomFichier}.o && echec="FALSE"
			else g++ -std=c++11 -c $i -o compil/${nomFichier}.o && echec="FALSE"
		fi

		if [ $echec = "FALSE" ]
		then
			echo -e "$i -> compil/${nomFichier}.o\n"
			#On sauvegarde la date de la dernière modif'
			stat -c%Y code/${nomFichier}.cpp > compil/${nomFichier}.date
		else
			echo -e "Erreur pendant la compilation de $i !\n"
			echec_compilation="TRUE"
		fi
	fi
done

if [ $echec_compilation = "TRUE" ]
then
	echo "Il y a eu des échecs de compilation, impossible de continuer !"
else

	echo -e "\nCréation de l'éxecutable..."

	if [ -z "$(find . -maxdepth 1 -name "*.so" -print)" ]
	then
		# Pas de bibliothèques
	 	echo "Aucune bibliothèque détectée."
	 	g++ compil/*.o -o Combat-recompilation && echo "Terminé !"
	else
	 	echo "Bibliothèques détectées dans le répertoire courant."
		g++ -fPIC compil/*.o *.so -o Combat-recompilation && echo "Terminé !"
	fi
fi