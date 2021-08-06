#include "engine.h"

/*****************************
           ANCHOR
******************************/
void static anchor_init(anchor_t **a){
	(*a) = (anchor_t*)malloc(sizeof(anchor_t));
	(*a)->next = NULL;
	(*a)->prio = NULL;
	(*a)->entity = NULL;
}
anchor_t *anchor_add(anchor_t *a, entity_t *e){
	/* Agrega un entity de manera ordenada en base al ixx
	   e iyy del entity. Primero los entity con iyy menor.
	   Si coinciden en iyy entonces el que tenga menor ixx
	   va primero */

	/* En esta cadena especial, siempre tenemos un primer
		eslabon que no se utiliza para almacenar datos */
	anchor_t *aux;
	anchor_t *before;
	anchor_t *new;
	bool encontro;

	before = a;
	aux = a->next;

	new = (anchor_t *)malloc(sizeof(anchor_t));
	new -> entity = e;
	new -> prio = NULL;
	new -> next = NULL;
	new -> neighbor = NULL;

	printf("Tratando de agregar\n");
	encontro = false;
	while(!encontro && aux != NULL){
		printf("Buscando\n");
		if((entity_iyy(new->entity) < entity_iyy(aux->entity)) ||
			(entity_iyy(new->entity) == entity_iyy(aux->entity)) &&
			(entity_ixx(new->entity) < entity_ixx(aux->entity))){
			encontro = true;
		} else {
			before = aux;
			aux = aux->next;
		}
	}
	new->next = aux;
	new->prio = before;
	before->next = new;

	if(aux != NULL)
		aux->prio = new;
	printf("Agrego\n");
	return new;
}

void anchor_anchor(anchor_t *a, anchor_t *b){
	/* Conecta el nodo actual de una cadena con
		el nodo de otra cadena */
	a->neighbor = b;
}

void anchor_delete(anchor_t *a){
	/* Elimina un nodo. Si el nodo tiene vecino,
		propaga la eliminación. La eliminacio
		elimina demas las entity que posee cada nodo */

	void anchor_delete_node(anchor_t *a){
		if(a->neighbor != NULL){
			anchor_delete_node(a->neighbor);
		}
		if(a->prio != NULL)
			a->prio->next = a->next;
		if(a->next != NULL)
			a->next->prio = a->prio;
		entity_free(a->entity);
		free(a);
	}
	if(a->prio == NULL){
		printf("ERROR: Intentando borrar el priemr anchor");
		exit(100);
	} else {
		anchor_delete_node(a);
	}
}

/*****************************
          ENGINE 
******************************/

/* Para el array dinamico que genera el mosaico del piso */
int static a2to1(engine_t *e,uint16_t row, uint16_t col){
	return row * e->mosaic_cols + col;
}

void engine_iso_move(SDL_Rect *rect, int ix, int iy){
	/* Mueve en coordenadas isometricas el rectangulo,
		obteniendo la nueva posicion en coordenadas
		cartesianas */
	rect->x += ix;
	rect->y += ix/2;
	rect->x -= iy;
	rect->y += iy/2;
}

/* Dado row y col, retorna el tile dentro del mosaic */
tile_t *engine_tile(engine_t *e,int row, int col){
	return &(e->mosaic[a2to1(e,row,col)]);
}

void engine_iso_cord(engine_t *e, int ix, int iy, int *x, int *y){
	/* Dadas coordenadas isometricas, calcula
	   las coordenadas para dibujar en pantalla */
	*x = ix - iy;
	*y = (ix + iy)/2;
}

void engine_iso_tile(engine_t *e, int ix, int iy, tile_t *tile, int *ixx, int *iyy){
	/* Dadas coordenadas isometricas, retorna el index
		del tile correspondiente y las coordenadas
		dentro del mismo */
	//*row = ix / e->tile_height;
	//*col = iy / e->tile_height;
	tile = &(e->mosaic[a2to1(e,(ix / e->tile_height),(iy / e->tile_height))]);
	*ixx = ix % e->tile_height;
	*iyy = iy % e->tile_height;
}

int engine_tile_width(engine_t *e){
	return e->tile_width;
}
int engine_tile_height(engine_t *e){
	return e->tile_height;
}

void engine_create(engine_t **e, SDL_Renderer *r){
	*e = (engine_t*)malloc(sizeof(engine_t));
	(*e)->renderer = r;
	(*e)->debug = false;
	(*e)->show_screen_rect = false;
}

void engine_debug(engine_t *e, bool debug){
	e->debug = debug;
}

void static draw_tile(engine_t *e, uint64_t i, uint64_t j, SDL_Rect *tile){
	uint8_t index;
	SDL_Rect dest;
	int dest_x, dest_y;
	entity_t *entity;
	anchor_t *aux;

	index = e->mosaic[a2to1(e,i,j)].index;

	e->tileset_cut.y = ((index / e->tileset_cols) * e->tile_height);
	e->tileset_cut.x = (index % e->tileset_cols) * e->tile_width;

	dest.x = tile->x;
	dest.y = tile->y;
	dest.h = e->tile_height;
	dest.w = e->tile_width;

	if(!e->debug){
		/* Recorte superior */
		if(e->playground.y > tile->y){
			dest.h = dest.y + e->tile_height - e->playground.y;
			dest.y = e->playground.y;
			e->tileset_cut.y += (e->tile_height - dest.h);
		}
  	 /* Recorte izquierdo */
		if(e->playground.x > tile->x){
			dest.w = dest.x + e->tile_width - e->playground.x;
			dest.x = e->playground.x;
			e->tileset_cut.x += (e->tile_width - dest.w);
		}
  	 /* Recorte inferior */
		if((e->playground.y + e->playground.h) < (tile->y + e->tile_height))
			dest.h = e->tile_height - (dest.y + e->tile_height -(e->playground.y + e->playground.h));
  	 /* Recorte derecho */
		if((e->playground.x + e->playground.w) < (tile->x + e->tile_width))
			dest.w = e->tile_width - (dest.x + e->tile_width -(e->playground.x + e->playground.w));
	}	
	e->tileset_cut.h = dest.h;
	e->tileset_cut.w = dest.w;

	/* Sumamos el desplazamiento del screen */
	dest.x = e->screen.x + dest.x - e->playground.x;
	dest.y = e->screen.y + dest.y - e->playground.y;

	SDL_RenderCopy(e->renderer,e->tileset,&(e->tileset_cut),&dest);

	/* Preparamos dest.x y dest.y para las entidades. Recordar
		que han quedado recordatos  */
	dest.x = e->screen.x + tile->x - e->playground.x;
	dest.y = e->screen.y + tile->y - e->playground.y;

	/* Dibujamos las entidades */
	aux = e->mosaic[a2to1(e,i,j)].entities->next;
	printf("Preparados a dibujar\n");
	while(aux != NULL){
		printf("Dibujando!!!\n");
		entity_draw(aux->entity,&dest);
		aux = aux->next;
	}
/*
	SDL_RenderPresent(e->renderer);
	SDL_Delay(1000);
*/

}

void static tile_corner(engine_t *e,int x, int y, m_index_t *index){
	int grid_row;
	int grid_col;

	if(x < 0)
		x -= e->tile_width;

	if(y < 0)
		y -= e->tile_height;

	grid_row = x / e->tile_width;
	grid_col = y / e->tile_height;

	index->row = grid_col - grid_row;
	index->col = grid_row + grid_col;
}

void engine_show_screen_rect(engine_t *e, bool show){
	e->show_screen_rect = show;
}

static void engine_cut_entities(	engine_t *e,
											m_index_t *tile_sup_der,
											m_index_t *tile_inf_der,
											m_index_t *tile_inf_iz,
											m_index_t *tile_sup_iz){
	int col_begin, col_end, row, col;
	anchor_t *aux;
	entity_t *entity;

	col_begin = tile_sup_der->col - 1;
	col_end = tile_sup_der->col + 1;
	row = tile_sup_der->row;
	while(row <= tile_inf_iz->row){
		col = col_begin;
		while(col <= col_end){
			if( row >= 0 && col >= 0 && row < e->mosaic_rows
				&& col < e->mosaic_cols){
				/* Si posee entidades lo troceamos. Para
					mejorar la performance, solo lo troceamos
					nuevamente si se ha movido. */
				aux = e->mosaic[a2to1(e,row,col)].entities->next;
				while(aux != NULL){
					printf("Partiendo\n");
					entity_cut(aux->entity,row,col);
					aux = aux->next;
				}
			}
			col++;
		}
		row++;
		if(row > (tile_sup_iz->row + 1))
			col_begin++;
		else
			if(col_begin > tile_sup_iz->col)
				col_begin--;
		if(row > (tile_inf_der->row + 1))
			col_end--;
		else 
			if(col_end < tile_inf_der->col)
				col_end++;
	}
}

void engine_draw(engine_t *e){
	SDL_Rect tile;
	int64_t i,j,k,cols,rows,col,init_row;

	m_index_t tile_sup_der;
	m_index_t tile_sup_iz;
	m_index_t tile_inf_der;
	m_index_t tile_inf_iz;

	int tile_sup_der_row;
	int tile_sup_der_col;
	int tile_sup_iz_row;
	int tile_sup_iz_col;

	int tile_inf_der_row;
	int tile_inf_der_col;
	int tile_inf_iz_row;
	int tile_inf_iz_col;

	int y1,y2,x1,x2,tile_x_init,tile_y_init;
	int col_begin, col_end, row;
	int tile_x, tile_y;
	
	/* Determinamos los tile esquinas. Algunas pueden ver virtuales porque dan 
		valores negativos en alguno de sus campos o ambos campos */
	tile_corner(e,e->playground.x,e->playground.y,&tile_sup_iz);
	tile_corner(e,e->playground.x + e->playground.w,e->playground.y,&tile_sup_der);
	tile_corner(e,e->playground.x,e->playground.y + e->playground.h,&tile_inf_iz);
	tile_corner(e,e->playground.x + e->playground.w,e->playground.y + e->playground.h,&tile_inf_der);

		/* Tile superior derecha... arranca siempre un tile menos en la fila y
		termina un tile luego en la fila. Son tres columnas siempre que se pueda */

	/* En las siguientes filas se van agregando un tile más antes y luego en la fila.
		hasta que coincida con la columna el tile de la esquina superior izquierda o inferior derecha.
		A partir de allí no se suma un tile ya que no debemos superar la coordenada del tile esquina.
		Cuando se pasa en un tile al tile esquina, se comienza a decrementar en un tile. */

	/* A partir de allí se resta un tile por fila hasta llegar a la fila del tile esquina inferior izquierda */ 

	/* Arrancamos por la primer fila que coincide con la fila del tile_der_sup. */
	/* La columna de inicio es una columna menos que la de tile_sup_der y la columna
	   de fin es una mas que tile_sup_der */

	engine_cut_entities(e, &tile_sup_der, &tile_inf_der, &tile_inf_iz, &tile_sup_iz);

	/* Ahora dibujamos tile por tile */
	tile.x = 0 - (tile_sup_der.row * (e->tile_width/2));
	tile.y = 0 + (tile_sup_der.row * (e->tile_height/2));
	tile.x += (tile_sup_der.col - 1) * (e->tile_width/2);
	tile.y += (tile_sup_der.col - 1) * (e->tile_height/2);
	col_begin = tile_sup_der.col - 1;
	col_end = tile_sup_der.col + 1;
	row = tile_sup_der.row;
	while(row <= tile_inf_iz.row){
		col = col_begin;
		/* Recordamos las coordenadas del primer tile de la fila */
		tile_x = tile.x;
		tile_y = tile.y;
		/* Por cada columna */
		while(col <= col_end){
			if( row >= 0 && col >= 0 && row < e->mosaic_rows
				&& col < e->mosaic_cols)
					draw_tile(e,row,col,&tile);
			tile.x += e->tile_width/2;
			tile.y += e->tile_height/2;
			col++;
		}
		row++;
		/* Si la fila siguiente es mayor a la fila del
			tile_sup_iz por 1 unidad, entonces decremetamos
			la columna de arranque en uno */
		if(row > (tile_sup_iz.row + 1)){
			tile.x = tile_x;
			tile.y = tile_y  + e->tile_height;
			col_begin++;
		} else {
			/* Sino podemos estar en la misma columna que tile_sup_iz
				en cuyo caso mantenemos la columna de inicio. O no en
				cuyo caso decrementamos la columna de inicio en uno.*/
			if(col_begin > tile_sup_iz.col){
				col_begin--;
				tile.x = tile_x - e->tile_width;
				tile.y = tile_y;
			} else {
				/* Estamos en la columna de tile_sup_iz. mantenemos la columna */
				tile.x = tile_x - e->tile_width/2;
				tile.y = tile_y + e->tile_height/2;
			}
		}

		/* Si la fila siguiente es mayor a a la fila
			del tile inf_der por 1 unidad entonce decrementamos
			la columna de fin en una unidad */
		if(row > (tile_inf_der.row + 1))
			col_end--;
		else 
			/* Sino podemos estar en la misma columna que tile_inf_der
				en cuyo caso mantenemos la columna de fin. O no en cuyo caso
				incrementamos la columna de fin en una unidad */
			if(col_end < tile_inf_der.col){
				col_end++;
			}
	}

	if(e->show_screen_rect){
		x1 = e->screen.x;
		x2 = e->screen.x + e->screen.w;
		y1 = e->screen.y;
		y2 = e->screen.y + e->screen.h;
		SDL_SetRenderDrawColor(e->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(e->renderer,x1,y1,x2,y1);
		SDL_RenderDrawLine(e->renderer,x1,y2,x2,y2);
		SDL_RenderDrawLine(e->renderer,x1,y1,x1,y2);
		SDL_RenderDrawLine(e->renderer,x2,y1,x2,y2);
	}
	SDL_RenderPresent(e->renderer);
}


void engine_set_tileset( engine_t *e, char *fileName,
								 int columns, int tile_width, int tile_height){
	e->tileset_cols = columns;
	e->tile_width = tile_width;
	e->tile_height = tile_height;
	e->tileset = IMG_LoadTexture(e->renderer, fileName);
	e->tileset_cut.w = tile_width;
	e->tileset_cut.h = tile_height;
}

void engine_load_mosaic(engine_t *e, char *fileName){
	FILE *fd;
	uint8_t z;
	uint8_t index;
	uint64_t i,j;
	fd = fopen(fileName,"rb");

	/* El primer uint32_t determina las filas (eje Y) */
	fread(&(e->mosaic_rows),sizeof(uint32_t),1,fd);
	/* El segundo uint32_t determina las columnas (eje x) */
	fread(&(e->mosaic_cols),sizeof(uint32_t),1,fd);

	/* Generamos el array 2D como un array 1D */
	e->mosaic = (tile_t*)malloc(e->mosaic_rows * e->mosaic_cols * sizeof(tile_t));

	/* Poblamos el array */
	for(i=0;i<e->mosaic_rows;i++){
		for(j=0;j<e->mosaic_cols;j++){
			/* Leemos index */
			fread(&index,sizeof(uint8_t),1,fd);
			/* Leemos z */
			fread(&z,sizeof(uint8_t),1,fd);

			e->mosaic[a2to1(e,i,j)].index = index;
			e->mosaic[a2to1(e,i,j)].z = z;
			/* De momento las entidades no se leen del archivo */
			/* Cada tile debe tener un primer nodo de anchor que
			   no poseerá ningun entity. Es solamente para que
				funcione el borrado encadenado cruzado entre tiles */
			anchor_init(&(e->mosaic[a2to1(e,i,j)].entities));
		}
	}
	printf("Fin carga terreno\n");
	fclose(fd);
}

void engine_place_entity(engine_t *e, int row, int col, entity_t *entity){
	tile_t *tile;

	tile = &(e->mosaic[a2to1(e,row,col)]);
	anchor_add(tile->entities,entity);
}

void engine_set_screen(engine_t *e, int x, int y, int w, int h){
	/* Screen representa la porcion de pantalla donde se dibujara */
	e->screen.x = x;
	e->screen.y = y;
	e->screen.w = w;
	e->screen.h = h;

	/* Playground posee las mismas dimenciones que screen pero
		sus coordenadas son logicas al mosaico */
	e->playground.w = w;
	e->playground.h = h;
	//rect_set_dim(e->playground,w,h);
}

void engine_set_playground(engine_t *e, uint32_t x, uint32_t y){
	e->playground.x = x;
	e->playground.y = y;
	//rect_set_point(e->playground,x,y);
}

uint32_t engine_mosaic_rows(engine_t *e){
	return e->mosaic_rows;
}

uint32_t engine_mosaic_cols(engine_t *e){
	return e->mosaic_cols;
}

void engine_free(engine_t *e){
	printf("Implementar\n");
}

