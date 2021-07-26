#include "rect.h"

void rect_init(rect_t *rect){
	point_init(rect->point);
	rect->width = 0;
	rect->height = 0;
}

point_t *rect_get_point(rect_t *rect){
	return rect->point;
}

void rect_set_point(rect_t *rect, int32_t x, int32_t y){
	point_set(rect->point,x,y);
}

void rect_set_dim(rect_t *rect, uint32_t width, uint32_t height){
	rect->width = width;
	rect->height = height;
}

uint32_t rect_get_width(rect_t *rect){
	return rect->width;
}

uint32_t rect_get_height(rect_t *rect){
	return rect->height;
}

uint16_t rect_collision(rect_t *rect, rect_t *rect2){
	
	uint32_t b_x = point_x(rect->point);
	uint32_t b_y = point_y(rect->point);
	uint32_t b_h = rect->height;
	uint32_t b_w = rect->width;

	uint32_t o_x = point_x(rect_get_point(rect2));
	uint32_t o_y = point_y(rect_get_point(rect2));
	uint32_t o_h = rect_get_height(rect2);
	uint32_t o_w = rect_get_width(rect2);

	return (o_x > b_x && o_x < b_x + b_w &&
	((o_y < b_y && o_y > b_y - b_h) ||
	(o_y + o_h < b_y && o_y + o_h > b_y - b_h))) ||

	(o_x + o_w > b_x && o_x < b_x + b_w &&
	((o_y + o_w < b_y && o_y + o_w > b_y - b_h) ||
	(o_y + o_h < b_y && o_y + o_h > b_y - b_h)));
}

void rect_free(rect_t **rect){
	free((*rect)->point);
	free(*rect);
}
