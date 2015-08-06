#ifndef TERRAIN_H
#define TERRAIN_H

class Terrain
{

public:

    Terrain();

    ~Terrain();

    void LoadBMP(const char* pFileName);

    enum TerrainEnum
    {
        TERRAIN_MAX_SIZE = 1024,

        VERTICES_PER_TRIANGLE = 3
    };

private:

    void GenerateVertexArrays();

    int m_nSize;

    int m_hEBO;

    int m_hPositionVBO;
    int m_hTexCoordVBO;
    int m_hNormalVBO;

    float* m_arHeights;

};


#endif
