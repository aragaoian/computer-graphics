#include <GL/glut.h>
#include <math.h>
#include <vector>

GLfloat vertices[6][4][3] = {
    { {1.0f, 1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}, {-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} },
    { {1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f} },
    { {1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, 1.0f} },
    { {1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, -1.0f} },
    { {-1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f} },
    { {1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, -1.0f} }
};

GLfloat colors[6][3] = {
    {0.0f, 1.0f, 0.0f},
    {5.0f, 3.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f}
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


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor e o de profundidade
    glMatrixMode(GL_MODELVIEW);     //Operar na matriz de ModelView

    // Renderiza um cubo com 6 quads diferentes
    glLoadIdentity();                 // Reseta para a matriz identidade
    glTranslatef(0.0f, 0.0f, -7.0f);  // Move para a direta da view o que será desenhado

    glBegin(GL_QUADS);                // Começa a desenhar o cubo
    for (int face = 0; face < 6; face++) {
        glColor3f(colors[face][0], colors[face][1], colors[face][2]);
        for (int vertex = 0; vertex < 4; vertex++) {
            glVertex3f(vertices[face][vertex][0],
                       vertices[face][vertex][1],
                       vertices[face][vertex][2]);
        }
    }
    glEnd();  

    glutSwapBuffers();  // Double Buffer, troca o atual pelo que está aguardando
}

void move(GLfloat (&vertices)[6][4][3], int distance, float angle) {
	for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            std::vector<float> p = {vertices[i][j][0], vertices[i][j][1], vertices[i][j][2]};
            std::vector<float> newCoordinates = matMul(xRotationMatrix(angle), p); 
            // Implementar o resto
        }
	}
}


//Função de redesenhou prioriza o aspecto da projeção
void reshape(GLsizei width, GLsizei height) {  
    if (height == 0) height = 1;                
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();             
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE); // Double buffer

    glutInitWindowSize(640, 480); 
    glutInitWindowPosition(50, 50); 

    glutCreateWindow("3D Shapes");          

    glutDisplayFunc(display);       
    glutReshapeFunc(reshape);
    initGL();                       
    glutMainLoop();                
    return 0;
}