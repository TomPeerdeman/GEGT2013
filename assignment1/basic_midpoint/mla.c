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
        for(;x!=x1;x+=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
            y++;
        }
      }
      
      // draw green octant
      else{
        for(;y!=y1;y+=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) >= 0)
            x++;
        }
      }
    }
    
    
    // if the y change is smaller than zero, draw up
    else if(y_check < 0){
      
      // draw white octant
      if(total_check > 0){
        for(;x!=x1;x+=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) > 0)
            y--;
        }
      }
      
      // draw blue octant
      else{
        for(;y!=y1;y-=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
            x++;
        }      
      }
    }
  }
  // if the x change is smaller than zero, draw to the left
  else if(x_check <= 0){
    // if the y change is greater than zero, draw down
    if(y_check > 0){
      
      // draw white octant
      if(total_check >= 0){
        for(;x!=x1;x-=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) > 0)
            y++;
        }
      }
      
      // draw blue octant
      else{
        for(;y!=y1;y+=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
            x--;
        }
      }
    }
    
    // if the y change is greater than zero, draw up
    else if(y_check <= 0){
      
      // draw red octant
      if(total_check > 0){
        for(;x!=x1;x-=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
            y--;
        }
      }
      
      // draw green octant
      else{
        for(;y!=y1;y-=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) > 0)
            x--;
        }
      }
    }
  }
  return;
}

