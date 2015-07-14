#include "MeshLoader.h"
#include "VGL.h"
#include "Log.h"
#include <stdlib.h>
#include <stdio.h>

char MeshLoader::s_arFileBuffer[MESH_LOADER_MAX_FILE_SIZE];

//*****************************************************************************
// GetCounts
//*****************************************************************************
void MeshLoader::GetCounts(const char* pFileName,
                                  int& nNumVerts,
                                  int& nNumUVs,
                                  int& nNumNormals,
	                              int& nNumFaces)
{
    int nFileSize = 0;
    int nLineLength = 0;
    char* pNewLine = 0;
    char* pStr = 0;
    FILE* pFile = 0;

    nNumVerts   = 0;
	nNumFaces   = 0;
    nNumNormals = 0;
	nNumUVs     = 0;

    // Open the requested file.
    pFile = fopen(pFileName, "r");

    // Check if file was found
    if (pFile == 0)
    {
        LogError("OBJ file could not be loaded.");
        return;
    }

    // Check the file size.
    fseek(pFile, 0, SEEK_END);
    nFileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    // Check if file is too big
    if (nFileSize >= MeshLoader::MESH_LOADER_MAX_FILE_SIZE - 1)
    {
        LogError("OBJ is too large.");
        fclose(pFile);
        return;
    }

    // Clear the buffer that will hold contents of obj file
    memset(s_arFileBuffer, 0, nFileSize + 1);

    // Read from file and store in character buffer.
    fread(s_arFileBuffer, nFileSize, 1, pFile);

    // Close file, it is no longer needed.
    fclose(pFile);

    pStr = s_arFileBuffer;

    while (1)
    {
        // Find the next newline
        pNewLine  = strchr(pStr, '\n');

        // If a null pointer was returned, that means
        // a null terminator was hit.
        if (pNewLine == 0)
        {
            break;
        }

        // Parse the beginning of each line to count
        // the total number of vertices/UVs/normals/faces.
        if (pStr[0] == 'v' &&
            pStr[1] == ' ')
        {
            nNumVerts++;
        }
        else if (pStr[0] == 'v' &&
                 pStr[1] == 't')
        {
            nNumUVs++;
        }
        else if (pStr[0] == 'v' &&
                 pStr[1] == 'n')
        {
            nNumNormals++;
        }
        else if (pStr[0] == 'f')
        {
            nNumFaces++;
        }

        // Now move the pointer to the next line.
        pStr = pNewLine + 1;
    }
}

//*****************************************************************************
// GenerateVertexBuffer
//*****************************************************************************
void MeshLoader::GenerateVertexBuffer(int    nNumFaces,
                                      float* pVertices,
                                      float* pUVs,
                                      float* pNormals,
                                      int*   pFaces,
                                      float* pVB)
{
    if (pVB != 0)
    {
	    //Add vertex data for each face	
	    for(int i = 0; i < nNumFaces; i++)
	    {
		    //Vertex 1
		    pVB[i*24 + 0]  = pVertices[(pFaces[i*9 + 0]-1)*3 + 0];
		    pVB[i*24 + 1]  = pVertices[(pFaces[i*9 + 0]-1)*3 + 1];
		    pVB[i*24 + 2]  = pVertices[(pFaces[i*9 + 0]-1)*3 + 2];
		    pVB[i*24 + 3]  = pUVs[(pFaces[i*9 + 1]-1)*2 + 0];
		    pVB[i*24 + 4]  = pUVs[(pFaces[i*9 + 1]-1)*2 + 1];
		    pVB[i*24 + 5]  = pNormals[(pFaces[i*9 + 2]-1)*3 + 0];
		    pVB[i*24 + 6]  = pNormals[(pFaces[i*9 + 2]-1)*3 + 1];
		    pVB[i*24 + 7]  = pNormals[(pFaces[i*9 + 2]-1)*3 + 2];

		    //Vertex 2
		    pVB[i*24 + 8]  = pVertices[(pFaces[i*9 + 3]-1)*3 + 0];
		    pVB[i*24 + 9]  = pVertices[(pFaces[i*9 + 3]-1)*3 + 1];
		    pVB[i*24 + 10] = pVertices[(pFaces[i*9 + 3]-1)*3 + 2];
		    pVB[i*24 + 11] = pUVs[(pFaces[i*9 + 4]-1)*2 + 0];
		    pVB[i*24 + 12] = pUVs[(pFaces[i*9 + 4]-1)*2 + 1];
		    pVB[i*24 + 13] = pNormals[(pFaces[i*9 + 5]-1)*3 + 0];
		    pVB[i*24 + 14] = pNormals[(pFaces[i*9 + 5]-1)*3 + 1];
		    pVB[i*24 + 15] = pNormals[(pFaces[i*9 + 5]-1)*3 + 2];

		    //Vertex 3
		    pVB[i*24 + 16] = pVertices[(pFaces[i*9 + 6]-1)*3 + 0];
		    pVB[i*24 + 17] = pVertices[(pFaces[i*9 + 6]-1)*3 + 1];
		    pVB[i*24 + 18] = pVertices[(pFaces[i*9 + 6]-1)*3 + 2];
		    pVB[i*24 + 19] = pUVs[(pFaces[i*9 + 7]-1)*2 + 0];
		    pVB[i*24 + 20] = pUVs[(pFaces[i*9 + 7]-1)*2 + 1];
		    pVB[i*24 + 21] = pNormals[(pFaces[i*9 + 8]-1)*3 + 0];
		    pVB[i*24 + 22] = pNormals[(pFaces[i*9 + 8]-1)*3 + 1];
		    pVB[i*24 + 23] = pNormals[(pFaces[i*9 + 8]-1)*3 + 2];
	    }
    }
}

//*****************************************************************************
// LoadOBJ
//*****************************************************************************
unsigned int MeshLoader::LoadOBJ(const char*   pFileName, 
                                 unsigned int& nFaces)
{
    int i = 0;
    char* pStr     = 0;
    char* pNewLine = 0;
    int v = 0;
    int n = 0;
    int t = 0;
    int f = 0;
    unsigned int unVBO   = 0;
    float* pVertexBuffer = 0;

    int nNumVerts   = 0;
	int nNumFaces   = 0;
    int nNumNormals = 0;
	int nNumUVs     = 0;

    float* pVertices = 0;
    float* pNormals  = 0;
    float* pUVs      = 0;
    int*   pFaces    = 0;

    // Retrieve the counts for positions/UVs/normals/faces
	GetCounts(pFileName,
              nNumVerts,
              nNumUVs,
              nNumNormals,
              nNumFaces);

    // Set the number of faces output
	nFaces = nNumFaces;

    // Allocate arrays to store vertex data
    pVertices = new float[nNumVerts   * 3];
	pNormals  = new float[nNumNormals * 3];
	pUVs      = new float[nNumUVs     * 2];
	pFaces    = new   int[nNumFaces   * 9];

    // Set string pointer to beginning of file buffer
    pStr = s_arFileBuffer;

    // Loop until end of file, reading line by line.
    while (1)
    {
        pNewLine = strchr(pStr, '\n');

        if (pNewLine == 0)
        {
            break;
        }

        if (pStr[0] == 'v' &&
            pStr[1] == ' ')
        {
            // Extract X/Y/Z coordinates
            pStr = &pStr[2];
            pStr = strtok(pStr, " ");
            pVertices[v*3] = (float) atof(pStr);
            pStr = strtok(0," ");
            pVertices[v*3 + 1] = (float) atof(pStr);
            pStr = strtok(0, " \n");
            pVertices[v*3 + 2] = (float) atof(pStr);

            // Increase the number of vertices
            v++;
        }
        else if (pStr[0] == 'v' &&
                 pStr[1] == 't')
        {
            // Extract U/V coordinates
            pStr = &pStr[3];
            pStr = strtok(pStr, " ");
            pUVs[t*2] = (float) atof(pStr);
            pStr = strtok(0, " \n");
            pUVs[t*2 + 1] = (float) atof(pStr);

            // Increase number of texcoords
            t++;
        }
        else if(pStr[0] == 'v' &&
                pStr[1] == 'n')
        {
            pStr = &pStr[3];
            pStr = strtok(pStr, " ");
            pNormals[n*3] = (float) atof(pStr);
            pStr = strtok(0, " ");
            pNormals[n*3 + 1] = (float) atof(pStr);
            pStr = strtok(0, " \n");
            pNormals[n*3 + 2] = (float) atof(pStr);
            
            // Increase number of normals
            n++;
        }
        else if (pStr[0] == 'f')
        {
            pStr = &pStr[2];

            // Parse and convert all 9 indices.
            for (i = 0; i < 9; i++)
            {
                if (i == 0)
                {
                    pStr = strtok(pStr, " /\n");
                }
                else
                {
                    pStr = strtok(0, " /\n");
                }
                pFaces[f*9 + i] = atoi(pStr);
            }

            // Increase the face count
            f++;
        }

        // Now move the pointer to the next line.
        pStr = pNewLine + 1;
    }

    // From the separate arrays, create one big buffer
    // that has the vertex data interleaved.
	pVertexBuffer = new float[nNumFaces*24];
    GenerateVertexBuffer(nNumFaces,
                         pVertices,
                         pUVs,
                         pNormals,
                         pFaces,
                         pVertexBuffer);

    // Free arrays made to hold data.
	delete [] pVertices;
    pVertices = 0;
	delete [] pNormals;
    pNormals = 0;
	delete [] pUVs;
    pUVs = 0;
	delete [] pFaces;
    pFaces = 0;

    // Create the Vertex Buffer Object and fill it with vertex data
	glGenBuffers(1, &unVBO);
	glBindBuffer(GL_ARRAY_BUFFER, unVBO);
	glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * (nNumFaces)*24,
                 pVertexBuffer,
                 GL_STATIC_DRAW);

    // Delete the clientside buffer, since it is no longer needed.
    delete [] pVertexBuffer;
    pVertexBuffer = 0;

	return unVBO;
}

void MeshLoader::LoadAMF(const char*     pFileName,
                         char***         pAnimationNames,
                         unsigned int*   pAnimationCount,
                         unsigned int**  pKeyFrameCounts,
                         unsigned int*** pKeyFrameStarts,
                         unsigned int*** pFaces,
                         unsigned int*** pVBO)
{
    int   i              = 0;
    int   j              = 0;
    FILE* pFile          = 0;
    char* pBuffer        = 0;
    int   nFileSize      = 0;
    int   nNameLength    = 0;

    // Open the AMF file in read mode.
    pFile = fopen(pFileName, "r");
    
    // If file wasn't found, report error.
    if (pFile == 0)
    {
        LogError("Failed to load AMF file.");
        return;
    }

    // Check file size
    fseek(pFile, 0, SEEK_END);
    nFileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    // If file is too big, bail.
    if (nFileSize >= MeshLoader::MESH_LOADER_MAX_FILE_SIZE - 1)
    {
        LogError("AMF file is too large.");
        fclose(pFile);
        return;
    }

    // Allocate temporary buffer to hold AMF file
    pBuffer = new char[nFileSize + 1];
    memset(pBuffer, 0, nFileSize + 1);
    fread(pBuffer, nFileSize, 1, pFile);
    fclose(pFile);

    // Begin parsing file
    // Grab animation count
    pBuffer = strstr(pBuffer, "animationcount");
    pBuffer += sizeof("animationcount");
    *pAnimationCount = atoi(pBuffer);
    pBuffer = strchr(pBuffer, '\n');

    // Based on the animation count, allocate the name array
    // and the keyframe count array.
    *pAnimationNames = new char*[*pAnimationCount];
    *pKeyFrameCounts = new unsigned int[*pAnimationCount];
    *pKeyFrameStarts = new unsigned int*[*pAnimationCount];
    *pVBO            = new unsigned int*[*pAnimationCount];
    *pFaces          = new unsigned int*[*pAnimationCount];
    

    // Loop through animations
    for (i = 0; i < *pAnimationCount; i++)
    {
        // Find the next animation section
        strstr(pBuffer, "animation ");
        pBuffer += sizeof("animation ");
        pBuffer = strtok(pBuffer, " \n");

        // Allocate the new string
        nNameLength = strlen(pBuffer);
        if (nNameLength > MESH_LOADER__ANIMATION_NAME_SIZE_WARNING)
        {
            // Log a warning to hint at incorrect file parsing.
            LogWarning("Animation name is very long in LoadAMF().");
        }
        (*pAnimationNames)[i] = new char[nNameLength + 1];
        memset((*pAnimationNames)[i], 0, nNameLength + 1);
        strcpy((*pAnimationNames)[i], pBuffer);

        // Find the framecount for this animation
        pBuffer = strstr(pBuffer, "keyframecount ");
        pBuffer += sizeof("keyframecount ");
        (*pKeyFrameCounts)[i] = atoi(pBuffer);

        // Now that we know how many keyframes are in the current animation
        // we can allocate the arrays for VBOs, Faces, and KeyFrameStarts
        (*pKeyFrameStarts)[i] = new unsigned int[(*pKeyFrameCounts)[i]];
        (*pFaces)[i]          = new unsigned int[(*pKeyFrameCounts)[i]];
        (*pVBO)[i]            = new unsigned int[(*pKeyFrameCounts)[i]];

        // Begin looping through each of the keyframes
        for (j = 0; j < (*pKeyFrameCounts)[i]; j++)
        {
            pBuffer = strstr(pBuffer, "keyframe ");
            pBuffer += sizeof ("keyframe ");
            strtok(pBuffer, " ");
            (*pKeyFrameStarts)[i][j] = atoi(pBuffer);

            // Now point to the file name
            pBuffer = strtok(0, " \n");

            // Read the individual OBJ file
            (*pVBO)[i][j] = LoadOBJ(pBuffer, (*pFaces)[i][j]);
        }
    }
}