#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <vector>
#include "ezdungeon.h"
#include <iostream>
#include <random>
#include <ctime>

struct room
{
    unsigned int id = -1;
    unsigned int sizeX;
    unsigned int sizeY;
    unsigned int sizeZ;

    unsigned int posX;
    unsigned int posY;
    unsigned int posZ;
};

const int minRoomWidth = 3;
const int maxRoomWidth = 7;
const int minRoomHeight = 1;
const int maxRoomHeight = 3;

const int sizeX = 30;
const int sizeY = 10;
const int sizeZ = 30;

room genRoom(utilityMatrix * matrix, unsigned int posX,unsigned int posY,unsigned int posZ,int id)
{
    unsigned int sizeX = (std::rand() % (maxRoomWidth-minRoomWidth)) + minRoomWidth;
    unsigned int sizeY = (std::rand() % (maxRoomHeight-minRoomHeight)) + minRoomHeight;
    unsigned int sizeZ = (std::rand() % (maxRoomWidth-minRoomWidth)) + minRoomWidth;

    unsigned int offsetX = (float)sizeX/2;
    unsigned int offsetY = (float)sizeY/2;
    unsigned int offsetZ = (float)sizeZ/2;

    bool finish = false;
    bool valid = true;

    while(!finish)
    {
        bool validSize = true;

        for(int i = 0; i < sizeX && valid; i++)
            for(int j = 0; j < sizeY && valid; j++)
                for(int k = 0; k < sizeZ && valid; k++)
                    validSize = testPosition(matrix,i+posX-offsetX,j+posY-offsetY,k+posZ-offsetZ);

        if(!validSize)
        {
            if(sizeX==minRoomWidth||sizeZ==minRoomWidth) valid = false;
            sizeX--;
            sizeZ--;
            offsetX = (float)sizeX/2;
            offsetZ = (float)sizeZ/2;
        }

        finish = validSize;
    }

    room validRoom;

    if(valid)
    {
        validRoom.posX = posX+offsetX;
        validRoom.posY = posY+offsetY;
        validRoom.posZ = posZ+offsetZ;
        validRoom.sizeX = sizeX;
        validRoom.sizeY = sizeY;
        validRoom.sizeZ = sizeZ;
        validRoom.id = id;
    }
    return validRoom;
}

    extern "C"
{

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
#endif
    struct dungeonMatrix *genDungeon(int args, const char **argv)
    {
        std::srand(std::time(nullptr));

        struct dungeonMatrix *dungeonMatrix;
        struct utilityMatrix *dungeon;

        allocUtilityMatrix(&dungeon, sizeX, sizeY, sizeZ);

        return dungeonMatrix;
    };
}
