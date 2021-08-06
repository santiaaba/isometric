#include "anchor.h"

void anchor_add(anchor_t *a, entity_t *e){
	/* Agrega un entity de manera ordenada en base al ixx
	   e iyy del entity. Primero los entity con iyy menor.
	   Si coinciden en iyy entonces el que tenga menor ixx
	   va primero */

	anchor_node_t *aux;
	anchor_node_t *before;
	anchor_node_t *new;
	bool encontro;

	aux = a->first;
	before = a->first;
	new = (anchor_node_t *)malloc(sizeof(anchor_node_t));
	new -> entity = e;
	if(aux == NULL){
	} else {
		encontro = false;
		while(!encontro && aux != NULL){
			if((entity_iyy(new->entity) < entity_iyy(aux->entity)) ||
				(entity_iyy(new->entity) == entity_iyy(aux->entity)) &&
				(entity_ixx(new->entity) < entity_ixx(aux->entity))){
				encontro = true;
			} else {
				before = aux;
				aux = aux->next;
			}
			new->next = aux;
			new->prior = aux->prior;
			aux->prior = new;
			if(before == aux){
				a->first = new;
			} else {
				before->next = new;
			}
		}
	}
}

void anchor_anchor(anchor_t *a, anchor_t *b){
	/* Conecta el nodo actual de una cadena con
		el nodo de otra cadena */
	a->actual->neiborg = b->actual;
}

void anchor_delete(anchor_t *a){
	/* Elimina un nodo. Si el nodo tiene vecino,
		propaga la eliminación. La eliminacio
		elimina demas las entity que posee cada nodo */

	void anchor_delete_node(anchor_node_t *node){
		if(a->actual->neiborg != NULL){
			anchor_delete_node(a->actual->neiborg);
		}
		node->before->next = node->next;
		node->next->before = node->before;
		entity_free(node->entity);
		free(node);
	}
	anchor_delete_node(a->actual);
}

void anchor_first(anchor_t *a){
	a->actual = a->first;
}

entity_t *anchor_get(anchor_t *a){
	return a->actual->entity;
}

void anchor_next(anchor_t *a){
	a->actual = a->actual->next;
}

bool anchor_eol(anchor_t *){
	return a->actual == NULL;
}

void anchor_free(anchor_t *a){
	printf("Implementar");
}
