#ifndef ISOMETRIC_H
#define ISOMETRIC_H

#include "point.h"

void iso_to_cart(point_t *iso, point_t *cart);
void cart_to_iso(point_t *cart, point_t *iso);

#endif
