#ifndef FLOOR_H
#define FLOOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include "isomtric.h"

/* Cada tile_t representa un mosaico. El index es el
   número de mosaico en el archivo png que posee todos
	los posibles mosaicos para el piso */

typedef struct {
   uint8_t z;
   uint8_t index;    // numero de tile dentro del png
} tile_t;

typedef struct {
	tile_t **mosaic;
	SDL_Texture texture;
	SDL_Renderer *renderer;
	uint8_t col;		//cantidad de columnas en el png
	SDL_rect cut;		//corte del archivo png
} floor_t;

void floor_init(	floor_t *f, SDL_Renderer *r, uint8_t col,
						int width, int height);
void floor_draw(floor_t *f);
void floor_load_texture(floor_t *f, char *fileName);
void floor_load_mosaic(floor_t *f, char *fileName);

#endif
