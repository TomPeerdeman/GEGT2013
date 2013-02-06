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
    byte r, byte g, byte b) {
	
	//printf("Draw triangle {(%f, %f), (%f, %f), (%f, %f)} color (%d, %d, %d)\n", x0, y0, x1, y1, x2, y2, r, g, b);
	// variables
	float x_max, y_max, x_min, y_min;
	float x, y;
	float alpha, beta, gamma;
	float f12, f20, f01;
	float p0_off, p1_off, p2_off;

	// set boundaries
	x_max = ceil(max(x0,x1,x2));
	y_max = ceil(max(y0,y1,y2));
	x_min = floor(min(x0,x1,x2));
	y_min = floor(min(y0,y1,y2));
	
	//printf("Loop x:%f->%f y:%f->%f\n", x_min, x_max, y_min, y_max);
	
	f12 = alphacalc(x0, x1, x2, y0, y1, y2);
	f20 = betacalc(x1, x0, x2, y1, y0, y2);
	f01 = gammacalc(x2, x0, x1, y2, y0, y1);
	
	// f12(off_x, offy) * f12(x0, y0)
	p0_off = alphacalc(OFF_X, x1, x2, OFF_Y, y1, y2) * f12;
	p1_off = betacalc(OFF_X, x0, x2, OFF_Y, y0, y2) * f20;
	p2_off = gammacalc(OFF_X, x0, x1, OFF_Y, y0, y1) * f01;

	for(y=y_min; y < y_max; y++){
		for(x=x_min; x < x_max; x++){
			beta = betacalc(x,x0,x2,y,y0,y2) / f20;
			gamma = gammacalc(x,x0,x1,y,y0,y1) / f01;
			alpha = alphacalc(x,x1,x2,y,y1,y2) / f12;
			if(alpha >= 0 && beta >= 0 && gamma >= 0) {
				if((alpha > 0 || p0_off > 0) && (beta > 0 || p1_off > 0)
						&& (gamma > 0 || p2_off > 0)) {
					/*r = (int) 255.0f * alpha;
					g = (int) 255.0f * beta;
					b = (int) 255.0f * gamma;*/
 					PutPixel(x, y, r, g, b);
				}
			}
		}
	}

}

// helper function to get the maximum value of three integers
float max(float a, float b, float c) {
	float m = a;
	if(b > m){
		m = b;
		if(c > m)
			m = c;
		return m;
	}
	if(c > m)
	  m = c;
	return m;
}

// helper function to get the minimum value of three integers
float min(float a, float b, float c) {
	float m = a;
	if(b < m){
		m = b;
		if(c < m)
			m = c;
		return m;
	}
	if(c < m)
		m = c;
	return m;
}

// helper function to calculate a part of the alpha value
float alphacalc(float x, float x1, float x2, float y, float y1, float y2) {
	return((y1-y2)*x + (x2-x1)*y + x1*y2 - x2*y1);
}

// helper function to calculate a part of the beta value
float betacalc(float x, float x0, float x2, float y, float y0, float y2) {
	return((y2-y0)*x + (x0-x2)*y + x2*y0 - x0*y2);
}

// helper function to calculate a part of the gamma value
float gammacalc(float x, float x0, float x1, float y, float y0, float y1) {
	return((y0-y1)*x + (x1-x0)*y + x0*y1 - x1*y0);
}

void
draw_triangle_optimized(float x0, float y0, float x1, float y1, float x2, float y2,
		byte r, byte g, byte b) {
	(void)(x0);
	(void)(y0);
	(void)(x1);
	(void)(y1);
	(void)(x2);
	(void)(y2);
	(void)(r);
	(void)(g);
	(void)(b);
}
