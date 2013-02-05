/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm 
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
  int ix,iy;
  int x,y;
  int x_check;
  int y_check;
  int total_check;
  
  PutPixel(s,x0,y0,colour);
  PutPixel(s,x1,y1,colour);
  
  x_check = x1 - x0;
  y_check = y1 - y0;
  total_check = abs(x_check) - abs(y_check);
  
  // if the y change is zero, draw a horizontal line
  if(y_check == 0){
    if(x1>x0) ix=1; else ix=-1;
    for(x=x0;x!=x1;x+=ix)
      PutPixel(s,x,y0,colour);
  }
  
  // if the x change is zero, draw a vertical line
  else if(x_check == 0){
    if(y1>y0) iy=1; else iy=-1;
    for(y=y0;y!=y1;y+=iy)
      PutPixel(s,x1,y,colour); 
  }
  
  // if the x change is greater than zero, draw to the right
  else if(x_check > 0){
    // if the y change is greater than zero, draw down
    if(y_check > 0){
      
    }
    
    
    // if the y change is smaller than zero, draw up
    if(y_check < 0){
      x=x0;
      y=y0;
      printf("\ntotal: %d",total_check);
      printf("\nx,y: %d;%d, x1,y1: %d;%d", x,y, x1,y1);
      if(total_check >= 0){
        for(;x!=x1;x+=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) >= 0)
            y--;
        }
      }
      else{
        for(;y!=y1;y-=1){
          PutPixel(s,x,y,colour);
          if(((y0-y1) * x + (x1-x0) * y + x0 * y1 - x1 * y0) < 0)
            x++;
        }      
      }
      printf("\nx,y: %d;%d, x1,y1: %d;%d\n", x,y, x1,y1);
    }
  }
  
  // if the x change is smaller than zero, draw to the left
  else if(x_check < 0){
    // if the y change is greater than zero, draw down
    if(y_check > 0){
      
    }
    
    
    // if the y change is greater than zero, draw up
    if(y_check < 0){
    
    }
  }
  

  return;
}

