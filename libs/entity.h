#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "point.h"
#include "vector.h"
//#include "border.h"

typedef struct {
	/* Posicion dentro del tile coord isometrica x
		en el tile desde el vertice de la base*/
	uint16_t x;
	uint16_t y;

	/* Coordenada x sobre la base de la imagen */
	/* La coordenada y no es necesaria */
	int x_base;

	vector_t *direction;		/* Sentido de desplazamiento */
	bool debug;
	SDL_Texture *texture;	/* Textura de donde tomar la imagen */
	SDL_Rect rect;				/* Para recortar la textura */
	SDL_Renderer *renderer;
	int tile_width;
	int tile_height;
								
} entity_t;

void entity_create(entity_t **e, SDL_Renderer *r, int tile_width, int tile_height);
void entity_draw(entity_t *e, SDL_Rect *tile);
void entity_add_texture(entity_t *e, SDL_Texture *texture, int w,
								int h, int x, int y, int coord_x, int coord_y,
								int x_base);
void entity_free(entity_t *e);

#endif
