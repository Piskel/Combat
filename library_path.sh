#!/bin/bash

#Sert à indiquer que les bibliothèques sont dans le répertoire courant.
echo export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH >> ~/.profile

#On met les droits d'éxecution aussi, au cas où.
chmod u+x Combat
