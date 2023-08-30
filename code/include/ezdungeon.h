
#ifndef EZDUNGEON
#define EZDUNGEON
const unsigned int DUN_FULL_BLOCK = 1;
const unsigned int DUN_PXY_WALL = 2;
const unsigned int DUN_PXZ_WALL = 4;
const unsigned int DUN_PYZ_WALL = 8;
const unsigned int DUN_FULL_VAR = 16;
const unsigned int DUN_PXY_VAR = 32;
const unsigned int DUN_PXZ_VAR = 64;
const unsigned int DUN_PYZ_VAR = 128;

struct utilityMatrix {
    int *** data;
    unsigned int size_x = 0;
    unsigned int size_y = 0;
    unsigned int size_z = 0;
};

struct dungeonMatrix {
    unsigned int *** data;
    unsigned int size_x = 0;
    unsigned int size_y = 0;
    unsigned int size_z = 0;
    unsigned int err;
};

void allocUtilityMatrix(utilityMatrix **matrix,unsigned int sizeX,unsigned int sizeY, unsigned int sizeZ)
{
    *matrix = static_cast<struct utilityMatrix*>(malloc(sizeof(struct utilityMatrix)));

    (*matrix)->size_x = sizeX;
    (*matrix)->size_y = sizeY;
    (*matrix)->size_z = sizeZ;

    (*matrix)->data = (int ***)malloc(sizeX * sizeof( int **));
    for(int i = 0 ; i < sizeX; i++)
    {
        (*matrix)->data[i] = ( int **)malloc(sizeY * sizeof( int *));
        for(int j = 0 ; j < sizeY; j++)
        {
            (*matrix)->data[i][j] = ( int *)malloc(sizeZ * sizeof( int));
            for(int k = 0 ; k < sizeZ; k++)
            {
                (*matrix)->data[i][j][k] = -1;
            }
        }
            
    }
}

void allocDungeonMatrix(dungeonMatrix **matrix,unsigned int sizeX,unsigned int sizeY, unsigned int sizeZ)
{
    (*matrix) = static_cast<struct dungeonMatrix*>(malloc(sizeof(struct dungeonMatrix)));

    (*matrix)->size_x = sizeX;
    (*matrix)->size_y = sizeY;
    (*matrix)->size_z = sizeZ;

    (*matrix)->err = 0;

    (*matrix)->data = (unsigned int ***)malloc(sizeX * sizeof(unsigned int **));
    for(int i = 0 ; i < sizeX; i++)
    {
        (*matrix)->data[i] = (unsigned int **)malloc(sizeY * sizeof(unsigned int *));
        for(int j = 0 ; j < sizeY; j++)
            (*matrix)->data[i][j] = (unsigned int *)malloc(sizeZ * sizeof(unsigned int));
    }
}

void freeDungeon(struct dungeonMatrix ** dgn)
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
    freeDungeon(destination);

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

bool testPosition(utilityMatrix * dgn,unsigned int posX,unsigned int posY, unsigned int posZ)
{
    if((posX>dgn->size_x)||(posY>dgn->size_y)||(posZ>dgn->size_z)) return false;

    if(dgn->data[posX][posY][posZ]==-1) return true;

    return false;
}

#endif
