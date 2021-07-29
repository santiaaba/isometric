#include "entity.h"

void entity_create(entity_t **e, SDL_Renderer *r){
	*e = (entity_t*)malloc(sizeof(entity_t));
	(*e)->renderer = r;
	(*e)->debug = false;
	(*e)->texture = NULL;
}

void entity_draw(entity_t *e, SDL_Rect *dest_tile){
	SDL_Rect dest;
	int x_iso;
	int y_iso;
	int x_cart;
	int y_cart;

	/* Arrancamos desde el vertice inferior del tile isometrico. Que viene
		siendo el centro de la linea orizonal del rectangulo dest_tile*/
	x_iso = dest_tile->x + TILE_WIDTH/2;
	y_iso = dest_tile->y + TILE_HEIGHT;

	/* Debemos calcular las coordenadas del dest que dibuja el entity */
	dest.x = x_iso - e->x_base - e->y;
	dest.y = y_iso - e->rect.h - e->x;
	dest.w = e->rect.w;
	dest.h = e->rect.h;

	SDL_RenderCopy(e->renderer,e->texture,&(e->rect),&dest);
}

void entity_add_texture(entity_t *e, SDL_Texture *texture, int w, int h,
								int x, int y, int coord_x, int coord_y, int x_base){
	e->texture = texture;
	e->rect.w = w;
	e->rect.h = h;
	e->rect.x = x;
	e->rect.y = y;
	e->x = coord_x;	// Coordenadas isometricas relativas al tile
	e->y = coord_y;	// Coordenadas isometricas relativas al tile
	e->x_base = x_base;
}

void entity_free(entity_t *e){
	printf("Implementar!!\n");
}
