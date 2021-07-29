#!/bin/bash

cd libs
rm *.o
gcc -c point.c
gcc -c vector.c
gcc -c engine.c
gcc -c rect.c
gcc -c entity.c
gcc -c lista.c
cd ..
gcc demo1.c libs/lista.o libs/rect.o libs/entity.o libs/vector.o libs/engine.o libs/point.o -lm -lSDL2_image -lSDL2main -lSDL2 -o demo1

