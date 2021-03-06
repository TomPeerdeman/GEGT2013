/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.h
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 *
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

GLfloat *myCrossProduct(GLfloat *A, GLfloat *B, GLfloat *result);
GLfloat *myNormalizeVector(GLfloat *vector);
void myScalef(GLfloat x, GLfloat y, GLfloat z);
void myTranslatef(GLfloat x, GLfloat y, GLfloat z);
void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
float min(float a, float b, float c);


#endif
