#ifndef MESH_H
#define MESH_H

enum MeshType
{
    MESH_NO_TYPE  = -1,
    MESH_STATIC   =  0,
    MESH_ANIMATED =  1,
    MESH_SKELETAL =  2
};

class Mesh
{
    
public:

    Mesh();

    ~Mesh();

    int GetType();

    virtual void SetRenderState(void*        pScene,
                                unsigned int hProg) = 0;

protected:

    int m_nType;

};

#endif
