#include <GL/freeglut.h>
#include "lights.h"

#define ATTENUATION 1.0f

void setupLights() {
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    
    GLfloat diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
    
    // TODO
    // Verificar com e sem atenuação
    // 1.0f -> sem atenuação
    // > 1.0f -> com atenuação
    
    // NOTE
    // Atenuação é uma forma de simular o comportamento
    // realista da luz, de forma que pontos mais distantes
    // tenham menos brilhos que pontos mais pertos da câmera.
    // Nem sempre vai produzir um resultado "realista".
    // Achei que ficou melhor com atenuação 5.0f do que sem
    // (menos estourado, mas que pode ser por conta do shininess)

    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, ATTENUATION);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, ATTENUATION);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, ATTENUATION);

    // Habilita material baseado na cor
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Material especular (brilho no objeto)
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glShadeModel(GL_SMOOTH);
}
