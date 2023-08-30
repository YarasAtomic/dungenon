#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <vector>
#include "ezdungeon.h"
#include <iostream>

extern "C" {


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
struct dungeonMatrix *  genDungeon(int args,const char**argv)
{
    const int size = 2;

    struct dungeonMatrix* dungeon = static_cast<struct dungeonMatrix*>(malloc(sizeof(struct dungeonMatrix)));

    unsigned int *** data = (unsigned int ***)malloc(size * sizeof(unsigned int **));
    for(int i = 0 ; i < size; i++)
    {
        data[i] = (unsigned int **)malloc(size * sizeof(unsigned int *));
        for(int j = 0 ; j < size; j++)
            data[i][j] = (unsigned int *)malloc(size * sizeof(unsigned int));
    }

    dungeon->data = data;
    dungeon->size_x = size;
    dungeon->size_y = size;
    dungeon->size_z = size;

    for(int i = 0 ; i < size; i++)
        for(int j = 0 ; j < size; j++)
            for(int k = 0 ; k < size; k++)
            {
                data[i][j][k] = DUN_PXY_WALL;
                data[i][j][k] |= DUN_PXZ_WALL;
                data[i][j][k] |= DUN_PYZ_WALL;
            }
                

    dungeon->err = 0;

    return dungeon;
};

}
