#include "main.h"
#include "ezdungeon.h"

bool usedModule = false;
dungeonMatrix * currentDungeon = nullptr;
bool dungeon2d = false;

void raylib()
{
        // Configuración
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Inicialización
    InitWindow(screenWidth, screenHeight, "DunGenon");

    // Initialize the camera
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Light light = {0};

    Mesh dungeonMesh;
    Model dungeonModel;
    Vector3 dungeonPos = {0,0,0};
    
    SetTargetFPS(60); // Establecer la frecuencia de actualización

    while (!WindowShouldClose()) { // Bucle principal

        // Actualizar
        // UpdateCamera(&camera,CAMERA_ORBITAL);

        #ifdef __EMSCRIPTEN__
        if (IsKeyReleased(KEY_N)) 
        {
            openModule("modules/cool.wasm");
            usedModule = true;
        }

        if (IsKeyReleased(KEY_M)) 
        {
            openModule("modules/uploaded.wasm");
            usedModule = true;
        }

        if (IsKeyReleased(KEY_E))
        {
            
            usedModule = false;
            cleanUploadedFileName();
            emscripten_browser_file::upload(".wasm", handle_upload_file);
            std::cout << "hola1 "<< std::endl;
        }
        // std::cout << "hola2 "<< std::endl;
        // if(getUploadedFileName()!=""&&!usedModule)
        // {
        //     std::cout << "holita "<< std::endl;
        //     openModule(getUploadedFileName());
        //     std::cout << "caput "<< std::endl;
        //     if(currentDungeon->size_y>1) 
        //     {
        //         std::cout << "Loading 3D dungeon" << std::endl;
        //         // dungeonModel = LoadModelFromMesh(meshFromdungeon(currentDungeon));

        //         // dungeonPos.x = - (float)currentDungeon->size_x/2;
        //         // dungeonPos.y = - (float)currentDungeon->size_y/2;
        //         // dungeonPos.z = - (float)currentDungeon->size_z/2;

        //         dungeon2d = false;
        //         std::cout << "Loaded 3D dungeon "<< std::endl;
        //     }
        //     else
        //     {
        //         dungeon2d = true;
        //         std::cout << "Loaded 2D dungeon" << std::endl;
        //     }

        //     usedModule = true;
        // }
        // std::cout << "hola3 "<< std::endl;

        #else

        if (IsKeyReleased(KEY_E)) 
        {
            openModule("modbin/module2.so");

            dungeonMesh = meshFromdungeon(currentDungeon);

            for(int i = 0; i < dungeonMesh.vertexCount;i++)
            {
                float x = dungeonMesh.vertices[i*3];
                float y = dungeonMesh.vertices[i*3+1];
                float z = dungeonMesh.vertices[i*3+2];
            }
            std::cout << "---------------" << std::endl;
            for(int i = 0; i < dungeonMesh.triangleCount;i++)
            {
                float x = dungeonMesh.indices[i*3];
                float y = dungeonMesh.indices[i*3+1];
                float z = dungeonMesh.indices[i*3+2];
            }
            dungeonModel = LoadModelFromMesh(dungeonMesh);

            dungeonPos.x = - (float)currentDungeon->size_x/2;
            dungeonPos.y = - (float)currentDungeon->size_y/2;
            dungeonPos.z = - (float)currentDungeon->size_z/2;

            dungeon2d = false;
            usedModule = true;
        }

        #endif


    //     // Dibujar
        BeginDrawing();
        ClearBackground(RAYWHITE);

    //     if(usedModule)
    //     {
    //         if(dungeon2d) 
    //         {
    //             drawDungeon2D(currentDungeon,30,30,400,400);
    //         }
    //         else
    //         {
    //             BeginMode3D(camera);

    //             DrawModel(dungeonModel,dungeonPos,1,GRAY);

    //             EndMode3D();
    //         }
                
    //     }

        EndDrawing();
    }

    // Cierre
    CloseWindow();
}

Rectangle currentTile;
Rectangle currentTileInside;

int border = 2;

Mesh meshFromdungeon(struct dungeonMatrix * dgn)
{
    std::cout << "Instanciando matriz de vertices ("<<dgn->size_x<<" , " << dgn->size_y << " , " << dgn->size_z << " )" << std::endl;

    int vm_size_x = dgn->size_x + 1;
    int vm_size_y = dgn->size_y + 1;
    int vm_size_z = dgn->size_z + 1;


    int *** vertexMatrix = (int ***)malloc(vm_size_x * sizeof(int **));
    for(int i = 0 ; i < vm_size_x; i++)
    {
        vertexMatrix[i] = (int **)malloc(vm_size_y * sizeof(int *));
        for(int j = 0 ; j < vm_size_y; j++)
            vertexMatrix[i][j] = (int *)malloc(vm_size_z * sizeof(int));
    }

    std::cout << "Evaluando matriz de vertices" << std::endl;
    for(int i = 0 ; i < vm_size_x; i++)
        for(int j = 0 ; j < vm_size_y; j++)
            for(int k = 0 ; k < vm_size_z; k++)
            {
                vertexMatrix[i][j][k] = -1;
            }
                

    Mesh mesh = {0};

    unsigned int nIndices = 0;

    std::cout << "Generando caras" << std::endl;


    std::vector<int> indices;
    std::vector<float> vertices;

    for(int i = 0 ; i < dgn->size_x; i++)
    {
        for(int j = 0 ; j < dgn->size_y; j++)
        {
            for(int k = 0 ; k < dgn->size_z; k++)
            {
                if(dgn->data[i][j][k] & DUN_PXY_WALL)
                {
                    //Cara de delante
                    generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);

                    generateVertex(i+1,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    //--------------------------
                    //Cara de atrás
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);

                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    mesh.triangleCount+=4;
                }
            }
        }
    }

    // Asignamos memoria al mesh
    mesh.indices = (unsigned short *)malloc(indices.size() * sizeof(unsigned short));
    mesh.vertices = (float *)malloc(vertices.size() * sizeof(float));

    for(int i = 0; i < indices.size(); i++)
        mesh.indices[i] = indices[i];
    for(int i = 0; i < vertices.size(); i++)
        mesh.vertices[i] = vertices[i];

    mesh.vertexCount = vertices.size() / 3;

    std::cout << "Numero de triangulos " << mesh.triangleCount << std::endl;
    std::cout << "Lista de indices de tamaño " << indices.size() << std::endl;
    std::cout << "Lista de vertices de tamaño " << vertices.size() << std::endl;

    // Liberamos la memoria de la matriz de vértices
    for(int i = 0; i < vm_size_x;i++)
    {
        for(int j = 0; j < vm_size_y;j++)
        {
            free(vertexMatrix[i][j]);
        }
        free(vertexMatrix[i]);
    }
    free(vertexMatrix);

    UploadMesh(&mesh, false);
    std::cout << "Mesh generado" << std::endl;
    return mesh;
}

void generateVertex(int x,int y,int z,std::vector<int>* indices, std::vector<float> *vertices, int *** vertexMatrix,unsigned int * nIndices)
{
    if(vertexMatrix[x][y][z] == -1)
    {
        vertexMatrix[x][y][z] = *nIndices;

        indices->push_back(*nIndices);

        vertices->push_back(x);
        vertices->push_back(y);
        vertices->push_back(z);

        (*nIndices)++;
    }
    else
    {
        indices->push_back(vertexMatrix[x][y][z]);
    }
}

void drawDungeon2D(struct dungeonMatrix * dgn,int posX,int posY,int sizeX,int sizeY)
{
    if(dgn->size_y==1)
    {
        float tileSizeX = (float)sizeX/(float)dgn->size_x;
        float tileSizeY = (float)sizeY/(float)dgn->size_z;

        
        for(int i = 0; i < dgn->size_x;i++)
        {
            for(int k = 0; k < dgn->size_z; k++)
            {
                if(dgn->data[i][0][k]>0)
                {
                    currentTile = {posX+(tileSizeX*i),posY+(tileSizeY*k),tileSizeX,tileSizeY};
                    currentTileInside = {posX+(tileSizeX*i)+border,posY+(tileSizeY*k)+border,tileSizeX-(border*2),tileSizeY-(border*2)};
                    DrawRectangleRec(currentTile, BLUE);
                    DrawRectangleRec(currentTileInside, SKYBLUE);
                }
            }
        }
    }
}

void drawDungeon3D(struct dungeonMatrix * dgn,int posX,int posY,int sizeX,int sizeY)
{

}

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

std::vector<std::string> getModules()
{
    std::vector<std::string> out;

    std::string extension;

    const char* directory_path = "./home/web_user";

    #ifdef __EMSCRIPTEN__
    extension = ".wasm";
    #else
    extension = ".so";
    #endif

    DIR* directory = opendir(directory_path);

    if (directory) 
    {
        struct dirent* entry;
        while ((entry = readdir(directory)) != nullptr) 
        {
            
            std::string filename = entry->d_name;
            if (entry->d_type == DT_REG)
            {
                if (filename.length() >= extension.size() && filename.substr(filename.length() - extension.size()) == extension) 
                {
                   out.push_back(entry->d_name);
                }
                
            }
            std::cout << "hola " <<(unsigned int)entry->d_type<< " "<< entry->d_name << std::endl;
        }
        closedir(directory);
    } 
    else 
    {
        std::cerr << "Failed to open directory" << std::endl;
    }

    return out;
}

void freeDungeon(struct dungeonMatrix * dgn)
{
    if(dgn == nullptr) return;

    for(int i = 0; i < dgn->size_x;i++)
    {
        for(int j = 0; j < dgn->size_y;j++)
        {
            free(dgn->data[i][j]);
        }
        free(dgn->data[i]);
    }
    free(dgn->data);
    free(dgn);
}

int openModule(std::string name)
{
    std::cout << "Loading module " << name << " ..." << std::endl;
    void * handle = nullptr;
    // #ifdef __EMSCRIPTEN__
    // handle = dlopen(name.c_str(),RTLD_LAZY);
    // #else
    // handle = dlopen(name.c_str(),RTLD_LAZY);
    // #endif
    if (!handle) {
        // fprintf(stderr, "Failed to find symbol: %s\n", dlerror());
        // dlclose(handle);
        return 1;
    }

    // dungeonMatrix * (* dungeonGen)(int,const char**) = (dungeonMatrix * (*)(int,const char**))dlsym(handle,"genDungeon");

    const char* argv [] = {"Arg1","Arg2"};

    std::cout << "Executing dunGen..." << std::endl;
    freeDungeon(currentDungeon);
    // currentDungeon = dungeonGen(2,argv);

    std::cout << "Finish" << std::endl;

    // dlclose(handle);
    return 0;
}

int main() 
{
    raylib();
    
    std::cout << "End" << std::endl;

    return 0;
}
