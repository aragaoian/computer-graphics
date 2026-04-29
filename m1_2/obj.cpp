#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include "obj.h"

extern std::vector<std::vector<double>> vertices;
extern std::vector<std::vector<double>> verticesNormals;
extern std::vector<std::vector<double>> verticesTexture;
extern std::vector<std::vector<FaceIndex>> faces;

extern unsigned int obj;

/*
A leitura do .obj só funcionará corretamente
se não houverem indíces negativos (não tratativa para).
E.g radar.obj
*/
FaceIndex parseFace(std::string coord) {
    FaceIndex f = {-1, -1, -1};

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

void loadObj(std::string filePath) {
    int read;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Arquivo não encontrado!" << std::endl;
        exit(1);
    }
    else {
        std::string type;
        while (file >> type) {
            if (type == "v") {
                float x, y, z;
                file >> x >> y >> z;
                std::vector<double> vertice = { x, y, z };
                vertices.push_back(vertice);
            }

            if (type == "vn") {
                float x, y, z;
                file >> x >> y >> z;
                std::vector<double> verticeNormal = { x, y, z };
                verticesNormals.push_back(verticeNormal);
            }

            if (type == "vt") {
                std::string line;
                std::getline(file, line);
                std::istringstream iss(line);
                float x = 0.0f, y = 0.0f, z = 0.0f;
                if (!(iss >> x >> y)) continue;
                iss >> z;
                verticesTexture.push_back({ (double)x, (double)y, (double)z });
            }

            if (type == "f") {
                std::string x, y, z;
                file >> x >> y >> z;
                std::vector<FaceIndex> face = {
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
    //cada face é um triângulo
    // então usamos GL_TRIANGLES
    glBegin(GL_TRIANGLES); 

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

