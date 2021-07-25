#include "isometric.h"

void iso_to_cart(point_t *iso, point_t *cart){
	point_set(	cart,
					(2 * point_y(iso) + point_x(iso))/2,
					(2 * point_y(iso) - point_x(iso))/2);
}

void cart_to_iso(point_t *cart, point_t *iso){
	point_set(	iso,
					point_x(cart) - point_y(cart),
					(point_x(cart) + point_y(cart)) / 2);
}
