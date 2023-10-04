#include "ezdungeon.h"
#include <iostream>
#include <string>
#include <dlfcn.h>

void printDungeon(struct dungeonMatrix * dgn)
{
    for(int j = 0; j < dgn->size_y;j++)
    {
        if(dgn->size_y>1)
            std::cout << std::endl << "Layer " << j << ":" << std::endl;
        for(int i = 0; i < dgn->size_x;i++)
        {
            for(int k = 0; k < dgn->size_z; k++)
                std::cout << dgn->data[i][j][k];
            std::cout << std::endl;
        }
    }
}

int openModule(std::string name,dungeonMatrix ** dungeon)
{
    std::cout << "Loading module " << name << "..." << std::endl;
    void * handle = nullptr;

    handle = dlopen(name.c_str(),RTLD_NOW);

    if (!handle) {
        fprintf(stderr, "Failed to find symbol: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    dungeonMatrix * (* dungeonGen)(int,const char**) = (dungeonMatrix * (*)(int,const char**))dlsym(handle,"genDungeon");

    const char* argv [] = {"Arg1","Arg2"};

    std::cout << "Executing dunGen..." << std::endl;
    dungeonMatrix * temp;
    try{
        temp = dungeonGen(2,argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception while loading " << e.what() << std::endl;
        return 1;
    }
    

    cpyDungeon(dungeon,temp);

    std::cout << "dunGen finished" << std::endl;

    dlclose(handle);
    return 0;
}

int main()
{
    

    const char * args[2] = {"Arg0","Arg1"};
    dungeonMatrix * dungeon;
    openModule("./modbin/basic2.so",&dungeon);

    std::cout << "sizeX " << dungeon->size_x << std::endl;
    std::cout << "sizeY " << dungeon->size_y << std::endl;
    std::cout << "sizeZ " << dungeon->size_z << std::endl;

    printDungeon(dungeon);

    return 0;
}