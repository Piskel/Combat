#!/bin/bash

DEBUT=$(date +'%s')

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
			echo "[-nw] : Les avertissements seront ignorés pendant cette compilation."
			noWarnings="TRUE"
		elif [ $i = "-r" ]
		then
			echo "[-r] : Le projet sera entièrement recompilé."
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

if [ ! -e src ]
	then echo "Le répertoire src/ n'existe pas."
	exit 2
fi

fichiers=$(echo "src/*.cpp")

echec_compilation="FALSE"

for i in src/*.cpp
do
	nomFichier=${i:4} #on enlève le "src/" du nom du fichier

	l=$(expr length $nomFichier)
	l=$(expr $l - 4)
	nomFichier=${nomFichier:0:$l} #on enlève l'extension du fichier

	#maintenant, nomFichier vaut le nom du fichier... Par exemple, src/Heros.cpp devient Heros

	if [ "$nomFichier" = "*" ]
	then 
		echo "Aucun fichier .cpp sous le répertoire src/"
		exit 2
	fi

	echo -e "\033[01;32m\nCompilation de $i...\033[00;00m"

	compiler="TRUE"

	if [ -e compil/${nomFichier}.date ]
		then date=$(cat compil/${nomFichier}.date)
	else date="0"
	fi

	#On vérifie que le fichier ne soit pas déjà compilé
	if [ "$date" = $(stat -c%Y src/${nomFichier}.cpp) ]
	then
		echo -e "Le fichier src/${nomFichier}.cpp a déjà été compilé."

		if [ $compilerTout = "TRUE" ]
			then compiler="TRUE"
			else compiler="FALSE"
		fi
	fi

	if [ $compiler = "TRUE" ]
	then
		echec="TRUE"
		if [ $noWarnings = "FALSE" ]
			then g++ -std=c++11 -I include/ -Wall -c $i -o compil/${nomFichier}.o && echec="FALSE"
			else g++ -std=c++11 -I include/ -w -c $i -o compil/${nomFichier}.o && echec="FALSE"
		fi

		if [ $echec = "FALSE" ]
		then
			echo -e "$i -> compil/${nomFichier}.o"
			#On sauvegarde la date de la dernière modif'
			stat -c%Y src/${nomFichier}.cpp > compil/${nomFichier}.date
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
	#Linkage
	echo -e "\n"

	#On recherche les .so dans le répertoire courant
	if [ -z "$(find . -maxdepth 1 -name "*.so" -print)" ]
	then
		# Pas de bibliothèques
	 	echo "Aucune bibliothèque détectée."
	 	BIBLIOTHEQUES=""
	else
		BIBLIOTHEQUES=$(find . -maxdepth 1 -name "*.so" -print)
	 	echo "Bibliothèques détectées et utilisées pour la compilation :"
	 	find . -maxdepth 1 -name "*.so" -print
	fi

	echo -e "\nÉdition de liens et création de l'exécutable..."
	ECHEC="TRUE"
	g++ -fPIC compil/*.o $BIBLIOTHEQUES -o Programme && ECHEC="FALSE"

	if [ $ECHEC = "TRUE" ]
	then
		echo -e "\nÉchec de l'édition de liens !"
	else
		FIN=$(date +'%s')
		TEMPS=$(($FIN - $DEBUT))
		echo -e -n "Terminé ! (${TEMPS}s)\nVoulez-vous lancer l'exécutable ? [o/n] : "
		read choix
		if [ $choix = "o" -o $choix = "O" ]
		then
		echo ""
			./Programme
		fi
	fi
fi