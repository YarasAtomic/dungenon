#include <raylib.h>
#include "emscripten_browser_file.h"
#include "main.h"

int openModule(std::string name,dungeonMatrix ** dungeon)
{
    std::cout << "Loading module " << name << "..." << std::endl;
    void * handle = nullptr;

    handle = dlopen(name.c_str(),RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "Failed to find symbol: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    dungeonMatrix * (* dungeonGen)(int,const char**) = (dungeonMatrix * (*)(int,const char**))dlsym(handle,"genDungeon");

    const char* argv [] = {"Arg1","Arg2"};

    std::cout << "Executing dunGen..." << std::endl;

    dungeonMatrix * temp = dungeonGen(2,argv);

    cpyDungeon(dungeon,temp);

    dlclose(handle);
    return 0;
}

void drawDungeon2D(struct dungeonMatrix * dgn,int posX,int posY,int sizeX,int sizeY)
{
    Rectangle currentTile;
    Rectangle currentTileInside;

    int border = 2;

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

bool isModuleLoaded = false;
std::string currentModFileId = "";
std::string currentModFileName = "";
int screenWidth = 800;
int screenHeight = 600;

// Exported funtion so javascript can call it when the window is resized
void resizeCanvas(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    SetWindowSize(width,height);
}

// Function called by the menu when a entry is clicked
void menuInteraction(std::string name,std::string id)
{
    currentModFileId = "modules/"+id;
    currentModFileName = name;
    isModuleLoaded = false;
}

void raylib()
{
    // Initialize the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "DunGenon");

    // Initialize the camera
    Camera camera;
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60); // Set framerate

    // Canvas
    EM_ASM_INT_V({ resizeCanvas(); });

    // Variables
    Model dungeonModel;
    Vector3 dungeonPos;

    bool isDungeon2d = false;

    dungeonMatrix * currentDungeon = nullptr;

    // Gui
    ScrollMenu moduleMenu(10,40,150,menuInteraction);

    #ifdef __EMSCRIPTEN__
    getModuleNames();
    #endif

    // Lights

    // Shader lightShader = LoadShader(TextFormat("/resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
    //                            TextFormat("/resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

    std::string vShader = "";
    std::string fShader = "";
    bool loadedShaders = false;

    getShader("https://raw.githubusercontent.com/raysan5/raylib/master/examples/shaders/resources/shaders/glsl100/lighting.vs",&vShader);
    getShader("https://raw.githubusercontent.com/raysan5/raylib/master/examples/shaders/resources/shaders/glsl100/lighting.fs",&fShader);

    Light global;

    Shader lightShader;

    while (!WindowShouldClose()) { // Main loop
        // Render update
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };

        ClearBackground(BLACK);

        UpdateCamera(&camera,CAMERA_ORBITAL);

        if(!loadedShaders && vShader!="" && fShader!= "")
        {
            loadedShaders = true;
            lightShader = LoadShaderFromMemory(vShader.c_str(),fShader.c_str());

            lightShader.locs[SHADER_LOC_VECTOR_VIEW] =  GetShaderLocation(lightShader,"viewPos");

            int ambientLoc = GetShaderLocation(lightShader,"ambient");
            SetShaderValue(lightShader,ambientLoc,(float[4]){0.3f,0.3f,0.3f,1.0f},SHADER_UNIFORM_VEC4);

            global = CreateLight(LIGHT_DIRECTIONAL,(Vector3){3,2,1},(Vector3){0,0,0},RAYWHITE,lightShader);
        }

        SetShaderValue(lightShader,lightShader.locs[SHADER_LOC_VECTOR_VIEW],cameraPos,SHADER_UNIFORM_VEC3);

        UpdateLightValues(lightShader,global);

        // Update
        moduleMenu.update();
        moduleMenu.updateInteraction(0,0);

        // Upload a new module
        if (IsKeyReleased(KEY_E)) 
        {
            cleanUploadedFileId();
            isModuleLoaded = false;
            #ifdef __EMSCRIPTEN__
            emscripten_browser_file::upload(".wasm", handleUploadFile);
            #endif
        }

        // Delete all modules
        if (IsKeyReleased(KEY_C)) 
        {
            #ifdef __EMSCRIPTEN__
            clearModules();
            moduleMenu.clear();
            #endif
        }

        // Is there an uploaded file?
        #ifdef __EMSCRIPTEN__
        if(getUploadedFileId()!="")
        {
            currentModFileId = "modules/"+getUploadedFileId();
            moduleMenu.addItem(getUploadedFileName(),getUploadedFileId());
            cleanUploadedFileId();
        }
        #endif

        // Is module file loaded?
        if(currentModFileId!=""&&!isModuleLoaded)
        {            
            // Try to load module
            if(openModule(currentModFileId,&currentDungeon)==0)
            {
                // Module loadedS
                if(currentDungeon->size_y>1) 
                {
                    Mesh dungeonMesh = meshFromDungeon(currentDungeon);

                    dungeonModel = LoadModelFromMesh(dungeonMesh);

                    dungeonPos.x = - (float)currentDungeon->size_x/2;
                    dungeonPos.y = - (float)currentDungeon->size_y/2;
                    dungeonPos.z = - (float)currentDungeon->size_z/2;

                    isDungeon2d = false;
                    std::cout << "Loaded 3D dungeon "<< std::endl;
                }
                else
                {
                    isDungeon2d = true;
                    std::cout << "Loaded 2D dungeon" << std::endl;
                }

                isModuleLoaded = true;
            }
            else
            {
                currentModFileId = "";
            }

        }
        else if(loadedShaders)
            dungeonModel.materials[0].shader = lightShader;


        BeginDrawing();

        // Is module loaded?
        if(isModuleLoaded)
        {
            if(isDungeon2d) 
            {
                int sizeX = 400>screenWidth ? screenWidth*0.8 : 400;
                int sizeY = 400>screenHeight ? screenHeight*0.8 : 400;

                int size = sizeX<sizeY ? sizeX : sizeY;

                drawDungeon2D(currentDungeon,screenWidth/2-size/2,screenHeight/2-size/2,size,size);
            }
            else
            {
                BeginMode3D(camera);
                DrawModel(dungeonModel,dungeonPos,1.0f,GRAY);

                EndMode3D();
            }
        }

        // Is names downloaded
        if(checkDownload())
        {
            int n = getDownloadedNamesCount();
            for(int i = 0; i < n; i++)
            {
                std::string name = getDownloadedName(i).first;
                std::string id = getDownloadedName(i).second;
                moduleMenu.addItem(name,id);
            }
            cleanDownloadedNames();
        }

        moduleMenu.draw(0,0);
        
        
        EndDrawing();
    }

    // Close
    CloseWindow();
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

int main()
{
    raylib();

    return 0;
}