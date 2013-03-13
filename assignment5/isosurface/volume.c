/* Computer Graphics, Assignment, Volume rendering with cubes/points/isosurface
 *
 * Student names: Tom Peerdeman & Rene Aparicio Saez
 * Student emails: tom.peerdeman@student.uva.nl & rene.apariciosaez@student.uva.nl
 * Collegekaart 10266186 & 10214054
 * Date 13 March 2013
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "volume.h"

/* The voxels of the volume dataset, stored as a one-dimensional array */
unsigned char   *volume;

/* The dimensions of the volume dataset */
int     nx, ny, nz;

/* The size of a voxel */
float   sizex, sizey, sizez;

/* Utility function to convert the index of a voxel
   into an index in the volume array above */
int
voxel2idx(int i, int j, int k)
{
    return (k*ny + j)*nx + i;
}

/* Extract a cell from the volume, so that datapoint 0 of the
   cell corresponds to voxel (i, j, k), datapoint 1 to voxel (i+1, j, k),
   etc. See the assignment. */
cell
get_cell(int i, int j, int k)
{
    cell c;
    c.p[0] = v3_create(i,j,k);
    c.value[0] = (double)volume[voxel2idx(i,j,k)];
    c.p[1] = v3_create(i+1,j,k);
    c.value[1] = (double)volume[voxel2idx(i+1,j,k)];
    c.p[2] = v3_create(i,j+1,k);
    c.value[2] = (double)volume[voxel2idx(i,j+1,k)];
    c.p[3] = v3_create(i+1,j+1,k);
    c.value[3] = (double)volume[voxel2idx(i+1,j+1,k)];
    c.p[4] = v3_create(i,j,k+1);
    c.value[4] = (double)volume[voxel2idx(i,j,k+1)];
    c.p[5] = v3_create(i+1,j,k+1);
    c.value[5] = (double)volume[voxel2idx(i+1,j,k+1)];
    c.p[6] = v3_create(i,j+1,k+1);
    c.value[6] = (double)volume[voxel2idx(i,j+1,k+1)];
    c.p[7] = v3_create(i+1,j+1,k+1);
    c.value[7] = (double)volume[voxel2idx(i+1,j+1,k+1)];
    
    return c;
}

/* Utility function to read a volume dataset from a VTK file.
   This will store the data in the "volume" array and update the dimension
   and size values. */

void
read_volume(const char *fname)
{
    FILE *f;
    char s[256];
    int nvoxels;

    printf("Reading %s\n", fname);
    f = fopen(fname, "rb");

    if (!f)
    {
        fprintf(stderr, "read_volume(): Could not open file '%s' for reading!\n", fname);
        exit(-1);
    }
	
	char *ignore;
	int ignore2;

    // header line
    ignore = fgets(s, 255, f);

    // comment line
    ignore = fgets(s, 255, f);

    // BINARY
    ignore = fgets(s, 255, f);

    // DATASET STRUCTURED_POINTS
    ignore =  fgets(s, 255, f);

    // DIMENSIONS %d %d %d
    ignore2 = fscanf(f, "%s %d %d %d\n", s, &nx, &ny, &nz);
    printf("%d x %d x %d voxels\n", nx, ny, nz);

    // ASPECT_RATIO/SPACING %f %f %f
    ignore2 = fscanf(f, "%s %f %f %f\n", s, &sizex, &sizey, &sizez);
    printf("voxel sizes: %.3f, %.3f, %.3f\n", sizex, sizey, sizez);

    // ORIGIN ...
    ignore = fgets(s, 255, f);

    // POINT_DATA ...
     ignore = fgets(s, 255, f);

    // SCALARS ...
     ignore = fgets(s, 255, f);

    // LOOKUP_TABLE ...
     ignore = fgets(s, 255, f);

    // allocate memory to hold the volume data and read it from file
    nvoxels = nx * ny * nz;
    volume = (unsigned char*)malloc(nvoxels);

    if (fread(volume, 1, nvoxels, f) < (size_t)nvoxels)
    {
        printf("WARNING: not all data could be read!\n");
    }
	
	(void)ignore;
	(void)ignore2;

    fclose(f);
}
