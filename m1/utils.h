#include <vector>
#include <GL/glut.h>

#ifndef VECTOROPS_H
#define VECTOROPS_H

std::vector<std::vector<float>> xRotationMatrix(float angle);

std::vector<std::vector<float>> yRotationMatrix(float angle);

std::vector<std::vector<float>> zRotationMatrix(float angle);

std::vector<double> matMul(const std::vector<std::vector<float>> &rotationMatix, const std::vector<float> &columnVector);

double dot(const std::vector<float> &v0, const std::vector<float> &v1);

std::vector<float> cross(const std::vector<float> &v0, const std::vector<float> &v1);

std::vector<float> tetrahedronCentroid(const std::vector<float> &v0, const std::vector<float> &v1, const std::vector<float> &v2);

float volume(std::vector<float> &v0, std::vector<float> &v1, std::vector<float> &v2);

std::vector<double> calcCentroidVec(GLfloat (&vertices)[8][3], const int triangles[12][3]);

double calcCentroid(GLfloat (&vertices)[8][3], const int triangles[12][3], char axis);

#endif