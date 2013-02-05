/* Computer Graphics assignment, Triangle Rasterization
 * Filename ........ trirast.c
 * Description ..... Implements triangle rasterization
 * Created by ...... Paul Melis
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"
#include "trirast.h"

/*
 * Rasterize a single triangle.
 * The triangle is specified by its corner coordinates
 * (x0,y0), (x1,y1) and (x2,y2).
 * The triangle is drawn in color (r,g,b).
 */


/*
 * we use barycentric coordinates
 * (x1-x0  x2-x0) (B)   (x-x0)
 * (y1-y0  y2-y0) (Y) = (y-y0)
 *
 * B = f20(x,y)/f20(x1,y1)
 * f20(x,y) = (y2-y0)*x + (x0-x2)*y + x2*y0 - x0*y2
 *
 * Y = f01(x,y)/f01(x1,y1)
 * f01(x,y) = (y0-y1)*x + (x1+x0)*y + x0*y1 - x1*y0
 *
 * determine bounding box xmin, xmax, ymin, ymax
 * for y = ymin to ymax do
 *   for x = xmin to xmax do
 *     beta = f20(x, y) / f20(x1, y1)
 *     gamma = f01(x, y) / f01(x2, y2)
 *     alpha = 1 - beta - gamma
 *     if (alpha, beta, gamma > 0 && < 1) then
 *       c = alpha c0 + beta c1 + gamma c2
 *       drawpixel (x, y) with color c
 *
 */
void
draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{

	// variables
	float x_max, y_max, x_min, y_min;
	float x, y;
	float alpha, beta, gamma;

	// set boundaries
	x_max = max(x0,x1,x2);
	y_max = max(y0,y1,y2);
	x_min = min(x0,x1,x2);
	y_min = min(y0,y1,y2);

	for(y=y_min; y < y_max; y++){
		for(x=x_min; x < x_max; x++){
			beta = betacalc(x,x0,x2,y,y0,y2)/betacalc(x1,x0,x2,y1,y0,y2);
			gamma = gammacalc(x,x0,x1,y,y0,y1)/gammacalc(x2,x0,x1,y2,y0,y1);
			alpha = 1 - beta - gamma;
			if(alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1 ){
            PutPixel(x, y, r, g, b);
			}
		}
	}

}

// helper function to get the maximum value of three integers
float max(float a, float b, float c){
  float max = a;
  if(b > a)
    max = b;
  if(c > b)
    max = c;
  return max;
}

// helper function to get the minimum value of three integers
float min(float a, float b, float c){
  float min = a;
  if(b < a)
    min = b;
  if(c < b)
    min = c;
  return min;
}

// helper function to calculate a part of the beta value
float betacalc(float x, float x0, float x2, float y, float y0, float y2){
	return((y2-y0)*x + (x0-x2)*y + x2*y0 - x0*y2);
}

// helper function to calculate a part of the gamma value
float gammacalc(float x, float x0, float x1, float y, float y0, float y1){
	return((y0-y1)*x + (x1+x0)*y + x0*y1 - x1*y0);
}

void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
    byte r, byte g, byte b)
{
}
