/* Motor grafico isometrico */

#ifndef ISO_H
#define ISO_H

#include "floor.h";

typedef struct{
	floor_t *floor;
	SDL_Renderer *renderer;
	SDL_Windows *windows;
} iso_t;

void iso_init(iso_t *i);
void iso_draw(iso_t *i);
floor_t *iso_floor(iso_t *i);
void iso_free(iso_t *i);

#endif
