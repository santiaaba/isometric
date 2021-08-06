#include "entity.h"
#include "types.h"
#include <stdint.h>

void anchor_create(anchor_t **a);
void anchor_add(anchor_t *a, entity_t *e);
void anchor_first(anchor_t *a);
entity_t *anchor_get(anchor_t *a);
void anchor_next(anchor_t *a);
bool anchor_eol(anchor_t *);
void anchor_free(anchor_t *a);
