#!/bin/bash

#la condition est pas top, elle empêche pas d'avoir quelque chose du genre LD_LIBRARY_PATH=coucou:.:salut:.:chose:.:
#Mais si on lance le script plusieurs fois par erreur, on n'aura pas une variable $LD_LIBRARY_PATH absolument immondedu genre .:.:.:.:.:
if test -z "$(cat ~/.bashrc | grep 'export LD_LIBRARY_PATH=.:')"
then
	#Sert à indiquer que les bibliothèques sont dans le répertoire courant.
	echo export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH >> ~/.profile
else
	echo "Le répertoire courant est déjà présent dans \$LD_LIBRARY_PATH"
fi

#On met les droits d'éxecution aussi, au cas où.
chmod u+x Combat
