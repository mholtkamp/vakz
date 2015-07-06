#include "MeshLoader.h"
#include "VGL.h"
#include "Log.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

float* vertices = NULL;
float* normals = NULL;
float* uvs = NULL;
int*	faces = NULL;
float** vb = NULL;

char arFileBuffer[MeshLoader::MESH_LOADER_MAX_FILE_SIZE];

int nNumVerts;
int nNumUVs;
int nNumNormals;
int nNumFaces;
int nNumObjects;

void get_numbers(const char* pFileName)
{
    int nFileSize = 0;
	nNumVerts = 0;
	nNumFaces = 0;
    nNumNormals = 0;
	nNumUVs = 0;

    //FILE* pFile = 0;
    //pFile = fopen(pFileName, "r");
    //fseek(pFile, 0, SEEK_END);
    //nFileSize = ftell(pFile);

    //if (nFileSize >= MeshLoader::MESH_LOADER_MAX_FILE_SIZE - 1)
    //{
    //    LogError("OBJ is too large.");
    //}

    //if (pFile == 0)
    //{
    //    LogError("OBJ file could not be loaded.");
    //    return;
    //}

	string line;
	ifstream fileStream(pFileName);
	if(fileStream.is_open())
	{
		while(getline(fileStream, line))
		{
            if(line.size() > 0)
            {
			    if((line[0] == 'v') && (line[1] == ' '))
				    nNumVerts++;
			    else if((line[0] == 'v') && (line[1] == 'n'))
				    nNumNormals++;
			    else if((line[0] == 'v') && (line[1] == 't'))
				    nNumUVs++;
			    else if(line[0] == 'f')
				    nNumFaces++;
            }
		}
	}
	fileStream.close();

}

float* create_vertex_buffer()
{
	float* vb = new float[nNumFaces*24];
	//Add vertex data for each face	
	for(int i = 0; i < nNumFaces; i++)
	{
		//Vertex 1
		vb[i*24 + 0] = vertices[(faces[i*9 + 0]-1)*3 + 0];
		vb[i*24 + 1] = vertices[(faces[i*9 + 0]-1)*3 + 1];
		vb[i*24 + 2] = vertices[(faces[i*9 + 0]-1)*3 + 2];
		vb[i*24 + 3] = uvs[(faces[i*9 + 1]-1)*2 + 0];
		vb[i*24 + 4] = uvs[(faces[i*9 + 1]-1)*2 + 1];
		vb[i*24 + 5] = normals[(faces[i*9 + 2]-1)*3 + 0];
		vb[i*24 + 6] = normals[(faces[i*9 + 2]-1)*3 + 1];
		vb[i*24 + 7] = normals[(faces[i*9 + 2]-1)*3 + 2];

		//Vertex 2
		vb[i*24 + 8] = vertices[(faces[i*9 + 3]-1)*3 + 0];
		vb[i*24 + 9] = vertices[(faces[i*9 + 3]-1)*3 + 1];
		vb[i*24 + 10] = vertices[(faces[i*9 + 3]-1)*3 + 2];
		vb[i*24 + 11] = uvs[(faces[i*9 + 4]-1)*2 + 0];
		vb[i*24 + 12] = uvs[(faces[i*9 + 4]-1)*2 + 1];
		vb[i*24 + 13] = normals[(faces[i*9 + 5]-1)*3 + 0];
		vb[i*24 + 14] = normals[(faces[i*9 + 5]-1)*3 + 1];
		vb[i*24 + 15] = normals[(faces[i*9 + 5]-1)*3 + 2];

		//Vertex 3
		vb[i*24 + 16] = vertices[(faces[i*9 + 6]-1)*3 + 0];
		vb[i*24 + 17] = vertices[(faces[i*9 + 6]-1)*3 + 1];
		vb[i*24 + 18] = vertices[(faces[i*9 + 6]-1)*3 + 2];
		vb[i*24 + 19] = uvs[(faces[i*9 + 7]-1)*2 + 0];
		vb[i*24 + 20] = uvs[(faces[i*9 + 7]-1)*2 + 1];
		vb[i*24 + 21] = normals[(faces[i*9 + 8]-1)*3 + 0];
		vb[i*24 + 22] = normals[(faces[i*9 + 8]-1)*3 + 1];
		vb[i*24 + 23] = normals[(faces[i*9 + 8]-1)*3 + 2];
	}
	return vb;
}

float* create_vertex_buffer(int nFaces, unsigned int faceOffset)
{
	float* vb = new float[nFaces*24];
	//Add vertex data for each face	
	for(int i = 0; i < nFaces; i++)
	{
		//Vertex 1
		vb[i*24 + 0] = vertices[(faces[i*9 + 0 + faceOffset]-1)*3 + 0];
		vb[i*24 + 1] = vertices[(faces[i*9 + 0 + faceOffset]-1)*3 + 1];
		vb[i*24 + 2] = vertices[(faces[i*9 + 0 + faceOffset]-1)*3 + 2];
		vb[i*24 + 3] = uvs[(faces[i*9 + 1 + faceOffset]-1)*2 + 0];
		vb[i*24 + 4] = uvs[(faces[i*9 + 1 + faceOffset]-1)*2 + 1];
		vb[i*24 + 5] = normals[(faces[i*9 + 2 + faceOffset]-1)*3 + 0];
		vb[i*24 + 6] = normals[(faces[i*9 + 2 + faceOffset]-1)*3 + 1];
		vb[i*24 + 7] = normals[(faces[i*9 + 2 + faceOffset]-1)*3 + 2];

		//Vertex 2
		vb[i*24 + 8] = vertices[(faces[i*9 + 3 + faceOffset]-1)*3 + 0];
		vb[i*24 + 9] = vertices[(faces[i*9 + 3 + faceOffset]-1)*3 + 1];
		vb[i*24 + 10] = vertices[(faces[i*9 + 3 + faceOffset]-1)*3 + 2];
		vb[i*24 + 11] = uvs[(faces[i*9 + 4 + faceOffset]-1)*2 + 0];
		vb[i*24 + 12] = uvs[(faces[i*9 + 4 + faceOffset]-1)*2 + 1];
		vb[i*24 + 13] = normals[(faces[i*9 + 5 + faceOffset]-1)*3 + 0];
		vb[i*24 + 14] = normals[(faces[i*9 + 5 + faceOffset]-1)*3 + 1];
		vb[i*24 + 15] = normals[(faces[i*9 + 5 + faceOffset]-1)*3 + 2];

		//Vertex 3
		vb[i*24 + 16] = vertices[(faces[i*9 + 6 + faceOffset]-1)*3 + 0];
		vb[i*24 + 17] = vertices[(faces[i*9 + 6 + faceOffset]-1)*3 + 1];
		vb[i*24 + 18] = vertices[(faces[i*9 + 6 + faceOffset]-1)*3 + 2];
		vb[i*24 + 19] = uvs[(faces[i*9 + 7 + faceOffset]-1)*2 + 0];
		vb[i*24 + 20] = uvs[(faces[i*9 + 7 + faceOffset]-1)*2 + 1];
		vb[i*24 + 21] = normals[(faces[i*9 + 8 + faceOffset]-1)*3 + 0];
		vb[i*24 + 22] = normals[(faces[i*9 + 8 + faceOffset]-1)*3 + 1];
		vb[i*24 + 23] = normals[(faces[i*9 + 8 + faceOffset]-1)*3 + 2];
	}
	return vb;
} 


unsigned int MeshLoader::LoadOBJ(const char*   pFileName, 
                                 unsigned int& nFaces)
{
	get_numbers(pFileName);

	nFaces = nNumFaces;

	vertices = (float*) malloc( nNumVerts * 3 * sizeof(float));
	normals = (float*) malloc( nNumNormals * 3 * sizeof(float));
	uvs = (float*) malloc( nNumUVs * 2 * sizeof(float));
	faces = (int*) malloc( nNumFaces * 9 * sizeof(int));

	string line;
	ifstream fileStream(pFileName);
	if(fileStream.is_open())
	{
		int v = 0;
		int n = 0;
		int t = 0;
		int f = 0;

		while( getline (fileStream, line))
		{
            if(line.size() == 0)
                continue;
			//Vertex parsing
			if(line[0] == 'v' && line[1] == ' ')
			{
				int pos = 2;
				for(int j = 0; j < 3; j++)
				{
					string str = "";
					while(pos < line.size() && line[pos] != ' ')
						str = str + line[pos++];

					vertices[v*3 + j] = stof(str,NULL);
					pos++;
				}
				v++;
			}

			//Normal parsing
			else if(line[0] == 'v' && line[1] == 'n')
			{
				int pos = 3;
				for(int j = 0; j < 3; j++)
				{
					string str = "";
					while(pos < line.size() && line[pos] != ' ')
						str = str + line[pos++];

					normals[n*3 + j] = stof(str,NULL);
					pos++;
				}
				n++;
			}

			//UV parsing
			else if(line[0] == 'v' && line[1] == 't')
			{
				//std::cout << "UV found\n";
				int pos = 3;
				for(int j = 0; j < 2; j++)
				{
					string str = "";
					while(pos < line.size() && line[pos] != ' ')
						str = str + line[pos++];

					uvs[t*2 + j] = stof(str,NULL);
					pos++;
				}
				t++;
				//std::cout << "UV parsed\n";
			}


			//Face Parsing
			else if(line[0] == 'f')
			{
				//std::cout << "Face found\n";
				int pos = 2;
				for(int j = 0; j < 9; j++)
				{
					string str("");
					while((pos < line.size()) &&
						  (line[pos] != '/')  &&
						  (line[pos] != ' '))
						str = str + line[pos++];

					//std::cout << "str: " << str << "\n";

					faces[f*9 + j] = stoi(str);
					if(pos > line.size())
						break;
					else
						pos++;
				}
				f++;
				//std::cout << "Face parsed\n";
			}
		}
	}

	//print_uvs();
	fileStream.close();

	float* vertexBuffer = create_vertex_buffer();

	free(vertices);
	free(normals);
	free(uvs);
	free(faces);

	GLuint vbo = 0;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (nNumFaces)*24, vertexBuffer, GL_STATIC_DRAW);

    delete [] vertexBuffer;

	return vbo;
}