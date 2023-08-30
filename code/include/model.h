#include <raylib.h>
#include <iostream>
#include <vector>
#include "ezdungeon.h"

#ifndef MODEL_H
#define MODEL_H

Mesh GenMeshCustom(void);

void generateVertex(int x,int y,int z,std::vector<int>* , std::vector<float> *, int *** ,unsigned int * );

void generateFace(int x,int y,int z,std::vector<int>* indices, std::vector<float> *vertices,std::vector<float> *normals,std::vector<float> *texCoords,int dir);

Mesh meshFromDungeon(dungeonMatrix *);

#endif