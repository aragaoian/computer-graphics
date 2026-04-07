#include <GL/glut.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void move(GLfloat (&vertices)[8][3], GLfloat distance, char axis);

void scale(GLfloat (&vertices)[8][3], float scaleFactor, char axis);

void rotate(GLfloat (&vertices)[8][3], const int triangles[12][3],float angle, char axis);

void reshape(GLsizei width, GLsizei height);

#endif