#define RLIGHTS_IMPLEMENTATION
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <dlfcn.h>
#include "model.h"
#include "network.h"
#include "gui.h"
#include "rlights.h"

#ifndef __EMSCRIPTEN__
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

extern "C"{
    void resizeCanvas(int width, int height);
}

void raylib();

int openModule(std::string,dungeonMatrix *);