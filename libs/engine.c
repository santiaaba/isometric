#include "engine.h"

void engine_init(engine_t *f, SDL_Renderer *r){
	f->renderer = r;
}

/* Para el array dinamico que genera el mosaico del piso */
int static a2to1(engine_t *f,uint16_t row, uint16_t col){
	return row * f->cols + col;
}

void engine_draw(engine_t *f){
	/* Algoritmo del pintor */
	uint64_t i,j,k;
	
	for(i=0;i<f->rows;i++){
		for(j=i;j>=0;j--){
			for(k=0;k<=i;k++){
				printf("(%lu,%lu)",j,k);
			}
		}
	}
}


void engine_set_tileset(engine_t *f, char *fileName, int columns, int height, int width){
	f->col = col;
	f->cut.w = width;
	f->cut.h = height;
	f->texture = IMG_LoadTexture(f->renderer, fileName);
}

/* Levanta el archivo que define la composicion del piso.
	El archivo se construye con el programa make_engine y su
	estructura es... primeros 16 bits para la cantidad de
	columnas, segundos 16 bits para la cantidad de filas,
	luego se repite la tupla (z,index) */
void engine_load_mosaic(engine_t *f, char *fileName){
	FILE *fd;
	uint8_t z;
	uint8_t index;
	uint64_t i,j;
	fd = fopen(fileName,"rb");

	/* El primer uint32_t determina las filas (eje Y) */
	fread(&(f->rows),sizeof(uint32_t),1,fd);
	/* El segundo uint32_t determina las columnas (eje x) */
	fread(&(f->cols),sizeof(uint32_t),1,fd);
	/* Generamos el array 2D como un array 1D */
	f->mosaic = (tile_t*)malloc(f->rows * f->cols * sizeof(tile_t));

	/* Poblamos el array */
	for(i=0;i<f->rows;i++){
		for(j=0;j<f->cols;j++){
			/* Leemos index */
			fread(&index,sizeof(uint8_t),1,fd);
			/* Leemos z */
			fread(&z,sizeof(uint8_t),1,fd);

			f->mosaic[a2to1(f,i,j)].index = index;
			f->mosaic[a2to1(f,i,j)].z = z;
		}
	}
	fclose(fd);
}

uint32_t engine_mosaic_rows(engine_t *f){
	return f->rows;
}

uint32_t engine_mosaic_cols(engine_t *f){
	return f->cols;
}

void engine_free(engine_t *f){
	printf("Implementar\n");
}
