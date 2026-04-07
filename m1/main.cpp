#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <vector>
#include <string>
#include "functions.h"

GLfloat vertices[8][3] = {
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, -1.0f},
    {1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, -1.0f},
    {-1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},
    {-1.0f, -1.0f, -1.0f}
};

int edges[12][2] = {
    {0, 1}, {0, 2}, {0, 4}, 
    {1, 3}, {1, 5}, 
    {2, 3}, {2, 6}, 
    {3, 7}, 
    {4, 5}, {4, 6}, 
    {5, 7}, 
    {6, 7}
};

// triângulos das faces dos cubos
const int triangles[12][3] = {
    {0, 2, 3}, {0, 3, 1}, // baixo
    {4, 5, 7},{4, 7, 6}, // topo
    {0, 1, 5},{0, 5, 4}, // frente
    {2, 6, 7},{2, 7, 3}, // atrás
    {0, 4, 6},{0, 6, 2}, // esquerda
    {1, 3, 7},{1, 7, 5} // direita
};

std::string mode = "move";
char axis = 'x';

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Coloca a cor de background para preto e opaco
    glClearDepth(1.0f);                   // Define o buffer de profundidade para o mais distante possível
    glEnable(GL_DEPTH_TEST);              // Habilita o culling de profundidade
    glDepthFunc(GL_LEQUAL);               // Define o tipo de teste de profundidade
}

void modeSelection(unsigned char key, int x, int y){
    switch (key)
    {
    case 'm':
        mode = "move";
        break;
    case 's':
        mode = "scale";
        break;
    case 'r':
        mode = "rotate";
        break;
    default:
        break;
    }
}

void cubeOperations(int key, int x, int y)
{

    if(mode == "move"){
        switch (key)
        {
            case GLUT_KEY_RIGHT:
                move(vertices, 1.0f, 'x');
                break;
            case GLUT_KEY_LEFT:
                move(vertices, -1.0f, 'x');
                break;
            case GLUT_KEY_DOWN:
                move(vertices, -1.0f, 'y');
                break;
            case GLUT_KEY_UP:
                move(vertices, 1.0f, 'y');
                break;
            case GLUT_KEY_HOME:
                move(vertices, 1.0f, 'z');
                break;
            case GLUT_KEY_END:
                move(vertices, -1.0f, 'z');
                break;
        }
    }else if(mode == "scale"){
        switch (key)
        {
            case GLUT_KEY_RIGHT:
                scale(vertices, 1.1f, 'x');
                break;
            case GLUT_KEY_LEFT:
                scale(vertices, 0.9f, 'x');
                break;
            case GLUT_KEY_DOWN:
                scale(vertices, 0.9f, 'y');
                break;
            case GLUT_KEY_UP:
                scale(vertices, 1.1f, 'y');
                break;
            case GLUT_KEY_HOME:
                scale(vertices, 1.1f, 'z');
                break;
            case GLUT_KEY_END:
                scale(vertices, 0.9f, 'z');
                break;
            case GLUT_KEY_F1:
                scale(vertices, 1.1f, 'a');
                break;
            case GLUT_KEY_F2:
                scale(vertices, 0.9f, 'a');
                break;
        }
    }else if(mode == "rotate"){
        switch (key)
        {
            case GLUT_KEY_RIGHT:
                rotate(vertices, triangles, 15.0f, 'y');
                break;
            case GLUT_KEY_LEFT:
                rotate(vertices, triangles, -15.0f, 'y');
                break;
            case GLUT_KEY_DOWN:
                rotate(vertices, triangles, -15.0f, 'x');
                break;
            case GLUT_KEY_UP:
                rotate(vertices, triangles, -15.0f, 'x');
                break;
            case GLUT_KEY_HOME:
                rotate(vertices, triangles, 15.0f, 'z');
                break;
            case GLUT_KEY_END:
                rotate(vertices, triangles, -15.0f, 'z');
                break;
        }
    }

    glutPostRedisplay();
}

void draw(GLfloat (&vertices)[8][3])
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    for (int i = 0; i < 12; i++)
    {
        int a = edges[i][0];
        int b = edges[i][1];

        glVertex3f(vertices[a][0], vertices[a][1], vertices[a][2]);
        glVertex3f(vertices[b][0], vertices[b][1], vertices[b][2]);
    }

    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor e o de profundidade
    glMatrixMode(GL_MODELVIEW);                         // Operar na matriz de ModelView
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -6.0f);
    draw(vertices);
    glutSwapBuffers();
}

int main(int argc, char **argv)
{

    // comando para executar:
    // g++ main.cpp utils.cpp functions.cpp  -lglut -lGLU -lGL && ./a.out

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("3D Shapes");

    glClearColor(1, 1, 1, 0);

    glutDisplayFunc(display);
    glutKeyboardFunc(modeSelection);
    glutSpecialFunc(cubeOperations);
    glutReshapeFunc(reshape);
    initGL();
    glutMainLoop();
    return 0;
}