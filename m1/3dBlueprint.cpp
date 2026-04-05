#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <vector>
#include <string>

GLfloat vertices[8][3] = {
    { 1.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f, -1.0f },
    { 1.0f, -1.0f,  1.0f },
    { 1.0f, -1.0f, -1.0f },
    {-1.0f,  1.0f,  1.0f },
    {-1.0f,  1.0f, -1.0f },
    {-1.0f, -1.0f,  1.0f },
    {-1.0f, -1.0f, -1.0f }
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

std::vector<std::vector<float>> xRotationMatrix(float angle) {
    std::vector<std::vector<float>> matrix = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, cos(angle), -sin(angle)},
        {0.0f, sin(angle), cos(angle)}
    };

    return matrix;
}

std::vector<std::vector<float>> yRotationMatrix(float angle) {
    std::vector<std::vector<float>> matrix = {
        {cos(angle), 0.0f, sin(angle)},
        {0.0f, 1.0f, 0.0f},
        {-sin(angle), 0.0f, cos(angle)}
    };

    return matrix;
}

std::vector<std::vector<float>> zRotationMatrix(float angle) {
    std::vector<std::vector<float>> matrix = {
        {cos(angle), -sin(angle), 0.0f},
        {sin(angle), cos(angle), 0.0f},
        {0.0f, 0.0f, 1.0f}
    };

    return matrix;
}

std::vector<float> matMul(const std::vector<std::vector<float>>& rotationMatix, const std::vector<float>& columnVector){
    std::vector<float> res(3, 0.0f); // vetor de coluna [x, y, z]

    for(int i = 0; i < 3; i++){
        float sum = 0.0f;
        for(int j = 0; j < 3; j++){
            sum += rotationMatix[i][j] * columnVector[j];
        }
        res[i] = sum;
    }
    
    return res;
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Coloca a cor de background para preto e opaco
    glClearDepth(1.0f);                   // Define o buffer de profundidade para o mais distante possível
    glEnable(GL_DEPTH_TEST);   // Habilita o culling de profundidade
    glDepthFunc(GL_LEQUAL);    // Define o tipo de teste de profundidade
}


void move(GLfloat (&vertices)[8][3], GLfloat distance, char axis) {
	for (int i = 0; i < 8; i++) {
        if (axis == 'x'){
            vertices[i][0] = vertices[i][0] + distance;
        }else if (axis == 'y'){
            vertices[i][1] = vertices[i][1] + distance;
        }else{
            vertices[i][2] = vertices[i][2] + distance;
        }
	}
}

void scale(GLfloat (&vertices)[8][3], float scaleFactor, char axis){
    /*
        * 'x', 
        * 'y', 
        * 'z' 
        * e 'a' (escalar em todos os eixos)
    */
    for (int i = 0; i < 8; i++) {
        if (axis == 'x' || axis == 'a'){
            vertices[i][0] = vertices[i][0] * scaleFactor;
        }
        if (axis == 'y' || axis == 'a'){
            vertices[i][1] = vertices[i][1] * scaleFactor;
        }
        if (axis == 'z' || axis == 'a'){
            vertices[i][2] = vertices[i][2] * scaleFactor;
        }
	}
}

void rotate(GLfloat (&vertices)[8][3], float angle, char axis){
    
}


//Função de redesenhou prioriza o aspecto da projeção
void reshape(GLsizei width, GLsizei height) {  
    if (height == 0) height = 1;                
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();             
    gluPerspective(75.0f, aspect, 0.1f, 100.0f);
}

void keyboard_special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		// move(vertices, -1.0f, 'y');
        scale(vertices, 0.9, 'a');
		break;

	case GLUT_KEY_UP:
		// move(vertices, 1.0f, 'y');
        scale(vertices, 1.1f, 'a');
		break;

	case GLUT_KEY_RIGHT:
		move(vertices, 1.0f, 'x');
		break;

	case GLUT_KEY_LEFT:
		move(vertices, -1.0f, 'x');
		break;

    case GLUT_KEY_HOME:
        move(vertices, 1.0f, 'z');
        break;

    case GLUT_KEY_END:
        move(vertices, -1.0f, 'z');
        break;    
	}

    glutPostRedisplay();
}

void draw(GLfloat (&vertices)[8][3]){
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    for(int i = 0; i < 12; i++){
        int a = edges[i][0];
        int b = edges[i][1];

        glVertex3f(vertices[a][0], vertices[a][1], vertices[a][2]);
        glVertex3f(vertices[b][0], vertices[b][1], vertices[b][2]);
    }

    glEnd();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor e o de profundidade
    glMatrixMode(GL_MODELVIEW); // Operar na matriz de ModelView
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f);
	draw(vertices);
	glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1000, 1000); 
    glutInitWindowPosition(50, 50); 

    glutCreateWindow("3D Shapes");        
    
    glClearColor(1, 1, 1, 0);

    glutDisplayFunc(display);       
    glutSpecialFunc(keyboard_special);
    glutReshapeFunc(reshape);
    initGL();                       
    glutMainLoop();                
    return 0;
}