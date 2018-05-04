
/************************************************************************
*    FILE NAME:       shaderdata.cpp
*
*    DESCRIPTION:     shader data class
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <common/shaderdata.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CShaderData::CShaderData()
    : m_programID(0), m_vertexID(0), m_fragmentID(0)
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CShaderData::~CShaderData()
{
    // DO NOT USE FOR FREEING DATA!

    // This class is for holding and copying data. Freeing OpenGL assets
    // is done from the out side with the Free() call.
}


/************************************************************************
*    DESC:  Set/Get the program ID
************************************************************************/
void CShaderData::setProgramID( uint32_t location )
{
    m_programID = location;
}

uint32_t CShaderData::getProgramID() const
{
    return m_programID;
}


/************************************************************************
*    DESC:  Set/Get the vertex ID
************************************************************************/
void CShaderData::setVertexID( uint32_t location )
{
    m_vertexID = location;
}

uint32_t CShaderData::getVertexID() const
{
    return m_vertexID;
}


/************************************************************************
*    DESC:  Set/Get the fragment ID
************************************************************************/
void CShaderData::setFragmentID( uint32_t location )
{
    m_fragmentID = location;
}

uint32_t CShaderData::getFragmentID() const
{
    return m_fragmentID;
}


/************************************************************************
*    DESC:  Set/Get the attribute location
************************************************************************/
void CShaderData::setAttributeLocation( const std::string & name, int32_t location )
{
    auto iter = m_attributeMap.find( name );
    if( iter != m_attributeMap.end() )
    {
        throw NExcept::CCriticalException("Set Attribute Error!",
            boost::str( boost::format("Shader attribute allready exists (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    m_attributeMap.emplace( name, location );
}

int32_t CShaderData::getAttributeLocation( const std::string & name ) const
{
    auto iter = m_attributeMap.find( name );
    if( iter == m_attributeMap.end() )
    {
        throw NExcept::CCriticalException("Get Attribute Error!",
            boost::str( boost::format("Shader attribute does not exist (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Set/Get the uniform location
************************************************************************/
void CShaderData::setUniformLocation( const std::string & name, int32_t location )
{
    auto iter = m_uniformMap.find( name );
    if( iter != m_uniformMap.end() )
    {
        throw NExcept::CCriticalException("Set Attribute Error!",
            boost::str( boost::format("Shader attribute allready exists (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    m_uniformMap.emplace( name, location );
}

int32_t CShaderData::getUniformLocation( const std::string & name ) const
{
    auto iter = m_uniformMap.find( name );
    if( iter == m_uniformMap.end() )
    {
        throw NExcept::CCriticalException("Get Uniform Error!",
            boost::str( boost::format("Shader uniform does not exists (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Check for the uniform location
************************************************************************/
bool CShaderData::hasUniformLocation( const std::string & name ) const
{
    auto iter = m_uniformMap.find( name );
    if( iter != m_uniformMap.end() )
        return true;

    return false;
}


/************************************************************************
*    DESC:  Get the vertex attribute count
************************************************************************/
size_t CShaderData::getVertexAttribCount()
{
    return m_attributeMap.size();
}


/************************************************************************
*    DESC:  Free the data
************************************************************************/
void CShaderData::free()
{
    // Detach the shaders
    if( (m_vertexID > 0) && (m_fragmentID > 0) && (m_programID > 0) )
    {
        glDetachShader(m_programID, m_vertexID);
        glDetachShader(m_programID, m_fragmentID);
    }

    // Delete the shaders
    if( m_vertexID > 0 )
        glDeleteShader(m_vertexID);

    if( m_fragmentID > 0 )
        glDeleteShader(m_fragmentID);

    // Delete the shader program
    if( m_programID > 0 )
        glDeleteProgram(m_programID);

    clear();
}


/************************************************************************
*    DESC:  Clear the data
************************************************************************/
void CShaderData::clear()
{
    m_programID = m_vertexID = m_fragmentID = 0;

    m_attributeMap.clear();
    m_uniformMap.clear();
}
