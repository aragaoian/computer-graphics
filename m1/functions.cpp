#include "functions.h"
#include <vector>
#include "utils.h"
#include <GL/glut.h>

void move(GLfloat (&vertices)[8][3], GLfloat distance, char axis)
{
    for (int i = 0; i < 8; i++)
    {
        if (axis == 'x')
        {
            vertices[i][0] = vertices[i][0] + distance;
        }
        else if (axis == 'y')
        {
            vertices[i][1] = vertices[i][1] + distance;
        }
        else
        {
            vertices[i][2] = vertices[i][2] + distance;
        }
    }
}

void scale(GLfloat (&vertices)[8][3], float scaleFactor, char axis)
{
    /*
     * 'x' para o eixo X,
     * 'y' para o eixo Y,
     * 'z' para o eixo Z
     * e 'a' (escalar em todos os eixos)
     */
    
    for (int i = 0; i < 8; i++)
    {
        if (axis == 'x' || axis == 'a')
        {
            vertices[i][0] = vertices[i][0] * scaleFactor;
        }
        if (axis == 'y' || axis == 'a')
        {
            vertices[i][1] = vertices[i][1] * scaleFactor;
        }
        if (axis == 'z' || axis == 'a')
        {
            vertices[i][2] = vertices[i][2] * scaleFactor;
        }
    }
}

void rotate(GLfloat (&vertices)[8][3], const int triangles[12][3],float angle, char axis)
{

    std::vector<double> c = calcCentroidVec(vertices, triangles);

    float radians = angle * (3.14159265f / 180.0f);

    for (int i = 0; i < 8; i++)
    {

        float x = vertices[i][0] - c[0];
        float y = vertices[i][1] - c[1];
        float z = vertices[i][2] - c[2];

        std::vector<float> v = {x, y, z};
        std::vector<double> r;

        if (axis == 'x')
            r = matMul(xRotationMatrix(radians), v);
        if (axis == 'y')
            r = matMul(yRotationMatrix(radians), v);
        if (axis == 'z')
            r = matMul(zRotationMatrix(radians), v);

        vertices[i][0] = r[0] + c[0];
        vertices[i][1] = r[1] + c[1];
        vertices[i][2] = r[2] + c[2];
    }
}

// Função de redesenhou prioriza o aspecto da projeção
void reshape(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0f, aspect, 0.1f, 100.0f);
}