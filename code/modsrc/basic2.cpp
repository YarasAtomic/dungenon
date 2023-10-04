#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <vector>
#include "ezdungeon.h"
#include <iostream>

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

const int maxHall = 10;

room * genRoom(utilityMatrix * rooms, unsigned int centerX,unsigned int centerY,unsigned int centerZ,int id)
{
    // unsigned int sizeX = (rand() % (maxRoomWidth-minRoomWidth)) + minRoomWidth;
    // unsigned int sizeY = (rand() % (maxRoomHeight-minRoomHeight)) + minRoomHeight;
    // unsigned int sizeZ = (rand() % (maxRoomWidth-minRoomWidth)) + minRoomWidth;

    unsigned int offsetX = (float)sizeX/2;
    unsigned int offsetY = (float)sizeY/2;
    unsigned int offsetZ = (float)sizeZ/2;

    bool finish = false;
    bool valid = true;

    // while(!finish)
    // {
    //     bool validSize = true;

    //     // for(int i = 0; i < sizeX && valid; i++)
    //     //     for(int j = 0; j < sizeY && valid; j++)
    //     //         for(int k = 0; k < sizeZ && valid; k++)
    //     //             validSize = testPosition(rooms,i+centerX-offsetX,j+centerY-offsetY,k+centerZ-offsetZ);

    //     if(!validSize)
    //     {
    //         if(sizeX==minRoomWidth||sizeZ==minRoomWidth) valid = false;
    //         sizeX--;
    //         sizeZ--;
    //         offsetX = (float)sizeX/2;
    //         offsetZ = (float)sizeZ/2;
    //     }

    //     finish = validSize;
    // }

    room * validRoom = static_cast<struct room*>(malloc(sizeof(struct room)));;

    if(valid)
    {
        validRoom->posX = centerX+offsetX;
        validRoom->posY = centerY+offsetY;
        validRoom->posZ = centerZ+offsetZ;
        // validRoom->sizeX = sizeX;
        // validRoom->sizeY = sizeY;
        // validRoom->sizeZ = sizeZ;
        validRoom->id = id;
    }
    return validRoom;
}
extern "C" {


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
struct dungeonMatrix *  genDungeon(int args,const char**argv)
{
    std::srand(time(NULL));

    struct dungeonMatrix* dungeon;
    struct utilityMatrix* rooms;

    allocDungeonMatrix(&dungeon,sizeX,sizeY,sizeZ);
    allocUtilityMatrix(&rooms,sizeX,sizeY,sizeZ);

    for(int i = 0 ; i < sizeX; i++)
        for(int j = 0 ; j < sizeY; j++)
            for(int k = 0 ; k < sizeZ; k++)
            {
                dungeon->data[i][j][k] = DUN_PXY_WALL;
                dungeon->data[i][j][k] |= DUN_PXZ_WALL;
                dungeon->data[i][j][k] |= DUN_PYZ_WALL;
            }

    genRoom(rooms,sizeX/2,0,sizeZ/2,0);

    return dungeon;
};

}
