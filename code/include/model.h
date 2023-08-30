#include <raylib.h>
#include <iostream>
#include <vector>
#include "ezdungeon.h"

#ifndef MODEL_H
#define MODEL_H

void generateVertex(int x,int y,int z,std::vector<int>* , std::vector<float> *, int *** ,unsigned int * );

Mesh meshFromDungeon(dungeonMatrix *);

#endif