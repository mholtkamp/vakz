#include "Terrain.h"
#include "ImageLoader.h"
#include "Log.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
Terrain::Terrain()
{
    m_nSize        = 0;
    m_hEBO         = 0;
    m_hPositionVBO = 0;
    m_hTexCoordVBO = 0;
    m_hNormalVBO   = 0;
    m_arHeights    = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Terrain::~Terrain()
{
    if (m_arHeights != 0)
    {
        delete [] m_arHeights;
        m_arHeights = 0;
    }

}

//*****************************************************************************
// LoadBMP
//*****************************************************************************
void Terrain::LoadBMP(const char* pFileName)
{
    int i         = 0;
    int j         = 0;
    int nWidth    = 0;
    int nHeight   = 0;
    short sBPP    = 0;
    float fHeight = 0.0f;
    unsigned char* pData = 0;

    pData = ImageLoader::LoadBMP(pFileName,
                                 nWidth,
                                 nHeight,
                                 sBPP);

    // Error checks
    if (nWidth != nHeight)
    {
        LogError("Terrain heightmap width and height must be equal.");
        return;
    }
    if (nWidth > TERRAIN_MAX_SIZE)
    {
        LogError("Terrain heightmap is too big.");
        return;
    }

    // Set the size, since we know height and width are equal.
    m_nSize = nWidth;

    m_arHeights = new float[m_nSize * m_nSize];

    // The terrain heightmaps origin is at the bottom left,
    // so y-values need to be reversed.
    for (i = m_nSize - 1; i >= 0; i++)
    {
        for (j = 0; j < m_nSize; j++)
        {
             fHeight = static_cast<float>((pData[i * m_nSize + j] - 128));
             m_arHeights[(m_nSize - i - 1)*m_nSize + j] = fHeight;
        }
    }

    // Delete the unsigned char array.
    delete [] pData;
    pData = 0;

    // Now that the heightmap is known, we can generate the 
    // position, normal, and texture coordinate arrays.
    GenerateVertexArrays();
}

//*****************************************************************************
// GenerateVertexArrays
//*****************************************************************************
void Terrain::GenerateVertexArrays()
{
    int i = 0;
    int j = 0;

    float* arPosition = new float[m_nSize * m_nSize * 3];
    float* arTexCoord = new float[m_nSize * m_nSize * 2];
    float* arNormal   = new float[m_nSize * m_nSize * 3];

    for (i = 0; i < m_nSize; i++)
    {
        for (j = 0; j < m_nSize; j++)
        {
            arPosition[i * m_nSize + j]     = static_cast<float>(j);
            arPosition[i * m_nSize + j + 1] = m_arHeights[i * m_nSize + j];
            arPosition[i * m_nSize + j + 2] = static_cast<float>(i);

            arTexCoord[i * m_nSize + j]     = static_cast<float>(j);
            arTexCoord[i * m_nSize + j + 1] = static_cast<float>(i);
        }
    }


    delete [] arPosition;
    delete [] arTexCoord;
    delete [] arNormal;
}
