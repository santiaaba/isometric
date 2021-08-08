#ifndef TYPES_H
#define TYPES_H

typedef struct {
	int row;
	int col;
} m_index_t;

typedef struct entity entity_t;
typedef struct anchor anchor_t;
typedef struct anchor {
   entity_t *entity;
   anchor_t *next;
   anchor_t *prio;
   anchor_t *neighbor;
} anchor_t;

typedef struct entities entities_t;
typedef struct entities {
	entity_t *entity;
	entities_t *next;
} entities_t;

typedef struct tile{
	uint8_t z;
	uint8_t index;					// numero de tile dentro del png
	//lista_t *entities;			// Entidades ancladas al tile
	anchor_t *entities;
} tile_t;

typedef struct {
	tile_t *mosaic;
	uint32_t mosaic_rows;		//Tamano fisico filas de mosaic
	uint32_t mosaic_cols;		//tamaño fisico columnas de mosaic
	int tile_width;				// Anchura de los tile para el suelo
	int tile_height;			  // Altura de los tile para el suelo
	SDL_Texture *tileset;
	SDL_Renderer *renderer;
	SDL_Rect screen;
	bool show_screen_rect;
	SDL_Rect playground;
	uint8_t tileset_cols;		//cantidad de columnas en el png
	SDL_Rect tileset_cut;		//corte del archivo png
	bool debug;
	entities_t *entities;
	
} engine_t;

typedef struct entity {
	/* Posicion dentro del tile coord isometrica x
		en el tile desde el vertice de la base.
		Son valores negativos*/
	//int ix;						/* Coordenadas x isometrica absoluta */
	//int iy;						/* Coordenada y isometrica absoluta */
	int ixx;						/* Coordenada isometrica x relativa dentro del tile. En negativo */
	int iyy;						/* Coordenada isometrica y relativa dentro del tile. En negativo */
	engine_t *engine;
	int x_base;					/* Coordenada x de la textura mas inferior */
	SDL_Texture *texture;	/* Textura de donde tomar la imagen */
	SDL_Rect cut;			 	/* Para recortar la textura */
	int original_x;			/* Ya que lo modificamos al cortar los tochos */
	int original_w;			/* Ya que lo modificamos al cortar los tochos */
	SDL_Rect *borde;			/* Borde para deteccion de coliciones */
	anchor_t *anchor;			/* Anclaje principal al tile donde inicia la imagen */
} entity_t;
#endif
