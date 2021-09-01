#!/bin/bash

mkdir -p test/build
cd test/build
cmake ..
make
# a la version 3.21 de cmake on peut ajouter le parametre --output-junit pour que le resultat des tests soit en format jenkins
# pour le moment on va pas executer les tests 2 fois ;)
# ctest --output-junit
