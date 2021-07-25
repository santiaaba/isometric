/* Dado un archivo csv se genera un binario
	que posee el piso */

/* Cada linea del csv debe tener la misma cantidad
	de columnas. Cada celda posee la tupla (index,z).
*/

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000

int main(char argc, char * argv[]){

	int next(char *chunk, char finding, uint64_t first){
		uint64_t last = first;
		while(chunk[last] != finding)
			last++;
		return last;
	}

	void par(char *chunk, uint64_t *first, uint8_t *index, uint8_t *z){
		char part[20];
		int last;

		last = next(chunk,',',*first);
		memcpy(part,&(chunk[*first]),last - *first);
		part[last - *first] = '\0';
		*index = atoi(part);
		*first = last + 1;
		last = next(chunk,';',*first);
		memcpy(part,&(chunk[*first]),last - *first);
		part[last - *first] = '\0';
		*z = atoi(part);
		*first = last + 1;
	}

	FILE *fdIN;
	FILE *fdOUT;
	char chunk[MAXLINE];
	uint32_t row,col;
	uint64_t first,last;
	uint8_t index;
	uint8_t z;

	fdIN = fopen(argv[1],"r");
	fdOUT = fopen(argv[2],"wb");

	/* Escribimos en 0 los dos primeros valores */
	row = 0;
	col = 0;
	fwrite(&row,sizeof(uint32_t),1,fdOUT);
	fwrite(&col,sizeof(uint32_t),1,fdOUT);

	while(fgets(chunk, sizeof(chunk), fdIN) != NULL) {
		row++;
		first = 0;
		printf("Linea leida %s --> %lu\n",chunk,strlen(chunk));
		while(first + 1 < strlen(chunk)){
			par(chunk,&first,&index,&z);
			col++;
			printf("(%i,%i, %lu)",index,z,first);
			fwrite(&index,sizeof(uint8_t),1,fdOUT);
			fwrite(&z,sizeof(uint8_t),1,fdOUT);
		}
	}
	fseek(fdOUT,0,SEEK_SET);
	fwrite(&row,sizeof(uint32_t),1,fdOUT);
	fwrite(&col,sizeof(uint32_t),1,fdOUT);
	fclose(fdIN);
	fclose(fdOUT);

	/* Pisamos al inicio la cantidad de filas y columnas
	   calculadas */
}
