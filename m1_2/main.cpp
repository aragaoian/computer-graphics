#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <GL/freeglut.h>
#include "lights.h"
#include "obj.h"
#include "controls.h"

// Executar com
// g++ main.cpp controls.cpp lights.cpp obj.cpp -lglut -lGL -lGLU && ./a.out

using namespace std;

// Substituir pelo caminho do seu arquivo
// NOTE
// .objs funcionais
// elepham.obj; mba1.obj; teddy.obj
#define FILEPATH "misc/mba1.obj"

vector<vector<double>> vertices;
vector<vector<double>> verticesNormals;
vector<vector<double>> verticesTexture;
vector<vector<FaceIndex>> faces;

float tx = 0, ty = 0, tz = 0;
float scaleFactor = 1.0;
float rx = 0, ry = 0;
bool mousePressed = false;
int lastX, lastY;

unsigned int obj;
float objRotation;


void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

void drawObj()
{
    glPushMatrix();

    // Ações controladas pelo usuário
    glTranslatef(tx, ty - 40.0f, -500.0f + tz);
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    glRotatef(rx, 1.0f, 0.0f, 0.0f);
    glRotatef(ry, 0.0f, 1.0f, 0.0f);

    // Rotação automática
    // só funciona se habilitar o glutTimerFunc()
    glRotatef(objRotation, 0.0f, 1.0f, 0.0f);

    glColor3f(1.0f, 0.23f, 0.27f);
    glCallList(obj);
    glPopMatrix();

    objRotation += 0.6f;
    if (objRotation > 360.0f)
        objRotation -= 360.0f;
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Configura a câmera para uma visão 3D
    gluLookAt(
        0, 0, 200,
        0, 0, 0,
        0, 1, 0
    );

    GLfloat pos0[] = { 0, 100, 100, 1 };
    GLfloat pos1[] = { 100, 0, 100, 1 };
    GLfloat pos2[] = { -100, 0, 100, 1 };

    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);

    drawObj();

    glutSwapBuffers();
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Carregar OBJ");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    setupLights();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutMouseWheelFunc(mouseWheel);
    // glutTimerFunc(10, timer, 0);

    loadObj(FILEPATH);
    createObj();

    glutMainLoop();
    return 0;
}