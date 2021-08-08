#include "entity.h"

void entity_init(entity_t *e){
}
void entity_create(entity_t **e, engine_t *engine){
	*e = (entity_t*)malloc(sizeof(entity_t));
	(*e)->texture = NULL;
	(*e)->engine = engine;
	(*e)->anchor = NULL;
	engine_add_entity(engine,(*e));
}

void entity_draw(entity_t *e, SDL_Rect *dest_tile){
	SDL_Rect dest;
	SDL_Rect cut;
	int x1,x2,y1,y2;

	cut.x = e->cut.x;
	cut.y = e->cut.y;
	cut.w = e->cut.w;
	cut.h = e->cut.h;

	/* Nos paramos en el vertice (0,0) del tile */
	dest.x = dest_tile->x + e->engine->tile_width/2;
	dest.y = dest_tile->y;

	/* Hacemos que el vertice de x_base coincida con el vertice (0,0)*/
	dest.x -= e->x_base;
	dest.y -= cut.h;

	/* Movemos el vertie x_base al punto dentro del tile indicado por ixx, iyy */
	engine_iso_move(&dest,e->ixx,e->iyy);

	dest.w = cut.w;
	dest.h = cut.h;

   /* Recorte superior */
	if(e->engine->screen.y > dest.y){
		dest.h = dest.h - (e->engine->screen.y - dest.y);
		cut.y += (e->engine->screen.y - dest.y);
		dest.y = e->engine->screen.y;
   }

   /* Recorte izquierdo */
	if(e->engine->screen.x > dest.x){
		dest.w = dest.w - (e->engine->screen.x - dest.x);
		cut.x += (e->engine->screen.x - dest.x);
		dest.x = e->engine->screen.x;
   }
   /* Recorte inferior */
	if((e->engine->screen.y + e->engine->screen.h) < (dest.y + dest.h)){
		dest.h -= (dest.y + dest.h) - (e->engine->screen.y + e->engine->screen.h);
   }
   /* Recorte derecho */
	if((e->engine->screen.x + e->engine->screen.w) < (dest.x + dest.w))
		dest.w = -dest.x + e->engine->screen.x + e->engine->screen.w;

   cut.h = dest.h;
   cut.w = dest.w;

	SDL_RenderCopy(e->engine->renderer,e->texture,&cut,&dest);

	/* Restauramos los valores de corte cut.w y cuy.h ya que pueden
	   haber sido modificado por los recortes */
	if(e->engine->debug){
		x1 = dest.x;
		x2 = dest.x + dest.w;
		y1 = dest.y;
		y2 = dest.y + dest.h;
		SDL_SetRenderDrawColor(e->engine->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(e->engine->renderer,x1,y1,x2,y1);
		SDL_RenderDrawLine(e->engine->renderer,x1,y2,x2,y2);
		SDL_RenderDrawLine(e->engine->renderer,x1,y1,x1,y2);
		SDL_RenderDrawLine(e->engine->renderer,x2,y1,x2,y2);

		if(e->borde != NULL){
			SDL_SetRenderDrawColor(e->engine->renderer, 0, 255, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawLine(e->engine->renderer,e->borde->x,e->borde->y,e->borde->x,e->borde->y + e->borde->h);
      	SDL_RenderDrawLine(e->engine->renderer,e->borde->x,e->borde->y + e->borde->h,e->borde->x + e->borde->w,e->borde->y + e->borde->h);
      	SDL_RenderDrawLine(e->engine->renderer,e->borde->x + e->borde->w,e->borde->y + e->borde->h,e->borde->x + e->borde->w,e->borde->y);
      	SDL_RenderDrawLine(e->engine->renderer,e->borde->x + e->borde->w,e->borde->y,e->borde->x,e->borde->y);
		}
	}
}

void entity_cut(entity_t *e, int row, int col){
	/* Divide la entidad para que sea dibujada a atrozos en su parte
		derecha en el tile que corresponde */
	int remaining_width,cut_x,cut_w,cut_h;
	int tile_height;
	tile_t *tile;
	anchor_t *neighbor, *parent;
	entity_t *cut_entity;
	int x_aux;
	
//	printf("Tile en coordenadas(%i,%i)\n",row,col);
	remaining_width = e->cut.w - (e->x_base + e->iyy);

	if(remaining_width > 0){
		tile_height = engine_tile_height(e->engine);
		/* Acomodamos el entity actual */
		e->cut.w -= remaining_width;
		cut_x = e->cut.x + e->cut.w;
		cut_h = e->cut.h - e->iyy/2;
		parent = e->anchor;
		while(remaining_width > 0){
			if(row - 1 < 0)
				break;
			else {
				row--;

				/* Creamos un entity especial */
				cut_entity = (entity_t*)malloc(sizeof(entity_t));
				//entity_create(&cut_entity,e->engine);
				cut_entity->anchor = NULL;
				cut_entity->texture = e->texture;
				cut_entity->engine = e->engine;
				cut_entity->cut.y = e->cut.y;
				cut_entity->cut.x = cut_x;
				cut_entity->cut.h = cut_h;

				if(remaining_width >= tile_height)
					cut_w = tile_height;
				else
					cut_w = remaining_width;

				cut_entity->cut.w = cut_w;
				cut_entity->ixx = e->ixx;
				cut_entity->iyy = tile_height;
				/* Los atributos ix e iy no importan */
				cut_entity->x_base = 0;
				/* Los cut_entity no poseen borde */
				cut_entity->borde = NULL;

				tile = engine_tile(e->engine,row,col);
			//	printf("Anclando la nueva pieza: %p\n",tile->entities);
				neighbor = anchor_add(tile->entities,cut_entity);
				/* Enganchamos con el entity original */
			//	printf("Anclando la nueva pieza 2\n");
				anchor_anchor(parent,neighbor);
				parent = neighbor;
			//	printf("Anclando la nueva pieza 3\n");
			
			//	engine_place_entity(e->engine,row,col,cut_entity);
	
				remaining_width -= cut_w;
				cut_x += cut_w;
				cut_h -= tile_height/2;
			}
		}
	}
}

int entity_ixx(entity_t *e){
	return e->ixx;
}
int entity_iyy(entity_t *e){
	return e->iyy;
}

void entity_position_set(entity_t *e, int ix, int iy){
	int row,col;
	tile_t *tile;
	anchor_t *anchor;

	/* Si la entidad esta cambiando de coordenadas, borramos
		todos sus anchor que forman las partes y los generamos
		nuevamente */
	if((e->borde->x != ix || e->borde->y != iy) && e->anchor != NULL){
		anchor_delete(e->anchor);
	}
	e->borde->x = ix - e->borde->w;
	e->borde->y = iy - e->borde->h;
//	printf("Coordenadas (%i,%i)\n",e->ix,e->iy);

	/* Regresamos las dimenciones del SDL_Rect cut a la normalidad */
	e->cut.w = e->original_w;

	/* Calculamos la col y row del tile y las coordenadas dentro del mismo */
	engine_iso_tile(e->engine,ix,iy,&row,&col,&(e->ixx),&(e->iyy));
	
	/* Posicionamos el entity en el tile*/
	tile = engine_tile(e->engine,row,col);
	e->anchor = anchor_add(tile->entities,e);

	/* Lo trozamos */
	entity_cut(e,row,col);
}

void entity_position_add(entity_t *e, int addix, int addiy){
	int row;
	int col;
	tile_t *tile;
	anchor_t *anchor;
	entities_t *c;

	if(e->anchor != NULL)
		anchor_delete(e->anchor);
	e->borde->x += addix;
	e->borde->y += addiy;
	/* Detectamos colisiones */
	c = NULL;
	engine_collisions(e->engine,e,&c);
	/* La primera aproximacion es que si hubieron
		colisiones, entonces regresamos a las coordenadas
		anteriores */
	if(c != NULL){
		e->borde->x -= addix;
		e->borde->y -= addiy;
		entities_empty(&c);
	}

	/* Regresamos las dimenciones del SDL_Rect cut a la normalidad */
	e->cut.w = e->original_w;
	/* Calculamos la col y row del tile y las coordenadas dentro del mismo */
	engine_iso_tile(e->engine,e->borde->x + e->borde->w,e->borde->y + e->borde->h,&row,&col,&(e->ixx),&(e->iyy));
	/* Posicionamos el entity en el tile*/
	tile = engine_tile(e->engine,row,col);
	e->anchor = anchor_add(tile->entities,e);
	/* Lo trozamos */
	entity_cut(e,row,col);
}

void entity_add_texture(entity_t *e, SDL_Texture *texture, int cut_w, int cut_h,
								int cut_x, int cut_y, int x_base, int borde_w, int borde_h){
	e->texture = texture;
	e->x_base = x_base;
	e->cut.x = cut_x;
	e->cut.y = cut_y;
	e->cut.w = cut_w;
	e->cut.h = cut_h;

	e->original_x = cut_x;
	e->original_w = cut_w;

	e->borde = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	e->borde->w = borde_w;
	e->borde->h = borde_h;
	e->borde->x = 0;	/* Se actualiza con el posicionamiento */
	e->borde->y = 0;	/* Se actualiza con el posicionamiento */
}

bool entity_collision(entity_t *e1, entity_t *e2) {
	/* Determina si e1 colisiona con e2 */
	printf("Buscando colision %p - %p\n",e1,e2);
	if (e1->borde == NULL || e2->borde == NULL)
		return false;
	printf("Entity - comparando:\n");
	printf("Entity e1:\n");
	printf("(x,y,w,h) = (%i,%i,%i,%i)\n",e1->borde->x,e1->borde->y,e1->borde->w,e1->borde->h);
	printf("Entity e2:\n");
	printf("(x,y,w,h) = (%i,%i,%i,%i)\n",e2->borde->x,e2->borde->y,e2->borde->w,e2->borde->h);
	return ((e2->borde->x < e1->borde->x + e1->borde->w &&
   e2->borde->x + e2->borde->w > e1->borde->x + e1->borde->w)
	||
	(e2->borde->x < e1->borde->x &&
	e2->borde->x + e2->borde->w > e1->borde->x))
	&&
	((e2->borde->y + e2->borde->h > e1->borde->y &&
	e2->borde->y < e1->borde->y)
	||
   (e2->borde->y < e1->borde->y + e1->borde->h &&
	e2->borde->y + e2->borde->h > e1->borde->y + e1->borde->h));
}

void entity_free(entity_t **e){
	//printf("Eliminando!!!\n");
/*
	if((*e)->anchor != NULL)
		free((*e)->anchor);
*/
	if((*e)->borde != NULL)
		free((*e)->borde);
	free(*e);
}
