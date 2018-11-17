
/************************************************************************
*    FILE NAME:       visualcomponent3d.h
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

#ifndef __visual_component_3d_h__
#define __visual_component_3d_h__

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/color.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/fontproperties.h>
#include <common/mesh3d.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class CObjectVisualData3D;
class CFont;
class CShaderData;

class CVisualComponent3D : boost::noncopyable
{
public:

    // Constructor
    CVisualComponent3D( const CObjectVisualData3D & visualData );

    // Destructor
    ~CVisualComponent3D();

    // do the render
    void Render( const CMatrix & matrix, const CMatrix & normalMatrix );

    // Set/Get the color
    void SetColor( const CColor & color );
    void SetRGBA( float r, float g, float b, float a );
    const CColor & GetColor() const;
    void SetDefaultColor();

    // Set/Get the alpha
    void SetAlpha( float alpha );
    float GetAlpha() const;

private:

    // Reference to object visual data
    const CObjectVisualData3D & m_visualData;
    
    // Shader data pointer - We DON'T own this pointer, don't free
    CShaderData * m_pShaderData;

    // shader location data
    GLint m_vertexLocation;
    GLint m_normalLocation;
    GLint m_uvLocation;
    GLint m_text0Location;
    GLint m_colorLocation;
    GLint m_matrixLocation;
    GLint m_normalMatrixLocation;

    // Copy of 3D mesh data
    const std::vector<CMesh3D> m_meshVec;

    // Color
    CColor m_color;
    
    // Vertex buffer sizes
    const GLbyte m_VERTEX_BUF_SIZE;
};

#endif  // __visual_component_3d_h__

