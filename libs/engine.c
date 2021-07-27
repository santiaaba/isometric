#include "engine.h"

void engine_create(engine_t **e, SDL_Renderer *r){
	*e = (engine_t*)malloc(sizeof(engine_t));
	(*e)->renderer = r;
	(*e)->debug = false;
	(*e)->tileset_cut.w = TILE_WIDTH;
	(*e)->tileset_cut.h = TILE_HEIGHT;
}

void engine_debug(engine_t *e, bool debug){
	e->debug = debug;
}

/* Para el array dinamico que genera el mosaico del piso */
int static a2to1(engine_t *e,uint16_t row, uint16_t col){
	return row * e->mosaic_cols + col;
}

void engine_draw(engine_t *e){

	void draw_tile(engine_t *e, uint64_t i, uint64_t j, SDL_Rect *tile){
		uint8_t index;
		SDL_Rect dest;
		int tile_y_aux;
		int tile_x_aux;

		printf("Dibujamos tile (%lu,%lu)\n",i,j);
		index = e->mosaic[a2to1(e,i,j)].index;

		e->tileset_cut.y = ((index / e->tileset_cols) * TILE_HEIGHT);
		e->tileset_cut.x = (index % e->tileset_cols) * TILE_WIDTH;

		/* Guardamos el valor original para reponerlo luego */
		tile_y_aux = tile->y;
		tile_x_aux = tile->x;

		tile->h = TILE_HEIGHT;
		tile->w = TILE_WIDTH;
		/* Recorte superior */
		if(e->playground.y > tile->y){
			printf("ENTRO recorte superior\n");
			tile->h = tile->y + TILE_HEIGHT - e->playground.y;
			tile->y = e->playground.y;
			e->tileset_cut.y += (TILE_HEIGHT - tile->h);
		}
		/* Recorte izquierdo */
		if(e->playground.x > tile->x){
			printf("ENTRO recorte izquierdo\n");
			tile->w = tile->x + TILE_WIDTH - e->playground.x;
			tile->x = e->playground.x;
			e->tileset_cut.x += (TILE_WIDTH - tile->w);
		}
		/* Recorte inferior */
		if((e->playground.y + e->playground.h) < (tile->y + TILE_HEIGHT)){
			printf("ENTRO recorte inferior\n");
			tile->h = TILE_HEIGHT - (tile->y + TILE_HEIGHT -(e->playground.y + e->playground.h));
		}
		/* Recorte derecho */
		if((e->playground.x + e->playground.w) < (tile->x + TILE_WIDTH)){
			printf("ENTRO recorte derecho\n");
			tile->w = TILE_WIDTH - (tile->x + TILE_WIDTH -(e->playground.x + e->playground.w));
		}

		e->tileset_cut.h = tile->h;
		e->tileset_cut.w = tile->w;

/*
		tile->w = e->tileset_cut.w;
*/

	/*
		e->tileset_cut.y = (index / e->tileset_cols) * TILE_HEIGHT;
		e->tileset_cut.h = TILE_HEIGHT;

		dest->h = e->tileset_cut.h;

	*/
		/* Sumamos el desplazamiento del screen */
		dest.x = e->screen.x + tile->x - e->playground.x;
		dest.y = e->screen.y + tile->y - e->playground.y;
		dest.w = tile->w;
		dest.h = tile->h;
		printf("(x:%i,y:%i,w:%i,h:%i)\n",dest.x,dest.y,dest.w,dest.h);
		SDL_RenderCopy(e->renderer,e->tileset,&(e->tileset_cut),&dest);

		if(e->debug){
			SDL_RenderPresent(e->renderer);
			SDL_Delay(300);
		}

		/* Estauramos el valor original de tile->y y tile->x */
		tile->y = tile_y_aux;
		tile->x = tile_x_aux;
	}

	SDL_Rect tile;
	int64_t i,j,k,cols,rows,col,row;
	int y1,y2,x1,x2;
	int printed;
	
	printf("Comenzamos a dibujar\n");

	/* Indicamos el punto inicial para el sprite (0,0) */

	tile.x = 0;
	tile.y = 0;

	/* Mitad superior. Imprimimos a partir de la fila */
	row = 0;
	rows = e-> mosaic_rows;
	while(row < rows){
		j = 0;
		i = row;
		printed = 0;
		while(j< e->mosaic_cols && i>=0){
			draw_tile(e,i,j,&tile);
			printed ++;
			tile.x += TILE_WIDTH;
			j++;
			i--;
		}
		printf("\n");
		printf("REstamos: %i\n",printed);
		tile.y += TILE_HEIGHT/2;
		tile.x -= (TILE_WIDTH/2 + TILE_WIDTH * printed);
		row++;
	}
	printf("Restantes\n");
	printf("Mosaic_cols:%i\n",e->mosaic_cols);

	/* Mitad inferior. Imprimimos a partir de la columna */
	row = row - 1;
	col = 1;
	tile.x += TILE_WIDTH;
	while(col < e->mosaic_cols){
		i = row;
		j = col;
		printed = 0;
		while(j<e->mosaic_cols && i >= 0){
			draw_tile(e,i,j,&tile);
			tile.x += TILE_WIDTH;
			i--;
			j++;
			printed++;
		}
		printf("\n");
		col++;
		tile.x -= (TILE_WIDTH/2 + TILE_WIDTH * (printed - 1));
		tile.y += TILE_HEIGHT/2;
	}


	/* Debug del recuadro del screen */
	if(e->debug){
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
								 int columns, int height, int width){
	e->tileset_cols = columns;
	e->tileset_cut.w = width;
	e->tileset_cut.h = height;
	printf("Aca muere %s %p\n",fileName,e->renderer);
	e->tileset = IMG_LoadTexture(e->renderer, fileName);
	printf("Aca muere2\n");
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

			if(e->debug)
				printf("(%i,%i)",index,z);
		}
		if(e->debug)
			printf("\n");
	}
	fclose(fd);
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
