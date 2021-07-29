#include "isometric.h"

void iso_to_cart(int iso_x, int iso_y, int *cart_x, int *cart_y){
	*cart_x = (2 * iso_y + iso_x)/2;
	*cart_y = (2 * iso_y - iso_x)/2;
}

void cart_to_iso(int cart_x, int cart_y, int *iso_x, int *iso_y){
	*iso_x = cart_x - cart_y;
	*iso_y = cart_x + cart_y/2;
}
