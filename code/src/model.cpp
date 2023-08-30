#include "model.h"

Mesh meshFromDungeon(dungeonMatrix * dgn)
{
    // int vm_size_x = dgn->size_x + 1;
    // int vm_size_y = dgn->size_y + 1;
    // int vm_size_z = dgn->size_z + 1;

    // // Allocating

    // int *** vertexMatrix = (int ***)malloc(vm_size_x * sizeof(int **));
    // for(int i = 0 ; i < vm_size_x; i++)
    // {
    //     vertexMatrix[i] = (int **)malloc(vm_size_y * sizeof(int *));
    //     for(int j = 0 ; j < vm_size_y; j++)
    //         vertexMatrix[i][j] = (int *)malloc(vm_size_z * sizeof(int));
    // }

    // // Default values

    // for(int i = 0 ; i < vm_size_x; i++)
    //     for(int j = 0 ; j < vm_size_y; j++)
    //         for(int k = 0 ; k < vm_size_z; k++)
    //         {
    //             vertexMatrix[i][j][k] = -1;
    //         }
                

    Mesh mesh = {0};

    // unsigned int nIndices = 0;

    // Generate faces

    std::vector<int> indices;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

    for(int i = 0 ; i < dgn->size_x; i++)
    {
        for(int j = 0 ; j < dgn->size_y; j++)
        {
            for(int k = 0 ; k < dgn->size_z; k++)
            {
                if(dgn->data[i][j][k] & DUN_PXY_WALL)
                {
                    // // Front face
                    // generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);

                    // generateVertex(i+1,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // //--------------------------
                    // // Back face
                    // generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);

                    // generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // mesh.triangleCount+=4;

                    generateFace(i,j,k,&indices,&vertices,&normals,&texCoords,DUN_PXY_WALL); 
                    mesh.triangleCount+=4;
                }
                if(dgn->data[i][j][k] & DUN_PXZ_WALL)
                {
                    // // Top face
                    // generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);

                    // generateVertex(i+1,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // //--------------------------
                    // // Botton face
                    // generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);

                    // generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i+1,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);

                    // mesh.triangleCount+=4;
                    generateFace(i,j,k,&indices,&vertices,&normals,&texCoords,DUN_PXZ_WALL);
                    mesh.triangleCount+=4;
                }
                if(dgn->data[i][j][k] & DUN_PYZ_WALL)
                {
                    // // Front face
                    // generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);

                    // generateVertex(i,j+1,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // //--------------------------
                    // // Back face
                    // generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);

                    // generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // generateVertex(i,j+1,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    // mesh.triangleCount+=4;
                    generateFace(i,j,k,&indices,&vertices,&normals,&texCoords,DUN_PYZ_WALL);
                    mesh.triangleCount+=4;
                }
            }
        }
    }

    // Allocate memory to the mesh
    mesh.indices = (unsigned short *)malloc(indices.size() * sizeof(unsigned short));
    mesh.vertices = (float *)malloc(vertices.size() * sizeof(float));
    mesh.normals = (float *)malloc(normals.size() * sizeof(float));
    mesh.texcoords = (float *)malloc(texCoords.size() * sizeof(float));

    for(int i = 0; i < indices.size(); i++)
        mesh.indices[i] = indices[i];
    for(int i = 0; i < vertices.size(); i++)
        mesh.vertices[i] = vertices[i];
    for(int i = 0; i < normals.size(); i++)
        mesh.normals[i] = normals[i];
    for(int i = 0; i < texCoords.size(); i++)
        mesh.texcoords[i] = texCoords[i];

    mesh.vertexCount = vertices.size() / 3;

    // // Free vertexMatrix memory
    // for(int i = 0; i < vm_size_x;i++)
    // {
    //     for(int j = 0; j < vm_size_y;j++)
    //     {
    //         free(vertexMatrix[i][j]);
    //     }
    //     free(vertexMatrix[i]);
    // }
    // free(vertexMatrix);

    UploadMesh(&mesh, false);
    return mesh;
}

Mesh GenMeshCustom(void)
{
    Mesh mesh = { 0 };
    mesh.triangleCount = 1;
    mesh.vertexCount = mesh.triangleCount*3;
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount*2*sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

    // Vertex at (0, 0, 0)
    mesh.vertices[0] = 0;
    mesh.vertices[1] = 0;
    mesh.vertices[2] = 0;
    mesh.normals[0] = 0;
    mesh.normals[1] = 1;
    mesh.normals[2] = 0;
    mesh.texcoords[0] = 0;
    mesh.texcoords[1] = 0;

    // Vertex at (1, 0, 2)
    mesh.vertices[3] = 1;
    mesh.vertices[4] = 0;
    mesh.vertices[5] = 2;
    mesh.normals[3] = 0;
    mesh.normals[4] = 1;
    mesh.normals[5] = 0;
    mesh.texcoords[2] = 0.5f;
    mesh.texcoords[3] = 1.0f;

    // Vertex at (2, 0, 0)
    mesh.vertices[6] = 2;
    mesh.vertices[7] = 0;
    mesh.vertices[8] = 0;
    mesh.normals[6] = 0;
    mesh.normals[7] = 1;
    mesh.normals[8] = 0;
    mesh.texcoords[4] = 1;
    mesh.texcoords[5] =0;

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

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

void generateFace(int x,int y,int z,std::vector<int>* indices, std::vector<float> *vertices,std::vector<float> *normals,std::vector<float> * texCoords,int dir)
{
    int vIndex = vertices->size()/3;

    int nX = 0;
    int nY = 0;
    int nZ = 0;

    int altX = 0;
    int altY = 0;
    int altZ = 0;

    int altX2 = 0;
    int altY2 = 0;
    int altZ2 = 0;

    switch(dir)
    {
    case DUN_PXY_WALL: 
        altX = 1;
        altY2 = 1;
        nZ = 1;
        break;
    case DUN_PXZ_WALL: 
        altX = 1;
        altZ2 = 1;
        nY = -1;
        break;
    case DUN_PYZ_WALL: 
        altY = 1;
        altZ2 = 1;
        nX = 1;
        break;
    }

    vertices->push_back(x);
    vertices->push_back(y);
    vertices->push_back(z);

    indices->push_back(vIndex);

    vertices->push_back(x+altX);
    vertices->push_back(y+altY);
    vertices->push_back(z+altZ);

    indices->push_back(vIndex+1);

    vertices->push_back(x+altX2);
    vertices->push_back(y+altY2);
    vertices->push_back(z+altZ2);

    indices->push_back(vIndex+2);

    vertices->push_back(x+altX+altX2);
    vertices->push_back(y+altY+altY2);
    vertices->push_back(z+altZ+altZ2);

    indices->push_back(vIndex+3);

    indices->push_back(vIndex+2);

    indices->push_back(vIndex+1);

    vIndex+=4;

    vertices->push_back(x);
    vertices->push_back(y);
    vertices->push_back(z);

    indices->push_back(vIndex+2);

    vertices->push_back(x+altX);
    vertices->push_back(y+altY);
    vertices->push_back(z+altZ);

    indices->push_back(vIndex+1);

    vertices->push_back(x+altX2);
    vertices->push_back(y+altY2);
    vertices->push_back(z+altZ2);

    indices->push_back(vIndex);

    vertices->push_back(x+altX+altX2);
    vertices->push_back(y+altY+altY2);
    vertices->push_back(z+altZ+altZ2);

    indices->push_back(vIndex+3);

    indices->push_back(vIndex+1);

    indices->push_back(vIndex+2);

    for(int i = 0; i < 4; i++)
    {
        normals->push_back(nX);
        normals->push_back(nY);
        normals->push_back(nZ);
    }

    for(int i = 0; i < 4; i++)
    {
        normals->push_back(-nX);
        normals->push_back(-nY);
        normals->push_back(-nZ);
    }

    for(int i = 0; i < 2; i++)
    {
        texCoords->push_back(0);
        texCoords->push_back(0);

        texCoords->push_back(0);
        texCoords->push_back(1);

        texCoords->push_back(1);
        texCoords->push_back(0);

        texCoords->push_back(1);
        texCoords->push_back(1);
    }
}