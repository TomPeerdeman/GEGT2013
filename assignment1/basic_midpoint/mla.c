/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm 
 *
 * Student names: Tom Peerdeman & Rene Aparicio Saez
 * Student emails: rene.apariciosaez@student.uva.nl
 * Collegekaart 10266186 & 10214054
 * Date 05 Feb 2013
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include "SDL.h"   
#include "init.h"

/*
 * Midpoint Line Algorithm
 *
 * As you probably will have figured out, this is the part where you prove
 * your programming skills. The code in the mla function should draw a direct 
 * line between (x0,y0) and (x1,y1) in the specified color. 
 * 
 * Until now, the example code below draws only a horizontal line between
 * (x0,y0) and (x1,y0) and a vertical line between (x1,y1).
 * 
 * And here the challenge begins..
 *
 * Good luck!
 *
 *
 */
 
void mla_func(SDL_Surface *s, Uint32 colour, int x0, int y0, int x1, int y1,
		int loop_x, int check_lt) {
	int i, j;
	
	int func;
	int func_inc;
	int func_cond_inc;
	int inc_cond;
	int loop_inc;
	int loop_max;
	
	func = (y0-y1) * x0 + (x1-x0) * y0 + x0 * y1 - x1 * y0;
	
	if(loop_x) {
		func_inc = (y0-y1);
		loop_inc = (x1 - x0 > 0) ? 1 : -1;
		func_inc *= loop_inc;
		
		func_cond_inc = (x1-x0);
		inc_cond = (y1 - y0 > 0) ? 1 : -1;
		func_cond_inc *= inc_cond;
		
		loop_max = x1;
        i = x0;
        j = y0;
	} else {
        func_inc = (x1-x0);
		loop_inc = (y1 - y0 > 0) ? 1 : -1;
		func_inc *= loop_inc;
		
		func_cond_inc = (y0-y1);
		inc_cond = (x1 - x0 > 0) ? 1 : -1;
		func_cond_inc *= inc_cond;
		
		loop_max = y1;
		i = y0;
		j = x0;
	}
	
	for(; i != loop_max; i += loop_inc) {
		if(loop_x)
			PutPixel(s, i, j, colour);
		else 
			PutPixel(s, j, i, colour);
			
		func += func_inc;
		
		if((check_lt && func < 0) || (!check_lt && func >= 0)) {
			func += func_cond_inc;
			j += inc_cond;
		}
	}
} 

void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 colour) {
	// variables for the function
	int x,y;
	int x_check;
	int y_check;
	int total_check;

	// initialize checks
	x_check = x1 - x0;
	y_check = y1 - y0;
	total_check = abs(x_check) - abs(y_check);

	// set buffers
	x=x0;
	y=y0;

	// if the x change is greater than zero, draw to the right
	if(x_check > 0){
	
		// if the y change is greater than zero, draw down
		if(y_check >= 0){
		
			// draw red octant
			if(total_check > 0){
				// for(;x!=x1;x+=1){
					// PutPixel(s,x,y,colour);
					// if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
						// y++;
				// }
				mla_func(s, colour, x0, y0, x1, y1, 1, 1);
				
			}

			// draw green octant
			else{
				// for(;y!=y1;y+=1){
					// PutPixel(s,x,y,colour);
					// if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) >= 0)
						// x++;
				// }
				mla_func(s, colour, x0, y0, x1, y1, 0, 0);
			}
		}

		// if the y change is smaller than zero, draw up
		else if(y_check < 0){
		
			// draw white octant
			if(total_check > 0){
				// for(;x!=x1;x+=1){
					// PutPixel(s,x,y,colour);
					// if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) > 0)
						// y--;
				// }
				mla_func(s, colour, x0, y0, x1, y1, 1, 0);
			}

			// draw blue octant
			else{
				// for(;y!=y1;y-=1){
					// PutPixel(s,x,y,colour);
					// if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
						// x++;
				// }    
				mla_func(s, colour, x0, y0, x1, y1, 0, 1);
			}
		}
	}
	// if the x change is smaller than zero, draw to the left
	else if(x_check <= 0){
		
		// if the y change is greater than zero, draw down
		if(y_check > 0){

			// draw white octant
			if(total_check >= 0){
				// for(;x!=x1;x-=1){
					// PutPixel(s,x,y,colour);
					// if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) > 0)
						// y++;
				// }
				mla_func(s, colour, x0, y0, x1, y1, 1, 0);
			}

			// draw blue octant
			else{
				// for(;y!=y1;y+=1){
					// PutPixel(s,x,y,colour);
					// if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
						// x--;
				// }
				mla_func(s, colour, x0, y0, x1, y1, 0, 1);
			}
		}

		// if the y change is greater than zero, draw up
		else if(y_check <= 0){

			// draw red octant
			if(total_check > 0){
				// for(;x!=x1;x-=1){
					// PutPixel(s,x,y,colour);
					// if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
						// y--;
				// }
				mla_func(s, colour, x0, y0, x1, y1, 1, 1);
			}

			// draw green octant
			else{
				// for(;y!=y1;y-=1){
					// PutPixel(s,x,y,colour);
					// if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) > 0)
						// x--;
				// }
				mla_func(s, colour, x0, y0, x1, y1, 0, 0);
			}
		}
	}
}
