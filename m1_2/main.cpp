#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <GL/freeglut.h>

using namespace std;

// Estrutura que representa um índice de face no arquivo .OBJ.
// Cada vértice de uma face pode conter:
// v  -> índice do vértice (posição)
// vt -> índice da coordenada de textura
// vn -> índice da normal
// O padrão do .obj é: v/vt/vn
// Esses índices são usados para acessar os vetores de vértices, normais e texturas.
struct FaceIndex {
    int v;
    int vt;
    int vn;
};

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
float obj_rotation;

// TODO
// Create the loadObj and createObj functions [X]
// Adapt parseFaceCoordinate to return a vector<int>(3) []
// Create the mode manager (export from m1) []
// Add the lighting (look up the requirements in the assigment specifications) []
// Organize the project structure []


FaceIndex parseFace(string coord) {
    FaceIndex f = { -1, -1, -1 };

    int first = coord.find('/');
    int last = coord.rfind('/');

    if (first == -1) {
        f.v = stoi(coord) - 1;
        return f;
    }

    f.v = stoi(coord.substr(0, first)) - 1;

    if (first != last) {
        if (last > first + 1)
            f.vt = stoi(coord.substr(first + 1, last - first - 1)) - 1;

        if (last + 1 < coord.size())
            f.vn = stoi(coord.substr(last + 1)) - 1;
    }

    return f;
}

void setupLights() {
    // Ativa o sistema de iluminação
    glEnable(GL_LIGHTING);

    // Ativa as 3 luzes
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    // Componentes das luzes
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // luz difusa
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // brilho
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // luz ambiente

    // Aplica os componentes para cada luz
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);

    // Atenuação (opcional, melhora realismo)
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);

    // Habilita material baseado na cor
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Material especular (brilho no objeto)
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Suaviza iluminação
    glShadeModel(GL_SMOOTH);
}

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

//Rotação do mouse
void motion(int x, int y) {
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
        scaleFactor += 0.1;
    else
        scaleFactor -= 0.1;

    glutPostRedisplay();
}

//Eventos de teclado para controlar a posição, escala, rotação e iluminação do objeto
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': ty += 5; break; //sobe
    case 's': ty -= 5; break; //desce
    case 'a': tx -= 5; break; //esquerda
    case 'd': tx += 5; break; //direita

    case '+': scaleFactor += 0.1; break; //aumenta
    case '-': scaleFactor -= 0.1; break; //diminui

    case 'j': ry -= 5; break; //gira para esquerda
    case 'l': ry += 5; break; //gira para direita
    case 'i': rx -= 5; break; //gira para cima
    case 'k': rx += 5; break; //gira para baixo

    case '1': glDisable(GL_LIGHT0); break; // desliga a luz 0
    case '2': glDisable(GL_LIGHT1); break; // desliga a luz 1
    case '3': glDisable(GL_LIGHT2); break; // desliga a luz 2

    case 'q': glEnable(GL_LIGHT0); break; // liga a luz 0
    case 'e': glEnable(GL_LIGHT1); break; // liga a luz 1
    case 'r': glEnable(GL_LIGHT2); break; // liga a luz 2
    }

    glutPostRedisplay();
}

void loadObj(string filePath = "misc/elepham.obj") {
    int read;
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Arquivo não encontrado!" << endl;
        exit(1);
    }
    else {
        string type;
        while (file >> type) {
            if (type == "v") {
                float x, y, z;
                file >> x >> y >> z;
                vector<double> vertice = { x, y, z };
                vertices.push_back(vertice);
            }

            if (type == "vn") {
                float x, y, z;
                file >> x >> y >> z;
                vector<double> verticeNormal = { x, y, z };
                verticesNormals.push_back(verticeNormal);
            }

            if (type == "vt") {
                float x, y, z;
                file >> x >> y >> z;
                vector<double> verticeTexture = { x, y, z };
                verticesTexture.push_back(verticeTexture);
            }

            if (type == "f") {
                string x, y, z;
                file >> x >> y >> z;
                // Cada face é composta por 3 vértices (triângulo), e cada vértice pode conter índices para posição, textura e normal.
                vector<FaceIndex> face = {
                    parseFace(x),
                    parseFace(y),
                    parseFace(z)
                };
                faces.push_back(face);
            }

        }
    }

    file.close();
}

void createObj() {
    obj = glGenLists(1);
    glNewList(obj, GL_COMPILE);
    glBegin(GL_TRIANGLES); //cada face é um triângulo, então usamos GL_TRIANGLES

    for (auto face : faces) {
        for (auto idx : face) {
            if (idx.vn >= 0) {
                glNormal3f(
                    verticesNormals[idx.vn][0],
                    verticesNormals[idx.vn][1],
                    verticesNormals[idx.vn][2]
                );
            }

            glVertex3f(
                vertices[idx.v][0],
                vertices[idx.v][1],
                vertices[idx.v][2]
            );
        }
    }
    glEnd();
    glEndList();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

void drawElephant()
{
    glPushMatrix();

    // Translação (posição inicial + controle do usuário)
    glTranslatef(tx, ty - 40.0f, -500.0f + tz);

    // Escala (controle do usuário)
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Rotações controladas
    glRotatef(rx, 1.0f, 0.0f, 0.0f); // eixo X
    glRotatef(ry, 0.0f, 1.0f, 0.0f); // eixo Y

    // Rotação automática (animação contínua)
    glRotatef(obj_rotation, 0.0f, 1.0f, 0.0f);

    // Cor do objeto (usada junto com iluminação)
    glColor3f(1.0f, 0.23f, 0.27f);

    // Desenha o objeto
    glCallList(obj);

    glPopMatrix();

    // Atualiza rotação automática
    obj_rotation += 0.6f;
    if (obj_rotation > 360.0f)
        obj_rotation -= 360.0f;
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Configura a câmera para uma visão 3D adequada
    gluLookAt(
        0, 0, 200,   // posição da câmera
        0, 0, 0,     // para onde olha
        0, 1, 0      // vetor up
    );

    // Posiciona as luzes (3 luzes em posições diferentes para melhor iluminação)
    GLfloat pos0[] = { 0, 100, 100, 1 };
    GLfloat pos1[] = { 100, 0, 100, 1 };
    GLfloat pos2[] = { -100, 0, 100, 1 };

    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);

    // Desenha o elefante usando a lista de exibição criada
    drawElephant();

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
    glutMotionFunc(motion);
    glutMouseWheelFunc(mouseWheel);
    //glutTimerFunc(10, timer, 0);

    loadObj();
    createObj();

    glutMainLoop();
    return 0;
}