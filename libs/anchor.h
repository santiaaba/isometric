#include "entity.h"
#include <stdint.h>

typedef struct anchor anchor_t;

typedef struct anchor_node {
	entity_t *entity;
	struct anchor_node *next;
	struct anchor_node *prior;
	struct anchor_node *neiborg;
} anchor_node_t;

typedef struct anchor {
	anchor_node_t *actual;
	anchor_node_t *first;
} anchor_t;

void anchor_create(anchor_t **a);
void anchor_add(anchor_t *a, entity_t *e);
void anchor_first(anchor_t *a);
entity_t *anchor_get(anchor_t *a);
void anchor_next(anchor_t *a);
bool anchor_eol(anchor_t *);
void anchor_free(anchor_t *a);
