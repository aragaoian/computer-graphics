#include "utils.h"
#include <vector>
#include <math.h>
#include <GL/glut.h>

std::vector<std::vector<float>> xRotationMatrix(float angle)
{
    std::vector<std::vector<float>> matrix = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, cos(angle), -sin(angle)},
        {0.0f, sin(angle), cos(angle)}};

    return matrix;
}

std::vector<std::vector<float>> yRotationMatrix(float angle)
{
    std::vector<std::vector<float>> matrix = {
        {cos(angle), 0.0f, sin(angle)},
        {0.0f, 1.0f, 0.0f},
        {-sin(angle), 0.0f, cos(angle)}};

    return matrix;
}

std::vector<std::vector<float>> zRotationMatrix(float angle)
{
    std::vector<std::vector<float>> matrix = {
        {cos(angle), -sin(angle), 0.0f},
        {sin(angle), cos(angle), 0.0f},
        {0.0f, 0.0f, 1.0f}};

    return matrix;
}

std::vector<double> matMul(const std::vector<std::vector<float>> &rotationMatix, const std::vector<float> &columnVector)
{
    std::vector<double> res(3, 0.0f);
    for (int i = 0; i < 3; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < 3; j++)
        {
            sum += rotationMatix[i][j] * columnVector[j];
        }
        res[i] = sum;
    }
    return res;
}

double dot(const std::vector<float> &v0, const std::vector<float> &v1)
{
    return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

std::vector<float> cross(const std::vector<float> &v0, const std::vector<float> &v1)
{
    return {
        v0[1] * v1[2] - v0[2] * v1[1],
        v0[2] * v1[0] - v0[0] * v1[2],
        v0[0] * v1[1] - v0[1] * v1[0]};
}

std::vector<float> tetrahedronCentroid(const std::vector<float> &v0,
                               const std::vector<float> &v1,
                               const std::vector<float> &v2)
{
    return {
        (v0[0] + v1[0] + v2[0]) / 4.0f,
        (v0[1] + v1[1] + v2[1]) / 4.0f,
        (v0[2] + v1[2] + v2[2]) / 4.0f};
}

float volume(std::vector<float> &v0, std::vector<float> &v1, std::vector<float> &v2)
{
    return dot(v0, cross(v1, v2)) / 6.0;
}

std::vector<double> calcCentroidVec(GLfloat (&vertices)[8][3], const int triangles[12][3])
{
    double totalVol = 0.0;
    std::vector<double> sumC(3, 0.0);

    // Ponto de referência
    // É usado pra evitar degeneridade da origem
    std::vector<float> ref = {
        vertices[0][0],
        vertices[0][1],
        vertices[0][2]};

    for (int i = 0; i < 12; i++)
    {
        int t0 = triangles[i][0];
        int t1 = triangles[i][1];
        int t2 = triangles[i][2];

        std::vector<float> v0 = {
            vertices[t0][0] - ref[0],
            vertices[t0][1] - ref[1],
            vertices[t0][2] - ref[2]};

        std::vector<float> v1 = {
            vertices[t1][0] - ref[0],
            vertices[t1][1] - ref[1],
            vertices[t1][2] - ref[2]};

        std::vector<float> v2 = {
            vertices[t2][0] - ref[0],
            vertices[t2][1] - ref[1],
            vertices[t2][2] - ref[2]};

        double vol = volume(v0, v1, v2);
        totalVol += vol;

        std::vector<float> c = tetrahedronCentroid(v0, v1, v2);

        for (int j = 0; j < 3; j++)
        {
            sumC[j] += c[j] * vol;
        }
    }

    std::vector<double> result(3, 0.0);

    if (totalVol != 0.0)
    {
        for (int j = 0; j < 3; j++)
        {
            result[j] = (sumC[j] / totalVol) + ref[j];
        }
    }

    return result;
}

double calcCentroid(GLfloat (&vertices)[8][3], const int triangles[12][3], char axis)
{
    std::vector<double> c = calcCentroidVec(vertices, triangles);
    if (axis == 'x')
        return c[0];
    if (axis == 'y')
        return c[1];
    return c[2];
}
