#include <raylib.h>
#include <string>
#include <dlfcn.h>
#include "model.h"
#include "network.h"
#include "gui.h"

extern "C"{
    void resizeCanvas(int width, int height);
}

void raylib();

int openModule(std::string,dungeonMatrix *);

void freeDungeon(struct dungeonMatrix *);

void cpyDungeon(dungeonMatrix **, dungeonMatrix *);