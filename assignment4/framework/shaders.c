/* Computer Graphics and Game Technology, Assignment Ray-tracing
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "shaders.h"
#include "perlin.h"
#include "v3math.h"
#include "intersection.h"
#include "scene.h"
#include "quat.h"
#include "constants.h"

// shade_constant()
//
// Always return the same color. This shader does no real computations
// based on normal, light position, etc. As such, it merely creates
// a "silhouette" of an object.

vec3
shade_constant(intersection_point ip)
{
    return v3_create(1, 0, 0);
}

vec3
shade_matte(intersection_point ip)
{
    // setup variables that will hold the dotproduct and total color values
    float ndotli, color = 0.0;
    
    // setup the vector variable that will hold the vector from the
    // intersection to the light source
    vec3 li;
    
    // loop over all the light sources
    for(int i = 0; i < scene_num_lights; i++){
        // get the intensity of the light source
        // calculate the vector from the intersection point to the light source
        li = v3_subtract(scene_lights[i].position,ip.p);
        
        // normalize the vector, might be unnecessary?
        li = v3_normalize(li);
        
        // check for shadow, use a small offset to fix self-shading
        // continue of the point is not a shadow
        if(!shadow_check(v3_add(ip.p, v3_create(0.1,0.1,0.1)), li)){
            // calculate the dotproduct between the normal vector and the
            // vector towards the light source
            ndotli = v3_dotprod(ip.n, li);
            
            // add the found value to the total color if the value is positive
            if(ndotli > 0.0)
                color += (scene_ambient_light
                        + scene_lights[i].intensity*ndotli);
            // else only add the ambient light value
            else
                color += scene_ambient_light;
        }
    }
    // return the proper color, each component must be between [0,1]
    return v3_create(color,color,color);
}

vec3
shade_blinn_phong(intersection_point ip)
{
    // setup known variables,the start value of the sums for the color and a 
    // variable that will hold the dot product of n with li.
    float kd = 0.8,
    ks = 0.2,
    alpha = 50,
    sum1 = 0.0,
    sum2 = 0.0,
    ndotli;
    
    // setup known vectors, and empty vectors that will hold values in the
    // for-loops
    vec3 cd = v3_create(1,0,0),
    cs = v3_create(1,1,1),
    li, h;
    
    // loop over all the light source
    for(int i = 0; i < scene_num_lights; i++){
        // get the intensity of the light source
        // calculate the vector from the intersection point to the light source
        li = v3_subtract(scene_lights[i].position,ip.p);
        
        // normalize the vector, might be unnecessary?
        li = v3_normalize(li);
        
        // check for shadow, use a small offset to fix self-shading
        // continue of the point is not a shadow
        if(!shadow_check(v3_add(ip.p, v3_create(0.1,0.1,0.1)), li)){
            // calculate the dotproduct between the normal vector and the
            // vector towards the light source
            ndotli = v3_dotprod(ip.n, li);
            
            // add the found value to the first if the value is positive
            if(ndotli > 0.0)
                sum1 += scene_lights[i].intensity*ndotli;
                
            // calculate the halfway point h, we use multiple because divide
            // is not defined. therefore we divide 1 by the found length
            h = v3_multiply(v3_add(ip.i,li), 1/v3_length(v3_add(ip.i,li)));
            
            // with this halfway point, calculate the value to be added to the
            // second sum
            sum2 += scene_lights[i].intensity*pow(v3_dotprod(ip.n,h),alpha);
        }
    }
    // calculate the color that should be added
    return v3_add(v3_multiply(cd, scene_ambient_light + kd*sum1),
                   v3_multiply(cs, ks*sum2));
}

vec3
shade_reflection(intersection_point ip)
{
    // calculate the value of r, the vector that represents the reflection
    vec3 r = v3_subtract(
                 v3_multiply(
                   v3_multiply(ip.n,v3_dotprod(
                                        ip.i,ip.n)
                   ),2
                 ),ip.i
              );
    // increment the reflection level
    ip.ray_level++;
    // get the color from the reflection direction, add a small value to the
    // current point p to make sure self-shadowing does not occur
    vec3 color = ray_color(ip.ray_level, v3_add(
                                             ip.p, v3_create(0.1,0.1,0.1)
                                         ),r);
    // get the color from the matte shading
    vec3 color_matte = shade_matte(ip);
    // if the reflection is not the background color, add it to the matte color
    if(color.x < 1.0 && color.y < 1.0 && color.z < 1.0)
        return v3_add(v3_multiply(color,0.25),v3_multiply(color_matte,0.75));
    // if the background color was reflected, only return the color found by
    // the matte shader
    return color_matte;
}

// Returns the shaded color for the given point to shade.
// Calls the relevant shading function based on the material index.
vec3
shade(intersection_point ip)
{
  switch (ip.material)
  {
    case 0:
      return shade_constant(ip);
    case 1:
      return shade_matte(ip);
    case 2:
      return shade_blinn_phong(ip);
    case 3:
      return shade_reflection(ip);
    default:
      return shade_constant(ip);

  }
}

// Determine the surface color for the first object intersected by
// the given ray, or return the scene background color when no
// intersection is found
vec3
ray_color(int level, vec3 ray_origin, vec3 ray_direction)
{
    intersection_point  ip;

    // If this ray has been reflected too many times, simply
    // return the background color.
    if (level >= 3)
        return scene_background_color;

    // Check if the ray intersects anything in the scene
    if (find_first_intersection(&ip, ray_origin, ray_direction))
    {
        // Shade the found intersection point
        ip.ray_level = level;
        return shade(ip);
    }

    // Nothing was hit, return background color
    return scene_background_color;
}
