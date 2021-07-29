#include "engine.h"

void engine_create(engine_t **e, SDL_Renderer *r){
	*e = (engine_t*)malloc(sizeof(engine_t));
	(*e)->renderer = r;
	(*e)->debug = false;
	(*e)->show_screen_rect = false;
}

void engine_debug(engine_t *e, bool debug){
	e->debug = debug;
}

/* Para el array dinamico que genera el mosaico del piso */
int static a2to1(engine_t *e,uint16_t row, uint16_t col){
	return row * e->mosaic_cols + col;
}

void static draw_tile(engine_t *e, uint64_t i, uint64_t j, SDL_Rect *tile){
	uint8_t index;
	SDL_Rect dest;
	int tile_y_aux;
	int tile_x_aux;
	lista_t *entities;
	entity_t *entity;

	index = e->mosaic[a2to1(e,i,j)].index;
	entities = e->mosaic[a2to1(e,i,j)].entities;

	e->tileset_cut.y = ((index / e->tileset_cols) * e->tile_height);
	e->tileset_cut.x = (index % e->tileset_cols) * e->tile_width;

	/* Guardamos el valor original para reponerlo luego */
	tile_y_aux = tile->y;
	tile_x_aux = tile->x;

	tile->h = e->tile_height;
	tile->w = e->tile_width;
	/* Recorte superior */
	if(e->playground.y > tile->y){
		tile->h = tile->y + e->tile_height - e->playground.y;
		tile->y = e->playground.y;
		e->tileset_cut.y += (e->tile_height - tile->h);
	}
	/* Recorte izquierdo */
	if(e->playground.x > tile->x){
		tile->w = tile->x + e->tile_width - e->playground.x;
		tile->x = e->playground.x;
		e->tileset_cut.x += (e->tile_width - tile->w);
	}
	/* Recorte inferior */
	if((e->playground.y + e->playground.h) < (tile->y + e->tile_height)){
		tile->h = e->tile_height - (tile->y + e->tile_height -(e->playground.y + e->playground.h));
	}
	/* Recorte derecho */
	if((e->playground.x + e->playground.w) < (tile->x + e->tile_width)){
		tile->w = e->tile_width - (tile->x + e->tile_width -(e->playground.x + e->playground.w));
	}

	e->tileset_cut.h = tile->h;
	e->tileset_cut.w = tile->w;

	/* Sumamos el desplazamiento del screen */
	dest.x = e->screen.x + tile->x - e->playground.x;
	dest.y = e->screen.y + tile->y - e->playground.y;
	dest.w = tile->w;
	dest.h = tile->h;
	SDL_RenderCopy(e->renderer,e->tileset,&(e->tileset_cut),&dest);

	if(e->debug){
		SDL_RenderPresent(e->renderer);
		SDL_Delay(300);
	}

	/* Dibujamos las entidades que tiene ancladas */

	if(entities != NULL){
		lista_first(entities);
		while(!lista_eol(entities)){
			entity = lista_get(entities);
			entity_draw(entity,&dest);
			lista_next(entities);
		}
	}

	/* Restauramos el valor original de tile->y y tile->x */
	tile->y = tile_y_aux;
	tile->x = tile_x_aux;
}

void static tile_corner(engine_t *e,int x, int y, int *row, int *col){
	int grid_row;
	int grid_col;

	x -= (e->tile_width)/2;		//	x = -40-32 = -72

	grid_row = x / e->tile_width;	//	-1
	grid_col = y / e->tile_height;//	0

	*row = grid_col - grid_row;	//	1
	*col = grid_row + grid_col;	//	-1

		/* Cuadrante superior izquierdo */
	if( x < e->tile_width * grid_row && y < e->tile_height/2 * (grid_col + 1))
		*col--;
		/* Cuadrante superior derecho */
	if( x > e->tile_width * grid_row && y < e->tile_height/2 * (grid_col + 1))
		*row--;
		/* Cuadrante inferior izquierdo */
	if( x < e->tile_width * grid_row && y > e->tile_height/2 * (grid_col + 1))
		*row++;
		/* Cuadrante inferior derecho */
	if( x > e->tile_width * grid_row && y > e->tile_height/2 * (grid_col + 1))
		*col++;
}

void engine_show_screen_rect(engine_t *e, bool show){
	e->show_screen_rect = show;
}

void engine_draw(engine_t *e){
	SDL_Rect tile;
	int64_t i,j,k,cols,rows,col,init_row;

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
	tile_corner(e,e->playground.x,e->playground.y,&tile_sup_iz_row,&tile_sup_iz_col);
	tile_corner(e,e->playground.x + e->playground.w,e->playground.y,&tile_sup_der_row,&tile_sup_der_col);
	tile_corner(e,e->playground.x,e->playground.y + e->playground.h,&tile_inf_iz_row,&tile_inf_iz_col);
	tile_corner(e,e->playground.x + e->playground.w,e->playground.y + e->playground.h,&tile_inf_der_row,&tile_inf_der_col);
	if(e->debug){
		printf("Tile corners:\n");
		printf("sup_der: (%i,%i)\n",tile_sup_der_row,tile_sup_der_col);
		printf("sup_iz: (%i,%i)\n",tile_sup_iz_row,tile_sup_iz_col);
		printf("inf_der: (%i,%i)\n",tile_inf_der_row,tile_inf_der_col);
		printf("inf_iz: (%i,%i)\n",tile_inf_iz_row,tile_inf_iz_col);
	}

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
	tile.x = 0 - (tile_sup_der_row * (e->tile_width/2));
	tile.y = 0 + (tile_sup_der_row * (e->tile_height/2));
	tile.x += (tile_sup_der_col - 1) * (e->tile_width/2);
	tile.y += (tile_sup_der_col - 1) * (e->tile_height/2);
	col_begin = tile_sup_der_col - 1;
	col_end = tile_sup_der_col + 1;

	row = tile_sup_der_row;
	while(row <= tile_inf_iz_row){
		col = col_begin;
		/* Recordamos las coordenadas del primer tile de la fila */
		tile_x = tile.x;
		tile_y = tile.y;
		if(e->debug)
			printf("row:%i cols: %i-%i\n",row,col_begin,col_end);
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
		if(row > (tile_sup_iz_row + 1)){
			tile.x = tile_x;
			tile.y = tile_y  + e->tile_height;
			col_begin++;
		} else {
			/* Sino podemos estar en la misma columna que tile_sup_iz
				en cuyo caso mantenemos la columna de inicio. O no en
				cuyo caso decrementamos la columna de inicio en uno.*/
			if(col_begin > tile_sup_iz_col){
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
		if(row > (tile_inf_der_row + 1))
			col_end--;
		else 
			/* Sino podemos estar en la misma columna que tile_inf_der
				en cuyo caso mantenemos la columna de fin. O no en cuyo caso
				incrementamos la columna de fin en una unidad */
			if(col_end < tile_inf_der_col){
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

	if(e->debug){
		printf("Rows: %u\n",e->mosaic_rows);
		printf("Cols: %u\n",e->mosaic_cols);
	}
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
			e->mosaic[a2to1(e,i,j)].entities = NULL;

			if(e->debug)
				printf("(%i,%i)",index,z);
		}
		if(e->debug)
			printf("\n");
	}
	fclose(fd);
}

void engine_add_entity(engine_t *e, int col, int row, entity_t *entity){
	tile_t *tile;

	tile = &(e->mosaic[a2to1(e,col,row)]);
	if(tile->entities == NULL){
		tile->entities = (lista_t*)malloc(sizeof(lista_t));
	   lista_init((tile->entities),sizeof(entity_t));
	}
	lista_add(tile->entities, entity);
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
