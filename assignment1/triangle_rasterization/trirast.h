#ifndef TRIRAST_H
#define TRIRAST_H

#include "types.h"

void    draw_triangle(float x0, float y0, float x1, float y1,
            float x2, float y2, byte r, byte g, byte b);
float    max(float a, float b, float c);
float    min(float a, float b, float c);
float    betacalc(float x, float x0, float x2, float y, float y0, float y2);
float    gammacalc(float x, float x0, float x1, float y, float y0, float y1);


void    draw_triangle_optimized(float x0, float y0, float x1, float y1,
            float x2, float y2, byte r, byte g, byte b);



#endif

