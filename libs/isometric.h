#ifndef ISOMETRIC_H
#define ISOMETRIC_H

#include "point.h"

/* Convierte una coordenada isometrica a cartesiana */
void iso_to_cart(point_t *iso, point_t *cart);

/* Convierte una coordenada cartesiana a isometrica */
void cart_to_iso(point_t *cart, point_t *iso);

#endif
