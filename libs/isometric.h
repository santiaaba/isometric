#ifndef ISOMETRIC_H
#define ISOMETRIC_H

#include "point.h"

#define TILE_WIDTH   64    //Debe ser par
#define TILE_HEIGHT  32    //Debe ser par


/* Convierte una coordenada isometrica a cartesiana */
void iso_to_cart(int iso_x, int iso_y, int *cart_x, int *cart_y);

/* Convierte una coordenada cartesiana a isometrica */
void cart_to_iso(int cart_x, int cart_y, int *iso_x, int *iso_y);

#endif
