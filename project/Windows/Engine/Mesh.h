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

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Mesh();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    virtual ~Mesh();

    //## **********************************************************************
    //## GetType
    //##
    //## Returns the type of mesh. Refer to MeshType enum.
    //##
    //## Returns:
    //##   int - mesh type enumeration value. See above class.
    //## **********************************************************************
    int GetType();

    //## **********************************************************************
    //## SetRenderState
    //##
    //## Sets the opengl state for rendering a mesh. This is a pure virtual
    //## function and must be implemented by any Mesh Child.
    //##
    //## Input:
    //##   pScene - pointer to the scene that is rendering.
    //##   hProg  - handle to shader program that is already bound.
    //## **********************************************************************
    virtual void SetRenderState(void*        pScene,
                                unsigned int hProg) = 0;

    //## **********************************************************************
    //## SetVertexCount
    //##
    //## Sets the number of vertices that the static mesh contains.
    //## 
    //## Input:
    //##   nCount - the number of vertices to draw.
    //## **********************************************************************
    void SetVertexCount(int nCount);

    //## **********************************************************************
    //## GetVertexCount
    //##
    //## Returns number of vertices to be rendered.
    //## 
    //## Returns:
    //##   int - number of vertices in static mesh.
    //## **********************************************************************
    int GetVertexCount();

protected:

    //## Type of mesh (Animated, Static, etc)
    int m_nType;

    //## Number of vertices to render
    int m_nVertexCount;
};

#endif
