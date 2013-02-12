/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.c
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
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

		float length;
    //
    // 1. Create the orthonormal basis
    //

    // Store the incoming rotation axis in w
    w[0] = x;
    w[1] = y;
    w[2] = z;
    
    // normalize w
		// calculate the length of the vector
    length = sqrt(w[0]*w[0]) + sqrt(w[1]*w[1]) + sqrt(w[2]*w[2]);

		// calculate the new normalized values
    w[0] = w[0]/length;
    w[1] = w[1]/length;
    w[2] = w[2]/length;

    // Compute the value of t, based on w
		// Get the smallest magnitude from the normalized vector w
		printf("wvals: %lf ; %lf ; %lf\n",w[0],w[1],w[2]);
		float m = min(w[0],w[1],w[2]);
		printf("min: %lf\n",m);
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
		else if(w[2] == m){
			t[0] = w[0];
			t[1] = w[1];
			t[2] = 1;
		}

    // Compute u = t x w
    u[0] =t[1]*w[2] - w[1]*t[2];
    u[1] =t[2]*w[0] - w[2]*t[0];
    u[2] =t[0]*w[1] - w[0]*t[1];

    // Normalize u
		// calculate the length of the vector
    length = sqrt(u[0]*u[0]) + sqrt(u[1]*u[1]) + sqrt(u[2]*u[2]);

		// calculate the new normalized values
    u[0] = u[0]/length;
    u[1] = u[1]/length;
    u[2] = u[2]/length;
    
    // Compute v = w x u
    v[0] =w[1]*u[2] - u[1]*w[2];
    v[1] =w[2]*u[0] - u[2]*w[0];
    v[2] =w[0]*u[1] - u[0]*w[1];
    
    /*
    // not necessary
    // Normalize v
		// calculate the length of the vector
		length = sqrt(v[0]*v[0]) + sqrt(v[1]*v[1]) + sqrt(v[2]*v[2]);

		// calculate the new normalized values
    v[0] = v[0]/length;
    v[1] = v[1]/length;
    v[2] = v[2]/length;
    */

    // At this point u, v and w should form an orthonormal basis.
    // If your routine does not seem to work correctly it might be
    // a good idea to the check the vector values.
    
    // all looking well :)
    //printf("\nw.u: %lf\n",w[0]*u[0]+w[1]*u[1]+w[2]*u[2]);
    //printf("w.v: %lf\n",w[0]*v[0]+w[1]*v[1]+w[2]*v[2]);
    //printf("u.v: %lf\n",u[0]*v[0]+u[1]*v[1]+u[2]*v[2]);

    //
    // 2. Set up the three matrices making up the rotation
    //

    // Specify matrix A

		// translate back
    GLfloat A[16] =
    {
        u[0], u[1], u[2], 0.0,
        v[0], v[1], v[2], 0.0,
        w[0], w[1], w[2], 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    // Convert 'angle' to radians
		angle = angle * (PI/180);
		
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

		// translate to origin
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

// helper function to get the minimum value of three integers and its index
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
