
/************************************************************************
*    FILE NAME:       ojectvisualdata3d.h
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

#ifndef __object_visual_data_3d_h__
#define __object_visual_data_3d_h__

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
    void loadFromNode( const XMLNode & objectNode );
    
    // Load the mesh data from file
    void loadMeshData( const std::string & group );

    // Create the object from data
    void createFromData( const std::string & group );

    // Get the name of the shader ID
    const std::string & getShaderID() const;

    // Get the color
    const CColor & getColor() const;

    // Whether or not the visual tag was specified
    bool isActive() const;

    // Get the mesh3d vector
    const CMesh3D & getMesh3D() const;
    
private:

    CMesh3D m_mesh3d;

    // Name of the shader
    std::string m_shaderID;

    // Initial color of the object
    CColor m_color;

    // mesh file path
    std::string m_meshFile;

};

#endif  // __object_visual_data_2d_h__
