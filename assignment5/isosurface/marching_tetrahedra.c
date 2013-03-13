/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 * (always fill in these fields before submitting!!)
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
	if(v1 > v2) {
		// Isovalue - v2 = 0 means isovalue = v2, so shift = 0
		// Shift = 0 means 1.0 - 0.0 = 1.0 times v2 and 0.0 times v1
		shift = (double) (isovalue - v2) / (double) (v1 - v2);
	} else if(v2 > v1) {
		shift = (double) (isovalue - v1) / (double) (v2 - v1);
		// Reverse the shift since the points are the other way around
		shift = 1.0 - shift;
	} else {
		return p1;
	}

    return v3_add(v3_multiply(p1, shift), v3_multiply(p2, (1.0 - shift)));
}

/* Using the given iso-value generate triangles for the tetrahedron
   defined by corner vertices v0, v1, v2, v3 of cell c.

   Store the resulting triangles in the "triangles" array.

   Return the number of triangles created (either 0, 1, or 2).

   Note: the output array "triangles" should have space for at least
         2 triangles.
*/

static int
generate_tetrahedron_triangles(triangle *triangles, unsigned char isovalue, cell c, int v0, int v1, int v2, int v3)
{
    // case 0000 or 1111, no triangles
    if((c.value[v0] < isovalue && c.value[v1] < isovalue
        && c.value[v2] < isovalue && c.value[v3] < isovalue)
        ||
        (c.value[v0] > isovalue && c.value[v1] > isovalue
        && c.value[v2] > isovalue && c.value[v3] > isovalue
        )){
    
      return 0;
    }

    // case 0001 or 1110, 1 triangle
    if((c.value[v0] < isovalue && c.value[v1] < isovalue
        && c.value[v2] < isovalue && c.value[v3] > isovalue)
        ||
        (c.value[v0] > isovalue && c.value[v1] > isovalue
        && c.value[v2] > isovalue && c.value[v3] < isovalue
        )){
    
      return 1;
    }

    // case 0010 or 1101, 1 triangle
    if((c.value[v0] < isovalue && c.value[v1] < isovalue
        && c.value[v2] > isovalue && c.value[v3] < isovalue)
        ||
        (c.value[v0] > isovalue && c.value[v1] > isovalue
        && c.value[v2] < isovalue && c.value[v3] > isovalue
        )){
    
      return 1;
    }

    // case 0100 or 1011, 1 triangle
    if((c.value[v0] < isovalue && c.value[v1] > isovalue
        && c.value[v2] < isovalue && c.value[v3] < isovalue)
        ||
        (c.value[v0] > isovalue && c.value[v1] < isovalue
        && c.value[v2] > isovalue && c.value[v3] > isovalue
        )){
    
      return 1;
    }

    // case 1000 or 0111, 1 triangle
    if((c.value[v0] > isovalue && c.value[v1] < isovalue
        && c.value[v2] < isovalue && c.value[v3] < isovalue)
        ||
        (c.value[v0] < isovalue && c.value[v1] > isovalue
        && c.value[v2] > isovalue && c.value[v3] > isovalue
        )){
    
      return 1;
    }
    
    // case 0011 or 1100, quad -> 2 triangles
    if((c.value[v0] < isovalue && c.value[v1] < isovalue
        && c.value[v2] > isovalue && c.value[v3] > isovalue)
        ||
        (c.value[v0] > isovalue && c.value[v1] > isovalue
        && c.value[v2] < isovalue && c.value[v3] < isovalue
        )){
      return 2;
    }
    
    // case 0101 or 1010, quad -> 2 triangles
    if((c.value[v0] < isovalue && c.value[v1] > isovalue
        && c.value[v2] < isovalue && c.value[v3] > isovalue)
        ||
        (c.value[v0] > isovalue && c.value[v1] < isovalue
        && c.value[v2] > isovalue && c.value[v3] < isovalue
        )){
      return 2;
    }
    
    // case 0110 or 1001, 1 triangle
    if((c.value[v0] < isovalue && c.value[v1] > isovalue
        && c.value[v2] > isovalue && c.value[v3] < isovalue)
        ||
        (c.value[v0] > isovalue && c.value[v1] < isovalue
        && c.value[v2] < isovalue && c.value[v3] > isovalue
        )){
      return 1;
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
	int ntriangles = generate_tetrahedron_triangles(triangles, isovalue, c, 0, 1, 3, 7);
	ntriangles += generate_tetrahedron_triangles(triangles, isovalue, c, 0, 2, 6, 7);
	ntriangles += generate_tetrahedron_triangles(triangles, isovalue, c, 0, 1, 5, 7);
	ntriangles += generate_tetrahedron_triangles(triangles, isovalue, c, 0, 2, 3, 7);
	ntriangles += generate_tetrahedron_triangles(triangles, isovalue, c, 0, 4, 5, 7);
	ntriangles += generate_tetrahedron_triangles(triangles, isovalue, c, 0, 4, 6, 7);
	
    return ntriangles;
}
