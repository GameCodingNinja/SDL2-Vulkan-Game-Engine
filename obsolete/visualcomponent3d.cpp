
/************************************************************************
*    FILE NAME:       visualcomponent3d.cpp
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

#if !(defined(__IOS__) || defined(__ANDROID__) || defined(__arm__))
// Glew dependencies (have to be defined first)
#include <GL/glew.h>
#endif

// Physical component dependency
#include <3d/visualcomponent3d.h>

// Game lib dependencies
#include <objectdata/objectvisualdata3d.h>
#include <managers/shadermanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/texturemanager.h>
#include <common/vertex3d.h>
#include <common/shaderdata.h>
#include <system/device.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <utilities/statcounter.h>

// AngelScript lib dependencies
#include <angelscript.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <memory>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponent3D::CVisualComponent3D( const CObjectVisualData3D & visualData ) :
	m_visualData( visualData ),
	m_pShaderData( nullptr ),
	m_vertexLocation( -1 ),
	m_uvLocation( -1 ),
	m_text0Location( -1 ),
	m_colorLocation( -1 ),
	m_matrixLocation( -1 ),
	m_normalMatrixLocation( -1 ),
	m_meshVec( visualData.GetMesh3D() ),
	m_color( visualData.GetColor() ),
        m_VERTEX_BUF_SIZE( (visualData.GetMesh3D().back().m_textureVec.empty()) ? sizeof(CVertex3D_no_txt) : sizeof(CVertex3D) )
{
    if( visualData.IsActive() )
    {
        m_pShaderData = &CShaderMgr::Instance().GetShaderData( visualData.GetShaderID() );

        m_vertexLocation = m_pShaderData->GetAttributeLocation( "in_position" );
        m_normalLocation = m_pShaderData->GetAttributeLocation( "in_normal" );
        m_matrixLocation = m_pShaderData->GetUniformLocation( "cameraViewProjMatrix" );
        m_normalMatrixLocation = m_pShaderData->GetUniformLocation( "normalMatrix" );
        m_colorLocation = m_pShaderData->GetUniformLocation( "color" );

        if( !m_meshVec.back().m_textureVec.empty() )
        {
            m_uvLocation = m_pShaderData->GetAttributeLocation( "in_uv" );
            m_text0Location = m_pShaderData->GetUniformLocation( "text0" );
        }
    }

}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CVisualComponent3D::~CVisualComponent3D()
{
}   // destructor


/************************************************************************
*    desc:  do the render
************************************************************************/
void CVisualComponent3D::Render( const CMatrix & matrix, const CMatrix & normalMatrix )
{
    for( auto & meshIter : m_meshVec )
    {
        // Increment our stat counter to keep track of what is going on.
        CStatCounter::Instance().IncDisplayCounter();

        // Bind the VBO and IBO
        CVertBufMgr::Instance().Bind( meshIter.m_vbo, meshIter.m_ibo );

        // Bind the shader. This must be done first
        CShaderMgr::Instance().Bind( m_pShaderData );

        // Setup the vertex attribute shader data
        glVertexAttribPointer( m_vertexLocation, 3, GL_FLOAT, GL_FALSE, m_VERTEX_BUF_SIZE, (GLvoid*)0 );

        // Setup the normal attribute shade data
        glVertexAttribPointer( m_normalLocation, 3, GL_FLOAT, GL_FALSE, m_VERTEX_BUF_SIZE, (GLvoid*)12 );

        // Enable the UV attribute shade data
        if( m_uvLocation > -1 )
        {
            // Bind the texture
            for( auto & txtIter : meshIter.m_textureVec )
            {
                CTextureMgr::Instance().Bind( txtIter.m_id );
                glUniform1i( m_text0Location, (int)txtIter.m_type); // 0 = TEXTURE0
            }

            // Setup the uv attribute shade data
            glVertexAttribPointer( m_uvLocation, 2, GL_FLOAT, GL_FALSE, m_VERTEX_BUF_SIZE, (GLvoid*)24 );
        }

        // Send the color to the shader
        glUniform4fv( m_colorLocation, 1, (GLfloat*)&m_color );

        glUniformMatrix4fv( m_matrixLocation, 1, GL_FALSE, matrix() );
        glUniformMatrix4fv( m_normalMatrixLocation, 1, GL_FALSE, normalMatrix() );

        // Render it
        glDrawElements( GL_TRIANGLES, meshIter.m_iboCount, GL_UNSIGNED_SHORT, nullptr );
    }

}   // Render


/************************************************************************
*    desc:  Set/Get the color
************************************************************************/
void CVisualComponent3D::SetColor( const CColor & color )
{
    m_color = color;

}   // SetColor

void CVisualComponent3D::SetRGBA( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_color.Set( r, g, b, a );

}   // SetRGBA

const CColor & CVisualComponent3D::GetColor() const
{
    return m_color;

}   // GetColor


/************************************************************************
*    desc:  Set/Get the alpha
************************************************************************/
void CVisualComponent3D::SetAlpha( float alpha )
{
    m_color.a = alpha;

}   // SetAlpha

float CVisualComponent3D::GetAlpha() const
{
    return m_color.a;

}   // GetAlpha


/************************************************************************
*    desc:  Set the default color
************************************************************************/
void CVisualComponent3D::SetDefaultColor()
{
    m_color = m_visualData.GetColor();

}   // SetDefaultColor
