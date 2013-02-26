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
    // setup variables that will hold the dotproduct and intensity values
    float ndotli;
    // setup variable that will store the color that should be returned
    float color = 0.0;
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
        
        // calculate the dotproduct between the normal vector and the vector
        // towards the light source
        ndotli = v3_dotprod(ip.n, li);
        // add the found value to total color if the value is positive
        if(ndotli > 0.0)
            color += (scene_ambient_light + scene_lights[i].intensity*ndotli);
        else
            color += scene_ambient_light;
    }
    // return the proper color, each component must be between [0,1]
    return v3_create(color,color,color);
}

vec3
shade_blinn_phong(intersection_point ip)
{
    return v3_create(1, 0, 0);
}

vec3
shade_reflection(intersection_point ip)
{
    return v3_create(1, 0, 0);
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