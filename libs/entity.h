#ifndef ENTITY_H
#define ENTITY_H

typedef struct {
	point_t *position;
	border_t *border;
	vector_t *direction;
} entity_t

void e_init(entity_t *e);
void e_set_direction(entity_t *e, vector_t *v);
vector_t *e_get_direction(entity_t *e);
void e_free(entity_t *e);

#endif
