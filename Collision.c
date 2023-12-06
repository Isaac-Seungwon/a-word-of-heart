#include "Collision.h"

int checkCollision(SDL_Rect *a, SDL_Rect *b){
	int t = 0, result = 0;

	if ((a->x <= b->x && b->x <= a->x + a->w) || (a->x <= b->x + b->w && b->x + b->w <= a->x + a->w) || (b->x <= a->x && a->x <= b->x + b->w) || (b->x <= a->x + a->w && a->x + a->w <= b->x + b->w)) t++;
	if ((a->y <= b->y && b->y <= a->y + a->h) || (a->y <= b->y + b->h && b->y + b->h <= a->y + a->h) || (b->y <= a->y && a->y <= b->y + b->h) || (b->y <= a->y + a->h && a->y + a->h <= b->y + b->h)) t++;
	if (t == 2) result = 1;

	return result;
}