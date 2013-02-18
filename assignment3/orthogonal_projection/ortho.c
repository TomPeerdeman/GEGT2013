/* Computer Graphics, Assignment 5, Orthogonal projection
 *
 * Filename ........ ortho.c
 * Description ..... Contains the re-programmed orthogonal projection matrix
 * Date ............ 01.09.2006
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
#include <GL/glut.h>   
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

 
#define sqr(x) ((x)*(x))

/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myOrtho(GLdouble left,
             GLdouble right,
             GLdouble bottom,
             GLdouble top,
             GLdouble near,
             GLdouble far) {
	
	// variables
	double lrplus, rlmin, btplus, tbmin, nfplus, nfmin;

	// set all variables outside the matrices
	lrplus = (left+right)/2;
	btplus = (bottom+top)/2;
	nfplus = (near+far)/2;
	rlmin = 2/(right-left);
	tbmin = 2/(top-bottom);
	nfmin = 2/(near-far);

	printf("%lf ; %lf ; %lf\n", lrplus, btplus, nfplus);
	printf("%lf ; %lf ; %lf\n", rlmin, tbmin, nfmin);

	// the first part of the orthogonal projection
	GLfloat MO1[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-lrplus, -btplus, -nfplus, 1
	};

	// second part of the orthogonal projection
	GLfloat MO2[16] = {
		rlmin, 0, 0, 0,
		0, tbmin, 0, 0,
		0, 0, nfmin, 0,
		0, 0, 0, 1
	};
	
	// apply the matrices
	glMultMatrixf(MO1);
	glMultMatrixf(MO2);
}
