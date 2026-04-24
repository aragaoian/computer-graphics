#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <GL/freeglut.h>

using namespace std;

vector<vector<double>> vertices;
vector<vector<double>> verticesNormals;
vector<vector<double>> verticesTexture;
vector<vector<int>> faces;

unsigned int obj;
float obj_rotation;

// TODO
// Create the loadObj and createObj functions [X]
// Adapt parseFaceCoordinate to return a vector<int>(3) []
// Create the mode manager (export from m1) []
// Add the lighting (look up the requirements in the assigment specifications) []
// Organize the project structure []


int parseFaceCoordinate(string coord, bool isNormal = false){
    /*
    The standart .obj pattern is v/vt/vn
    but this specific one is v/vn.

    Be aware that // is a valid approach to
    explicity signal that this .obj file is texture omitted.

    OBS: -1 is to transform to 0-based index.
    */
    size_t firstSlash = coord.find('/');

    // npos is used for non-existent positions
    if(firstSlash == string::npos){
        return stoi(coord) - 1;
    }

    if(!isNormal){
        return stoi(coord.substr(0, firstSlash)) - 1;
    }

    size_t lastSlash = coord.rfind('/');
    if(lastSlash == string::npos || lastSlash + 1 >= coord.size()){
        return -1;
    }

    return stoi(coord.substr(lastSlash + 1)) - 1;
}

void loadObj(string filePath = "misc/elepham.obj"){
    int read;
    ifstream file(filePath);

    if(!file.is_open()){
        cerr << "Arquivo não encontrado!" << endl;
        exit(1);
    }else{
        string type;
        while(file >> type){
            if(type == "v"){
                float x, y, z;
                file >> x >> y >> z;
                vector<double> vertice = {x, y, z};
                vertices.push_back(vertice);
            }
            
            if(type == "vn"){
                float x, y, z;
                file >> x >> y >> z;
                vector<double> verticeNormal = {x, y, z};
                verticesNormals.push_back(verticeNormal);
            }
            
            if(type == "vt"){
                float x, y, z;
                file >> x >> y >> z;
                vector<double> verticeTexture = {x, y, z};
                verticesTexture.push_back(verticeTexture);
            }
            
            if(type == "f"){
                string x, y, z;
                file >> x >> y >> z;
                vector<int> face = {
                    parseFaceCoordinate(y), 
                    parseFaceCoordinate(x), 
                    parseFaceCoordinate(z)
                };
                faces.push_back(face);
            }

        }
    }

    file.close();
}

void createObj(){
    obj = glGenLists(1);
    glPointSize(2.0);
    glNewList(obj, GL_COMPILE);

    glPushMatrix();
    glBegin(GL_LINES);

    for(vector<int> face: faces){
        glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);
        glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);

        glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);
        glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);

        glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);
        glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);
    }

    glEnd();
    glPopMatrix();
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
    glTranslatef(0, -40.00, -500);
    glColor3f(1.0, 0.23, 0.27);
    glScalef(0.4, 0.4, 0.4);
    glRotatef(obj_rotation, 0, 1, 0);
    glCallList(obj);
    glPopMatrix();
    obj_rotation = obj_rotation + 0.6;
    if (obj_rotation > 360) obj_rotation = obj_rotation - 360;
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
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
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    // glutTimerFunc(10, timer, 0);

    loadObj();
    createObj();

    glutMainLoop();
    return 0;
}