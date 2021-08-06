#include "entity.h"

void entity_create(entity_t **e, engine_t *engine){
	*e = (entity_t*)malloc(sizeof(entity_t));
	(*e)->texture = NULL;
	(*e)->engine = engine;
	(*e)->anchor = NULL;
}

void entity_draw(entity_t *e, SDL_Rect *dest_tile){
	SDL_Rect dest;
	SDL_Rect cut;
	int x1,x2,y1,y2;

/*
	printf("Entity - ixx:%i, iyy:%i, x_base:%i, cut_y:%i,cut_x:%i, cut_w:%i, cut_h:%i\n",
			  e->ixx,e->iyy,e->x_base,e->cut.y,e->cut.x,e->cut.w,e->cut.h);
*/

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

				entity_create(&cut_entity,e->engine);
				cut_entity->texture = e->texture;
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
				cut_entity->x_base = 0;

				//printf("cut_x:%i, cut_h:%i, cut_w:%i\n",cut_x,cut_h,cut_w);
				tile = engine_tile(e->engine,row,col);
				neighbor = anchor_add(tile->entities,e);
				/* Enganchamos con el entity original */
				anchor_anchor(parent,neighbor);
				parent = neighbor;
			
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

	/* Si la entidad esta cambiado de coordenadas, borramos
		todos sus anchor que forman las partes y los generamos
		nuevamente */
	if((e->ix != ix || e->iy != iy) && e->anchor != NULL){
		anchor_delete(e->anchor);
	}
	e->ix = ix;
	e->iy = iy;
	/* Calculamos la col y row del tile y las coordenadas dentro del mismo */
	engine_iso_tile(e->engine,ix,iy,tile,&(e->ixx),&(e->iyy));
	
	printf("POR\n");

	/* Posicionamos el entity en el tile*/
	e->anchor = anchor_add(tile->entities,e);

	/* Lo trozamos */
	entity_cut(e,row,col);

	printf("POR\n");
}

/*
void entity_position_add(entity_t *e, int addix, int addiy){
	int row;
	int col;

	if((e->ix != ix || e->iy != iy) && e->anchor != NULL){
		anchor_delete(e->anchor);
	}

	e->ix += addix;
	e->iy += addiy;
	engine_iso_tile(e->engine,e->ix,e->iy,&row,&col,&(e->ixx),&(e->iyy));
	engine_place_entity(e->engine,row,col,e);
}
*/

void entity_add_texture(entity_t *e, SDL_Texture *texture, int cut_w, int cut_h,
								int cut_x, int cut_y, int x_base){
	e->texture = texture;
	e->x_base = x_base;
	e->cut.x = cut_x;
	e->cut.y = cut_y;
	e->cut.w = cut_w;
	e->cut.h = cut_h;

	e->original_x = cut_x;
	e->original_w = cut_w;
}

void entity_free(entity_t *e){
	printf("Implementar!!\n");
}
