/* Computer Graphics, Assignment 4, Positioning the camera
 *
 * Filename ........ lookat.c
 * Description ..... Contains the re-programmed lookAt function
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
#include <math.h>

 
/* ANSI C/ISO C89 does not specify this constant (?) */
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

void myTranslatef(GLfloat x, GLfloat y, GLfloat z){
    GLfloat M[16] =
    {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x, y, z, 1.0
    };

    glMultMatrixf(M);
}

// Calculate the crossproduct of A x B where A and B are vectors in R3
GLfloat *crossProduct(GLfloat *A, GLfloat *B, GLfloat *result) {
	result[0] = A[1]*B[2] - A[2]*B[1];
	result[1] = A[2]*B[0] - A[0]*B[2];
	result[2] = A[0]*B[1] - A[1]*B[0];
	
	return result;
}

// Normalize a vector in R3
GLfloat *normalizeVector(GLfloat *vector) {
	float length = sqrt(vector[0] * vector[0]
	                    + vector[1] * vector[1]
	                    + vector[2] * vector[2]);
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	
	return vector;
}

void myLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
	          GLdouble centerX, GLdouble centerY, GLdouble centerZ,
	          GLdouble upX, GLdouble upY, GLdouble upZ) {
	//glTranslatef(-eyeX, -eyeY, -eyeZ);
	
	GLfloat cz[3];
	GLfloat cy[3];
	GLfloat cx[3];
	
	GLfloat up[3] = {upX, upY, upZ};
	
	// cz is the vector Pcamera to Plookat
	cz[0] = centerX - eyeX;
	cz[1] = centerY - eyeY;
	cz[2] = centerZ - eyeZ;
	
	// Normalize cz
	normalizeVector(cz);
	
	// cy = up x cz
	crossProduct(up, cz, cy);
	
	// Normalize cy
	normalizeVector(cy);
	
	// cx = cy x cz
	crossProduct(cy, cz, cx);
	
	GLfloat RT[16] = {
		cx[0], cx[1], -cx[2], 0,
		cy[0], cy[1], -cy[2], 0,
		cz[0], cz[1], -cz[2], 0,
		0    , 0    , 0     , 1
	};
	
	glMultMatrixf(RT);
}
