/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm 
 *
 * Student names: Tom Peerdeman & Rene Aparicio Saez
 * Student emails: tom.peerdeman@student.uva.nl & rene.apariciosaez@student.uva.nl
 * Collegekaart 10266186 & 10214054
 * Date 05 Feb 2013
 *
 */

#include "SDL.h"   
#include "init.h"

void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour) {
	// Variables for the function
	int x_check;
	int y_check;
	int total_check;

	int i, j;

	int func;
	int func_inc;
	int func_cond_inc;
	int inc_cond;
	int loop_inc;
	int loop_max;

	int loop_x;
	int check_lt;

	// Initialize checks
	x_check = x1 - x0;
	y_check = y1 - y0;
	total_check = abs(x_check) - abs(y_check);

	// Loop in x or y direction, depends on the total_check
	loop_x = (total_check > 0) ? 1 : 0;

	// The condition in the loop should check for a function value less than zero
	// if 
	if((y_check >= 0 && x_check > 0) || (x_check <= 0 && y_check <= 0)) {
		check_lt = loop_x;
	} else {
		check_lt = !loop_x;
	}
	
	// Initial function value
	func = (y0-y1) * x0 + (x1-x0) * y0 + x0 * y1 - x1 * y0;
	
	// The properties of the loop depend of we loop in x or y direcion
	if(loop_x) {
		func_inc = (y0-y1);
		loop_inc = (x_check > 0) ? 1 : -1;
		func_inc *= loop_inc;
		
		func_cond_inc = (x1-x0);
		inc_cond = (y_check > 0) ? 1 : -1;
		func_cond_inc *= inc_cond;
		
		loop_max = x1;
		i = x0;
		j = y0;
	} else {
		func_inc = (x1-x0);
		loop_inc = (y_check > 0) ? 1 : -1;
		func_inc *= loop_inc;
		
		func_cond_inc = (y0-y1);
		inc_cond = (x_check > 0) ? 1 : -1;
		func_cond_inc *= inc_cond;
		
		loop_max = y1;
		i = y0;
		j = x0;
	}
	
	// The actual loop that sets the pixel values.
	// Depending on the loop_x bool this loops over x or y, i is therefore x or y
	// j is the opposite direction of i
	for(; i != loop_max; i += loop_inc) {
		if(loop_x)
			PutPixel(s, i, j, colour);
		else
			PutPixel(s, j, i, colour);
		
		// Instead of recalculating the function just increase the function value
		func += func_inc;
		
		if((check_lt && func < 0) || (!check_lt && func >= 0)) {
			func += func_cond_inc;
			j += inc_cond;
		}
	}
}
