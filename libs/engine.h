#ifndef FLOOR_H
#define FLOOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdbool.h>
#include "isometric.h"
#include "rect.h"

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
	uint32_t mosaic_rows;		//Tamano fisico filas de mosaic
	uint32_t mosaic_cols;		//tamaño fisico columnas de mosaic
	SDL_Texture *tileset;
	SDL_Renderer *renderer;
	SDL_Rect screen;
	rect_t *playground;
	uint8_t tileset_cols;		//cantidad de columnas en el png
	SDL_Rect tileset_cut;		//corte del archivo png
	bool debug;
} engine_t;

void engine_create(	engine_t **e, SDL_Renderer *r);
void engine_draw(engine_t *e);
void engine_set_tileset(engine_t *e, char *fileName, int columns, int height, int width);
void engine_set_screen(engine_t *e, int x, int y, int w, int h);
void engine_set_playground(engine_t *e, uint32_t x, uint32_t y);
void engine_load_mosaic(engine_t *e, char *fileName);
void engine_debug(engine_t *e, bool debug);
uint32_t engine_mosaic_rows(engine_t *e);
uint32_t engine_mosaic_cols(engine_t *e);
void engine_free(engine_t *e);
#endif
