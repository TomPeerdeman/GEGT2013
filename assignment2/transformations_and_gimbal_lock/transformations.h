/* Computer Graphics, Assignment, Translations, Rotations and Scaling
 *
 * Filename ........ transformations.h
 * Description ..... Contains the re-programmed translation, rotation and scaling functions
 *
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

// pi
#define PI 3.14159265359

void myScalef(GLfloat x, GLfloat y, GLfloat z);
void myTranslatef(GLfloat x, GLfloat y, GLfloat z);
void myRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
float min(float a, float b, float c);


#endif
