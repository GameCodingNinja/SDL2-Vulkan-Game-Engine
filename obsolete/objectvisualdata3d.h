
/************************************************************************
*    FILE NAME:       ojectvisualdata3d.h
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

#ifndef __object_visual_data_3d_h__
#define __object_visual_data_3d_h__

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Game lib dependencies
#include <common/size.h>
#include <common/color.h>
#include <common/rect.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/face2d.h>
#include <common/scaledframe.h>
#include <common/texture.h>
#include <common/mesh3d.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward Declarations
struct XMLNode;

class CObjectVisualData3D
{
public:

    // Constructor/Destructor
    CObjectVisualData3D();
    ~CObjectVisualData3D();

    // Load thes object data from node
    void LoadFromNode( const XMLNode & objectNode );

    // Create the object from data
    void CreateFromData( const std::string & group );

    // Get the name of the shader ID
    const std::string & GetShaderID() const;

    // Get the color
    const CColor & GetColor() const;

    // Whether or not the visual tag was specified
    bool IsActive() const;

    // Get the mesh3d vector
    const std::vector<CMesh3D> & GetMesh3D() const;
    
private:

    std::vector<CMesh3D> m_meshVec;

    // Name of the shader
    std::string m_shaderID;

    // Initial color of the object
    CColor m_color;

    // mesh file path
    std::string m_meshFile;

};

#endif  // __object_visual_data_2d_h__
