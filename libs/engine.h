#ifndef FLOOR_H
#define FLOOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include "isometric.h"

#define TILE_WIDTH	64		//Debe ser par
#define TILE_HEIGHT	32		//Debe ser par

/* Cada tile_t representa un mosaico. El index es el
   número de mosaico en el archivo png que posee todos
	los posibles mosaicos para el piso */

typedef struct {
   uint8_t z;
   uint8_t index;    // numero de tile dentro del png
} tile_t;

typedef struct {
	tile_t *mosaic;
	uint64_t rows;		//Tamano fisico filas de mosaic
	uint64_t cols;		//tamaño fisico columnas de mosaic
	SDL_Texture *texture;
	SDL_Renderer *renderer;
	SDL_Rect screen;
	uint8_t col;		//cantidad de columnas en el png
	SDL_Rect cut;		//corte del archivo png
} engine_t;

void engine_init(	engine_t *f, SDL_Renderer *r);
void engine_draw(engine_t *f);
void engine_set_tileset(engine_t *f, char *fileName, int columns, int height, int width);
void engine_load_mosaic(engine_t *f, char *fileName);
uint32_t engine_mosaic_rows(engine_t *f);
uint32_t engine_mosaic_cols(engine_t *f);
void engine_free(engine_t *f);
#endif
