
/************************************************************************
*    FILE NAME:       visualcomponent3d.cpp
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

// Physical component dependency
#include <3d/visualcomponent3d.h>

// Game lib dependencies
#include <objectdata/objectdata3d.h>
#include <objectdata/objectvisualdata3d.h>
#include <managers/shadermanager.h>
#include <managers/vertexbuffermanager.h>
#include <common/vertex3d.h>
#include <common/shaderdata.h>
#include <system/device.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <utilities/statcounter.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <memory>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CVisualComponent3D::CVisualComponent3D( const CObjectData3D & objectData ) :
    iVisualComponent( objectData ),
    m_rObjectData( objectData ),
    m_pShaderData( nullptr ),
    m_vertexLocation( -1 ),
    m_uvLocation( -1 ),
    m_text0Location( -1 ),
    m_colorLocation( -1 ),
    m_matrixLocation( -1 ),
    m_normalMatrixLocation( -1 )
    //m_mesh3d( visualData.getMesh3D() ),
    //m_VERTEX_BUF_SIZE( visualData.getMesh3D().meshEmpty() || visualData.getMesh3D().textEmpty() ? sizeof(CVertex3D_no_txt) : sizeof(CVertex3D) )
{
    /*if( visualData.isActive() )
    {
        m_pShaderData = &CShaderMgr::Instance().getShaderData( visualData.getShaderID() );

        m_vertexLocation = m_pShaderData->getAttributeLocation( "in_position" );
        m_normalLocation = m_pShaderData->getAttributeLocation( "in_normal" );
        m_matrixLocation = m_pShaderData->getUniformLocation( "cameraViewProjMatrix" );
        m_normalMatrixLocation = m_pShaderData->getUniformLocation( "normalMatrix" );
        m_colorLocation = m_pShaderData->getUniformLocation( "color" );

        if( !m_mesh3d.back().m_textureVec.empty() )
        {
            m_uvLocation = m_pShaderData->getAttributeLocation( "in_uv" );
            m_text0Location = m_pShaderData->getUniformLocation( "text0" );
        }
    }*/
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CVisualComponent3D::~CVisualComponent3D()
{
}


/************************************************************************
*    DESC:  do the render
************************************************************************/
void CVisualComponent3D::render( const CMatrix & matrix, const CMatrix & normalMatrix )
{
    /*for( auto & meshIter : m_mesh3d.getMeshVec() )
    {
        // Increment our stat counter to keep track of what is going on.
        CStatCounter::Instance().incDisplayCounter();

        // Bind the VBO and IBO
        CVertBufMgr::Instance().bind( meshIter.m_vbo, meshIter.m_ibo );

        // Bind the shader. This must be done first
        CShaderMgr::Instance().bind( m_pShaderData );

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
                //CTextureMgr::Instance().bind( txtIter.m_id );
                glUniform1i( m_text0Location, (int)txtIter.m_type); // 0 = TEXTURE0
            }

            // Setup the uv attribute shade data
            glVertexAttribPointer( m_uvLocation, 2, GL_FLOAT, GL_FALSE, m_VERTEX_BUF_SIZE, (GLvoid*)24 );
        }

        // Send the color to the shader
        glUniform4fv( m_colorLocation, 1, (float*)&m_color );

        glUniformMatrix4fv( m_matrixLocation, 1, GL_FALSE, matrix() );
        glUniformMatrix4fv( m_normalMatrixLocation, 1, GL_FALSE, normalMatrix() );

        // Render it
        glDrawElements( GL_TRIANGLES, meshIter.m_iboCount, GL_UNSIGNED_SHORT, nullptr );
    }*/
}

