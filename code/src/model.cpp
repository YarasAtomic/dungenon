#include "model.h"

Mesh meshFromDungeon(dungeonMatrix * dgn)
{
    int vm_size_x = dgn->size_x + 1;
    int vm_size_y = dgn->size_y + 1;
    int vm_size_z = dgn->size_z + 1;

    // Allocating

    int *** vertexMatrix = (int ***)malloc(vm_size_x * sizeof(int **));
    for(int i = 0 ; i < vm_size_x; i++)
    {
        vertexMatrix[i] = (int **)malloc(vm_size_y * sizeof(int *));
        for(int j = 0 ; j < vm_size_y; j++)
            vertexMatrix[i][j] = (int *)malloc(vm_size_z * sizeof(int));
    }

    // Default values

    for(int i = 0 ; i < vm_size_x; i++)
        for(int j = 0 ; j < vm_size_y; j++)
            for(int k = 0 ; k < vm_size_z; k++)
            {
                vertexMatrix[i][j][k] = -1;
            }
                

    Mesh mesh = {0};

    unsigned int nIndices = 0;

    // Generate faces

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
                    // Front face
                    generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);

                    generateVertex(i+1,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    //--------------------------
                    // Back face
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);

                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    mesh.triangleCount+=4;
                }
                if(dgn->data[i][j][k] & DUN_PXZ_WALL)
                {
                    // Top face
                    generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);

                    generateVertex(i+1,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    //--------------------------
                    // Botton face
                    generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);

                    generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i+1,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);

                    mesh.triangleCount+=4;
                }
                if(dgn->data[i][j][k] & DUN_PYZ_WALL)
                {
                    // Front face
                    generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);

                    generateVertex(i,j+1,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    //--------------------------
                    // Back face
                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k,&indices,&vertices,vertexMatrix,&nIndices);

                    generateVertex(i,j+1,k,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    generateVertex(i,j+1,k+1,&indices,&vertices,vertexMatrix,&nIndices);
                    mesh.triangleCount+=4;
                }
            }
        }
    }

    // Allocate memory to the mesh
    mesh.indices = (unsigned short *)malloc(indices.size() * sizeof(unsigned short));
    mesh.vertices = (float *)malloc(vertices.size() * sizeof(float));

    for(int i = 0; i < indices.size(); i++)
        mesh.indices[i] = indices[i];
    for(int i = 0; i < vertices.size(); i++)
        mesh.vertices[i] = vertices[i];

    mesh.vertexCount = vertices.size() / 3;

    // Free vertexMatrix memory
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