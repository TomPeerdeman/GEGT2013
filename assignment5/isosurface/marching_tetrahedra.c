/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student names: Tom Peerdeman & Rene Aparicio Saez
 * Student emails: tom.peerdeman@student.uva.nl & rene.apariciosaez@student.uva.nl
 * Collegekaart 10266186 & 10214054
 * Date 13 March 2013
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "marching_tetrahedra.h"

/* Compute a linearly interpolated position where an isosurface cuts
   an edge between two vertices (p1 and p2), each with their own
   scalar value (v1 and v2) */

static vec3
interpolate_points(unsigned char isovalue, vec3 p1, vec3 p2, unsigned char v1, unsigned char v2)
{
    double shift;
    // get the ratio between two points
    shift = (isovalue - v2) / (double) (v1 - v2);
    // apply the calculated ratio to get the interpolation between the points
    return v3_add(v3_multiply(p1, shift), v3_multiply(p2,1 - shift));
    //return v3_create(vec.x * sizex, vec.y * sizey, vec.z * sizez);
}

/* Using the given iso-value generate triangles for the tetrahedron
   defined by corner vertices v0, v1, v2, v3 of cell c.

   Store the resulting triangles in the "triangles" array.

   Return the number of triangles created (either 0, 1, or 2).

   Note: the output array "triangles" should have space for at least
         2 triangles.
*/

static void triangle_calc_normal(triangle *triangles) {
    vec3 normal = v3_normalize(v3_crossprod(v3_subtract(triangles->p[1], triangles->p[0]), 
        v3_subtract(triangles->p[2], triangles->p[0])));
        
    triangles->n[0] = normal;
    triangles->n[1] = normal;
    triangles->n[2] = normal;
}

static int
generate_tetrahedron_triangles(triangle *triangles, unsigned char isovalue, cell c, int v0, int v1, int v2, int v3)
{
    // set the values used for the boolean operators
    // values set by checking cell values against the isovalue
    int v0_less = 0, v1_less = 0, v2_less = 0, v3_less = 0;
    if(c.value[v0] <= isovalue)
      v0_less++;
    if(c.value[v1] <= isovalue)
      v1_less++;
    if(c.value[v2] <= isovalue)
      v2_less++;
    if(c.value[v3] <= isovalue)
      v3_less++;
    
    
    // case 0000 or 1111, no triangles
    if((!(v0_less) && !(v1_less) && !(v2_less) && !(v3_less)) ||
       (  v0_less  &&   v1_less  &&   v2_less  &&   v3_less)){
        
      return 0;
    }
    
    // Do all the casting already
    unsigned char val0, val1, val2, val3;
    val0 = (unsigned char) c.value[v0];
    val1 = (unsigned char) c.value[v1];
    val2 = (unsigned char) c.value[v2];
    val3 = (unsigned char) c.value[v3];

    // case 0001 or 1110, 1 triangle
    if(( v0_less  && !(v1_less) && !(v2_less) && !(v3_less)) ||
      (!(v0_less) &&   v1_less  &&   v2_less  &&   v3_less)){

        triangles->p[0] = interpolate_points(isovalue, c.p[v0], c.p[v1], val0, val1);
        triangles->p[1] = interpolate_points(isovalue, c.p[v0], c.p[v3], val0, val3);
        triangles->p[2] = interpolate_points(isovalue, c.p[v0], c.p[v2], val0, val2);

        triangle_calc_normal(triangles);
        triangles++;
        
        return 1;
    }

    // case 0010 or 1101, 1 triangle
    if((!(v0_less) &&   v1_less  && !(v2_less) && !(v3_less)) ||
      (   v0_less  && !(v1_less) &&   v2_less  &&   v3_less)){

        triangles->p[0] = interpolate_points(isovalue, c.p[v1], c.p[v0], val1, val0);
        triangles->p[1] = interpolate_points(isovalue, c.p[v1], c.p[v3], val1, val3);
        triangles->p[2] = interpolate_points(isovalue, c.p[v1], c.p[v2], val1, val2);

        triangle_calc_normal(triangles);
        triangles++;
    
        return 1;
    }

    // case 0100 or 1011, 1 triangle
    if((!(v0_less) && !(v1_less) &&   v2_less  && !(v3_less)) ||
      (   v0_less  &&   v1_less  && !(v2_less) &&   v3_less)){

        triangles->p[0] = interpolate_points(isovalue, c.p[v2], c.p[v0], val2, val0);
        triangles->p[1] = interpolate_points(isovalue, c.p[v2], c.p[v3], val2, val3);
        triangles->p[2] = interpolate_points(isovalue, c.p[v2], c.p[v1], val2, val1);

        triangle_calc_normal(triangles);
        triangles++;
    
        return 1;
    }

    // case 1000 or 0111, 1 triangle
    if((!(v0_less) && !(v1_less) && !(v2_less) &&   v3_less) ||
      (   v0_less  &&   v1_less  &&   v2_less  && !(v3_less))){

        triangles->p[0] = interpolate_points(isovalue, c.p[v3], c.p[v0], val3, val0);
        triangles->p[1] = interpolate_points(isovalue, c.p[v3], c.p[v2], val3, val2);
        triangles->p[2] = interpolate_points(isovalue, c.p[v3], c.p[v1], val3, val1);

        triangle_calc_normal(triangles);
        triangles++;
    
        return 1;
    }
    
    // case 0011 or 1100, quad -> 2 triangles
    if((!(v0_less) && !(v1_less) &&   v2_less  &&   v3_less) ||
       (  v0_less  &&   v1_less  && !(v2_less) && !(v3_less))){
           vec3 vec02, vec03, vec13, vec12;
        vec02 = interpolate_points(isovalue, c.p[v0], c.p[v2], val0, val2);
        vec03 = interpolate_points(isovalue, c.p[v0], c.p[v3], val0, val3);
        vec13 = interpolate_points(isovalue, c.p[v1], c.p[v3], val1, val3);
        vec12 = interpolate_points(isovalue, c.p[v1], c.p[v2], val1, val2);
        
        triangles->p[0] = vec03;
        triangles->p[1] = vec02;
        triangles->p[2] = vec13;

        triangle_calc_normal(triangles);
        triangles++;
        
        triangles->p[0] = vec13;
        triangles->p[1] = vec12;
        triangles->p[2] = vec02;

        triangle_calc_normal(triangles);
        triangles++;
        
        return 2;
    }
    
    // case 0101 or 1010, quad -> 2 triangles
    if((!(v0_less) &&   v1_less  && !(v2_less) && v3_less) ||
       (  v0_less  && !(v1_less) &&   v2_less  && !(v3_less))){
        vec3 vec01, vec03, vec12, vec32;
        vec01 = interpolate_points(isovalue, c.p[v0], c.p[v1], val0, val1);
        vec03 = interpolate_points(isovalue, c.p[v0], c.p[v3], val0, val3);
        vec12 = interpolate_points(isovalue, c.p[v1], c.p[v2], val1, val2);
        vec32 = interpolate_points(isovalue, c.p[v3], c.p[v2], val3, val2);
        
        triangles->p[0] = vec03;
        triangles->p[1] = vec32;
        triangles->p[2] = vec12;

        triangle_calc_normal(triangles);
        triangles++;
        
        triangles->p[0] = vec01;
        triangles->p[1] = vec12;
        triangles->p[2] = vec03;

        triangle_calc_normal(triangles);
        triangles++;
        
        return 2;
    }
    
    // case 0110 or 1001, quad -> 2 triangles
    if((!(v0_less) &&   v1_less  &&   v2_less  && !(v3_less)) ||
       (  v0_less  && !(v1_less) && !(v2_less) && v3_less)){
       
        vec3 vec13, vec10, vec32, vec02;
        vec13 = interpolate_points(isovalue, c.p[v1], c.p[v3], val1, val3);
        vec10 = interpolate_points(isovalue, c.p[v1], c.p[v0], val1, val0);
        vec32 = interpolate_points(isovalue, c.p[v3], c.p[v2], val3, val2);
        vec02 = interpolate_points(isovalue, c.p[v0], c.p[v2], val0, val2);
        
        triangles->p[0] = vec32;
        triangles->p[1] = vec13;
        triangles->p[2] = vec02;

        triangle_calc_normal(triangles);
        triangles++;
        
        triangles->p[0] = vec13;
        triangles->p[1] = vec10;
        triangles->p[2] = vec02;

        triangle_calc_normal(triangles);
        triangles++;
        
        return 2;
    }
    return 0;
}

/* Generate triangles for a single cell for the given iso-value. This function
   should produce at most 6 * 2 triangles (for which the "triangles" array should
   have enough space).

   Use calls to generate_tetrahedron_triangles().

   Return the number of triangles produced
*/

int
generate_cell_triangles(triangle *triangles, cell c, unsigned char isovalue)
{
    int ntriangles, ntotal = 0;
    
    ntriangles = generate_tetrahedron_triangles(triangles, isovalue, c, 0, 1, 3, 7);
    triangles += ntriangles;
    ntotal += ntriangles;
    
    ntriangles = generate_tetrahedron_triangles(triangles, isovalue, c, 0, 2, 6, 7);
    triangles += ntriangles;
    ntotal += ntriangles;
    
    ntriangles = generate_tetrahedron_triangles(triangles, isovalue, c, 0, 1, 5, 7);
    triangles += ntriangles;
    ntotal += ntriangles;
    
    ntriangles = generate_tetrahedron_triangles(triangles, isovalue, c, 0, 2, 3, 7);
    triangles += ntriangles;
    ntotal += ntriangles;
    
    ntriangles = generate_tetrahedron_triangles(triangles, isovalue, c, 0, 4, 5, 7);
    triangles += ntriangles;
    ntotal += ntriangles;
    
    ntotal += generate_tetrahedron_triangles(triangles, isovalue, c, 0, 4, 6, 7);
    
    return ntotal;
}
