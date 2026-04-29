#ifndef OBJFUNCTIONS_H
#define OBJFUNCTIONS_H

#include <string>
#include <vector>

/*
    Estrutura que representa um índice de face no arquivo .OBJ.
    Cada vértice de uma face pode conter:
    v  -> índice do vértice (posição)
    vt -> índice da coordenada de textura
    vn -> índice da normal
    O padrão do .obj é: v/vt/vn
    Esses índices são usados para acessar os vetores de vértices, normais e texturas.
*/
struct FaceIndex
{
    int v;
    int vt;
    int vn;
};

FaceIndex parseFace(std::string coord);

void loadObj(std::string filePath);

void createObj();

#endif
