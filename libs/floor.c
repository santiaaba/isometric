#include "floor.h"

void floor_init(floor_t *f, SDL_Renderer *r, uint8_t col){
	f->renderer = r;
	f->col = col;
	f->rect.w = width;
	f->rect.h = height;
}

void floor_draw(floor_t *f){
	/* Algoritmo del pintor */
}

void floor_load_texture(floor_t *f, char *fileName){
	f->texture = IMG_LoadTexture(f->renderer, fileName);
}

/* Levanta el archivo que define la composicion del piso.
	El archivo se construye con el programa make_floor y su
	estructura es... primeros 16 bits para la cantidad de
	columnas, segundos 16 bits para la cantidad de filas,
	luego se repite la tupla (z,index) */
void floor_load_mosaic(floor_t *f, char *fileName){
	FILE *fd;
	uint8_t z;
	uint8_t index;
	fd = fopen(fileName,"rb");

	/* El primer uint32_t determina las filas (eje Y) */
	fread(&(f->rows),sizeof(uint32_t),1,fd);
	/* El segundo uint32_t determina las columnas (eje x) */
	fread(&(f->cols),sizeof(uint32_t),1,fd);
	/* Generamos el array */
	f->mosaic = (tile_t **)malloc((f->rows) * sizeof(tile_t *));
	for(i=0;i<(f->rows);i++)
		f->mosaic[i] = (tile_t **)malloc((f->cols) * sizeof(tile_t *));

	/* Poblamos el array */
	for(i=0;i<f->rows;i++){
		for(j=0;j<f->cols;j++){
			/* Leemos index */
			fread(&index,sizeof(uint8_t),1,fd);
			/* Leemos z */
			fread(&z,sizeof(uint8_t),1,fd);

			f->mosaic[i][j].index = index;
			f->mosaic[i][j].z = z;
		}
	}
	fclose(fd);
}

void floor_free(floor_t *f){
	printf("Nada de momento\n");
}
