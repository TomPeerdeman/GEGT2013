/* Computer Graphics, Assignment, Bezier curves
 * Filename ........ bezier.c
 * Description ..... Bezier curves
 * Date ............ 22.07.2009
 * Created by ...... Paul Melis
 *
 *
 * Student names: Tom Peerdeman & Rene Aparicio Saez
 * Student emails: tom.peerdeman@student.uva.nl & rene.apariciosaez@student.uva.nl
 * Collegekaart 10266186 & 10214054
 * Date 18 Feb 2013
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <math.h>
#include "bezier.h"
#include <stdio.h>

// Helper function, given an integer value, calculate its factorial value
float factorial(int fact){
	float returnVal;
	if(fact == 0)
		returnVal = 1;
	else{
		returnVal = 1;
		for(int i = 1; i < fact; i++){
			returnVal *= (i+1);
		}
	}	
	return returnVal;
}

// helper function, given n and k calculate its binomial distribution
float binomial(int n, int k){
	float factn = factorial(n);
	float factk = factorial(k);
	float factnmink = factorial(n-k);
	float u = factn/(factk*factnmink);
	return u;
}

// helper function, calculate the Bernstein polynomial value for point u
float bernstein(int n, int i, float u){
	float bernVal, bin;
	bin = binomial(n, i);
	bernVal = bin*pow(u,i)*pow((1-u),(n-i));
	return bernVal;
}

/* Given a Bezier curve defined by the 'num_points' control points
 * in 'p' compute the position of the point on the curve for parameter
 * value 'u'.
 *
 * Return the x and y values of the point by setting *x and *y,
 * respectively.
 */

void evaluate_bezier_curve(float *x, float *y, control_point p[], 
				int num_points, float u){
	float bs;
	*x = 0.0;
	*y = 0.0;

	for(int i = 0; i < num_points; i++) {
		bs = bernstein(num_points - 1, i, u);
		*x += bs * p[i].x;
		*y += bs * p[i].y;
	}
}

/* Draw a Bezier curve defined by the control points in p[], which
 * will contain 'num_points' points.
 *
 * Draw the line segments you compute directly on the screen
 * as a single GL_LINE_STRIP.
 *
 * The 'num_segments' parameter determines the "discretization" of the Bezier
 * curve and is the number of straight line segments that should be used
 * to approximate the curve.
 *
 * Call evaluate_bezier_curve() to compute the necessary points on
 * the curve.
 */

void draw_bezier_curve(int num_segments, control_point p[], int num_points){
	float x, y;
	float u = 0;
	float uInc = 1.0 / num_segments;
	
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i <= num_segments; i++, u += uInc) {
		x = 0.0;
		y = 0.0;
		evaluate_bezier_curve(&x,&y,p,num_points,u);
		glVertex2f(x, y);
	}
	glEnd();
}

/* Find the intersection of a cubic Bezier curve with the line X=x.
   Return 1 if an intersection was found and place the corresponding y
   value in *y.
   Return 0 if no intersection exists.
*/

int intersect_cubic_bezier_curve(float *liney, control_point p[], float linex){
	if(linex >= p[0].x && linex <= p[3].x) {
		float x, y, d;
		for(float u = 0.0; u <= 1.0; u += 0.001){
			x = 0;
			y = 0;
			evaluate_bezier_curve(&x, &y, p, 4, u);
			
			d = linex - x;
			if(d <= 0.001 && d >= -0.001){
				*liney = y;
				return 1;
			}
		}
	}
	return 0;
}

