#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include "network.h"
#endif
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <dlfcn.h>

#include <vector>
#include <string>
#include <raylib.h>

#define RLIGHTS_IMPLEMENTATION
// #include <rlights.h>

#include "ezdungeon.h"

int openModule(std::string );

std::vector<std::string> getModules();

void drawDungeon2D(struct dungeonMatrix * ,int ,int ,int ,int );

void generateVertex(int ,int ,int ,std::vector<int> *,std::vector<float> * , int *** ,unsigned int * );

Mesh meshFromdungeon(struct dungeonMatrix *);