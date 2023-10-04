#include "ezdungeon.h"

void allocUtilityMatrix(utilityMatrix **dungeon,unsigned int sizeX,unsigned int sizeY, unsigned int sizeZ)
{
    (*dungeon)= static_cast<struct utilityMatrix*>(malloc(sizeof(struct utilityMatrix)));

     int *** data = ( int ***)malloc(sizeX * sizeof( int **));
    for(int i = 0 ; i < sizeX; i++)
    {
        data[i] = ( int **)malloc(sizeY * sizeof( int *));
        for(int j = 0 ; j < sizeY; j++)
        {
            data[i][j] = ( int *)malloc(sizeZ * sizeof( int));
            for(int k = 0; k < sizeZ;k++)
                data[i][j][k] = -1;
        }
            
    }

    (*dungeon)->data = data;
    (*dungeon)->size_x = sizeX;
    (*dungeon)->size_y = sizeY;
    (*dungeon)->size_z = sizeZ;
}

void allocDungeonMatrix(dungeonMatrix **dungeon,unsigned int sizeX,unsigned int sizeY, unsigned int sizeZ)
{
    (*dungeon)= static_cast<struct dungeonMatrix*>(malloc(sizeof(struct dungeonMatrix)));

    unsigned int *** data = (unsigned int ***)malloc(sizeX * sizeof(unsigned int **));
    for(int i = 0 ; i < sizeX; i++)
    {
        data[i] = (unsigned int **)malloc(sizeY * sizeof(unsigned int *));
        for(int j = 0 ; j < sizeY; j++)
            data[i][j] = (unsigned int *)malloc(sizeZ * sizeof(unsigned int));
    }

    (*dungeon)->data = data;
    (*dungeon)->size_x = sizeX;
    (*dungeon)->size_y = sizeY;
    (*dungeon)->size_z = sizeZ;
}

void freeDungeonMatrix(struct dungeonMatrix ** matrix)
{
    if (*matrix) {
        for (unsigned int i = 0; i < (*matrix)->size_x; i++) {
            for (unsigned int j = 0; j < (*matrix)->size_y; j++) {
                delete[] (*matrix)->data[i][j];
            }
            delete[] (*matrix)->data[i];
        }
        delete[] (*matrix)->data;
        delete (*matrix);
    }
}

void freeUtilityMatrix(struct utilityMatrix ** dgn)
{
    if((*dgn) == nullptr) return;

    for(int i = 0; i < (*dgn)->size_x;i++)
    {
        for(int j = 0; j < (*dgn)->size_y;j++)
        {
            free((*dgn)->data[i][j]);
        }
        free((*dgn)->data[i]);
    }
    free((*dgn)->data);
    free((*dgn));
}

void cpyDungeon(dungeonMatrix ** destination, dungeonMatrix * source)
{
    freeDungeonMatrix(destination);

    if(source->size_x>0&&source->size_y>0&&source->size_z>0)
    {
        (*destination) = static_cast<struct dungeonMatrix*>(malloc(sizeof(struct dungeonMatrix)));

        (*destination)->data = (unsigned int ***)malloc(source->size_x * sizeof(unsigned int **));
        for(int i = 0 ; i < source->size_x; i++)
        {
            (*destination)->data[i] = (unsigned int **)malloc(source->size_y * sizeof(unsigned int *));
            for(int j = 0 ; j < source->size_y; j++)
                (*destination)->data[i][j] = (unsigned int *)malloc(source->size_z * sizeof(unsigned int));
        }

        (*destination)->size_x = source->size_x;
        (*destination)->size_y = source->size_y;
        (*destination)->size_z = source->size_z;

        (*destination)->err = source->err;

        for(int i = 0 ; i < source->size_x; i++)
            for(int j = 0 ; j < source->size_y; j++)
                for(int k = 0 ; k < source->size_z; k++)
                    (*destination)->data[i][j][k] = source->data[i][j][k];
    }


}

bool testPosition(utilityMatrix * dgn,unsigned int posX,unsigned int posY, unsigned int posZ)
{
    if((posX>dgn->size_x)||(posY>dgn->size_y)||(posZ>dgn->size_z)) return false;

    if(dgn->data[posX][posY][posZ]==-1) return true;

    return false;
}

void hello(){}