/* Computer Graphics, Assignment 4, Positioning the camera
 *
 * Filename ........ lookat.c
 * Description ..... Contains the re-programmed lookAt function
 * Created by ...... Jurgen Sturm 
 *
 * Student names: Tom Peerdeman & Rene Aparicio Saez
 * Student emails: tom.peerdeman@student.uva.nl & rene.apariciosaez@student.uva.nl
 * Collegekaart 10266186 & 10214054
 * Date 13 Feb 2013
 *
 */
#include <GL/glut.h>   
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
 
/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

// Calculate the crossproduct of A x B where A and B are vectors in R3
GLdouble *crossProduct(GLdouble *A, GLdouble *B, GLdouble *result) {
	//a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1
	result[0] = A[1]*B[2] - A[2]*B[1];
	result[1] = A[2]*B[0] - A[0]*B[2];
	result[2] = A[0]*B[1] - A[1]*B[0];
	
	return result;
}

// Normalize a vector in R3
GLdouble *normalizeVector(GLdouble *vector) {
	double length = sqrt(vector[0] * vector[0]
	                    + vector[1] * vector[1]
	                    + vector[2] * vector[2]);
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	
	return vector;
}

// Calculate the inproduct of vector A and B in R3
GLdouble inProduct(GLdouble *A, GLdouble *B) {
	return A[0]*B[0] + A[1]*B[1] + A[2]*B[2];
}

void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
	          GLdouble centerX, GLdouble centerY, GLdouble centerZ,
	          GLdouble upX, GLdouble upY, GLdouble upZ) {
	
	GLdouble cz[3];
	GLdouble cy[3];
	GLdouble cx[3];
	
	GLdouble up[3] = {upX, upY, upZ};
	GLdouble minEye[3] = {-eyeX, -eyeY, -eyeZ};
	
	// cz is the vector Pcamera to Plookat
	cz[0] = eyeX - centerX;
	cz[1] = eyeY - centerY;
	cz[2] = eyeZ - centerZ;
	
	// Normalize cz
	normalizeVector(cz);
	
	// cx = up x cz
	crossProduct(up, cz, cx);
	
	// Normalize cy
	normalizeVector(cx);
	
	// cy = cz x cx
	crossProduct(cz, cx, cy);

	GLfloat RT[16] = {
		cx[0], cy[0], cz[0], 0,
		cx[1], cy[1], cz[1], 0,
		cx[2], cy[2], cz[2], 0,
		0    , 0    , 0    , 1
	};
	
	glTranslatef(inProduct(minEye, cx), inProduct(minEye, cy), 
			inProduct(minEye, cz));
	glMultMatrixf(RT);
}
