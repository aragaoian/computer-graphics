#include <GL/freeglut.h>
#include "controls.h"

extern float tx;
extern float ty;
extern float scaleFactor;
extern float rx;
extern float ry;
extern bool mousePressed;
extern int lastX;
extern int lastY;

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            lastX = x;
            lastY = y;
        }
        else {
            mousePressed = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (mousePressed) {
        int dx = x - lastX;
        int dy = y - lastY;

        ry += dx * 0.5f;
        rx += dy * 0.5f;

        lastX = x;
        lastY = y;

        glutPostRedisplay();
    }
}

void mouseWheel(int button, int dir, int x, int y) {
    if (dir > 0)
        scaleFactor += 0.1f;
    else
        scaleFactor -= 0.1f;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': ty += 5.0f; break; // sobe
    case 's': ty -= 5.0f; break; // desce
    case 'a': tx -= 5.0f; break; // esquerda
    case 'd': tx += 5.0f; break; // direita

    case '+': scaleFactor += 0.1f; break; // aumenta
    case '-': scaleFactor -= 0.1f; break; // diminui

    case 'j': ry -= 5.0f; break; // gira para esquerda
    case 'l': ry += 5.0f; break; // gira para direita
    case 'i': rx -= 5.0f; break; // gira para cima
    case 'k': rx += 5.0f; break; // gira para baixo

    case '1': glDisable(GL_LIGHT0); break; // desliga a luz 0
    case '2': glDisable(GL_LIGHT1); break; // desliga a luz 1
    case '3': glDisable(GL_LIGHT2); break; // desliga a luz 2

    case 'q': glEnable(GL_LIGHT0); break; // liga a luz 0
    case 'e': glEnable(GL_LIGHT1); break; // liga a luz 1
    case 'r': glEnable(GL_LIGHT2); break; // liga a luz 2
    }

    glutPostRedisplay();
}
