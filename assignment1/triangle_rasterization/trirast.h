#ifndef TRIRAST_H
#define TRIRAST_H

#include "types.h"

// Coördinates for the offpoint
#define OFF_X -1
#define OFF_Y -1

void    draw_triangle(float x0, float y0, float x1, float y1,
            float x2, float y2, byte r, byte g, byte b);
float    max(float a, float b, float c);
float    min(float a, float b, float c);
float    alphacalc(float x, float x1, float x2, float y, float y1, float y2);
float    betacalc(float x, float x0, float x2, float y, float y0, float y2);
float    gammacalc(float x, float x0, float x1, float y, float y0, float y1);


void    draw_triangle_optimized(float x0, float y0, float x1, float y1,
            float x2, float y2, byte r, byte g, byte b);



#endif

