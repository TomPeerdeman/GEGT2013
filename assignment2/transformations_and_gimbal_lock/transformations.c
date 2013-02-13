/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.c
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 *
 * Student names: Tom Peerdeman & Rene Aparicio Saez
 * Student emails: tom.peerdeman@student.uva.nl & rene.apariciosaez@student.uva.nl
 * Collegekaart 10266186 & 10214054
 * Date 12 Feb 2013
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include "transformations.h"

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

// Calculate the crossproduct of A x B where A and B are vectors in R3
GLfloat *myCrossProduct(GLfloat *A, GLfloat *B, GLfloat *result) {
	//a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1
	result[0] = A[1]*B[2] - A[2]*B[1];
	result[1] = A[2]*B[0] - A[0]*B[2];
	result[2] = A[0]*B[1] - A[1]*B[0];
	
	return result;
}

// Normalize a vector in R3
GLfloat *myNormalizeVector(GLfloat *vector) {
	// calculate the length
	float length = sqrt(vector[0] * vector[0]
	                    + vector[1] * vector[1]
	                    + vector[2] * vector[2]);
	                    
	// calculate the new normalized vector
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	
	return vector;
}


// scaling by setting x,y,z in the diagonals
void myScalef(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat M[16] =
    {
        x, 0.0, 0.0, 0.0,
        0.0, y, 0.0, 0.0,
        0.0, 0.0, z, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    glMultMatrixf(M);
}

// translating by setting the new x,y,z coordinates
// NOTE: the model-view matrix M has its rows and columns swapped
void myTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat M[16] =
    {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x, y, z, 1.0
    };

    glMultMatrixf(M);
}

void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat u[3], v[3], w[3], t[3];
		float m;
		
    //
    // 1. Create the orthonormal basis
    //

    // Store the incoming rotation axis in w
    w[0] = x;
    w[1] = y;
    w[2] = z;
    
    // normalize w
    myNormalizeVector(w);

    // Compute the value of t, based on w
		// Get the smallest magnitude from the normalized vector w
		m = min(w[0],w[1],w[2]);
		if(w[0] == m){
			t[0] = 1;
			t[1] = w[1];
			t[2] = w[2];
		}
		else if(w[1] == m){
			t[0] = w[0];
			t[1] = 1;
			t[2] = w[2];
		}
		else{
			t[0] = w[0];
			t[1] = w[1];
			t[2] = 1;
		}

    // Compute u = t x w
		// get the crossproduct
    myCrossProduct(t, w, u);

    // Normalize u
		// calculate the length of the vector
		myNormalizeVector(u);
    
    // Compute v = w x u
    // get the crossproduct
    myCrossProduct(w, u, v);
		
		// NOTE: because w and u are normalized, v is also normalized.

    // At this point u, v and w should form an orthonormal basis.
    // If your routine does not seem to work correctly it might be
    // a good idea to the check the vector values.
    
    /* 
     * all looking well, they all return 0
     * printf("\nw.u: %lf\n",w[0]*u[0]+w[1]*u[1]+w[2]*u[2]);
     * printf("w.v: %lf\n",w[0]*v[0]+w[1]*v[1]+w[2]*v[2]);
     * printf("u.v: %lf\n",u[0]*v[0]+u[1]*v[1]+u[2]*v[2]);
		 */
		 
    //
    // 2. Set up the three matrices making up the rotation
    //

    // Specify matrix A

    GLfloat A[16] =
    {
        u[0], u[1], u[2], 0.0,
        v[0], v[1], v[2], 0.0,
        w[0], w[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Convert 'angle' to radians
		angle = angle * (M_PI/180);
		
    // Specify matrix B

		// rotate a given angle
    GLfloat B[16] =
    {
        cos(angle), sin(angle), 0.0, 0.0,
        -sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Specify matrix C

    GLfloat C[16] =
    {
        u[0], v[0], w[0], 0.0,
        u[1], v[1], w[1], 0.0,
        u[2], v[2], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    //
    // 3. Apply the matrices to get the combined rotation
    //

    glMultMatrixf(A);
    glMultMatrixf(B);
    glMultMatrixf(C);
}

// helper function to get the minimum value of three values
// source: https://github.com/TomPeerdeman/GEGT2013/blob/master/assignment1/triangle_rasterization/trirast.c
// made by: Tom Peerdeman & Rene Aparicio Saez
float min(float a, float b, float c) {
	float m = a;
	if(b < m)
		m = b;
	if(c < m)
		m = c;
	return m;
}
