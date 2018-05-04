
/************************************************************************
*    FILE NAME:       shadermanager.cpp
*
*    DESCRIPTION:     shader class singleton
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <managers/shadermanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <memory>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CShaderMgr::CShaderMgr() :
    m_pCurrentShaderData(nullptr),
    m_currentVertexAttribCount(0)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CShaderMgr::~CShaderMgr()
{
    // Free all textures in all groups
    for( m_Iter = m_shaderMap.begin();
         m_Iter != m_shaderMap.end();
         ++m_Iter )
    {
        m_Iter->second.free();
    }
}


/************************************************************************
*    DESC:  Load the shader from xml file path
************************************************************************/
void CShaderMgr::load( const std::string & filePath )
{
    // Open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "shaderLst" );
    if( mainNode.isEmpty() )
    {
        throw NExcept::CCriticalException("Shader Load Error!",
            boost::str( boost::format("Shader XML empty (%s).\n\n%s\nLine: %s")
                % filePath % __FUNCTION__ % __LINE__ ));
    }

    for( int i = 0; i < mainNode.nChildNode(); ++i )
        createShader( mainNode.getChildNode(i) );
}


/************************************************************************
*    DESC:  Create the shader
************************************************************************/
void CShaderMgr::createShader( const XMLNode & node )
{
    const std::string shaderStrId = node.getAttribute("Id");

    const XMLNode vertexNode = node.getChildNode("vertDataLst");
    const XMLNode fragmentNode = node.getChildNode("fragDataLst");

    // Check that the name doesn't exist
    auto iter = m_shaderMap.find( shaderStrId );
    if( iter != m_shaderMap.end() )
    {
        throw NExcept::CCriticalException("Shader Load Error!",
            boost::str( boost::format("Shader of this name already exists (%s).\n\n%s\nLine: %s")
                % shaderStrId % __FUNCTION__ % __LINE__ ));
    }

    // Insert the new shader
    // Get an iterator to the newly added shader data.
    // This is to aid in cleanup in the event of an error
    m_Iter = m_shaderMap.emplace( shaderStrId, CShaderData() ).first;

    // Create the vertex shader
    createShader( GL_VERTEX_SHADER, vertexNode.getAttribute("file") );

    // Create the vertex shader
    createShader( GL_FRAGMENT_SHADER, fragmentNode.getAttribute("file") );

    // Link the shader
    createProgram();

    // Bind the attribute location by index defined in shader config file
    bindAttributeLocation( vertexNode );

    // Link the shader program
    linkProgram();

    // Set all the shader attributes
    locateShaderVariables( vertexNode, fragmentNode );

    // Send out a signal to init this shader
    m_initShaderSignal( shaderStrId );
}


/************************************************************************
*    DESC:  Create the shader
************************************************************************/
void CShaderMgr::createShader( uint32_t shaderType, const std::string & filePath )
{
    // Create the shader
    uint32_t shaderID = glCreateShader( shaderType );
    if( shaderID == 0 )
    {
        throw NExcept::CCriticalException("Create Shader Error!",
            boost::str( boost::format("Error creating shader (%s).\n\n%s\nLine: %s")
                % filePath % __FUNCTION__ % __LINE__ ));
    }

    if( shaderType == GL_VERTEX_SHADER )
        m_Iter->second.setVertexID( shaderID );
    else
        m_Iter->second.setFragmentID( shaderID );

    // Load the shader from file
    std::shared_ptr<char> spChar = NGenFunc::FileToBuf( filePath );
    const char * pChar = spChar.get();
    glShaderSource( shaderID, 1, &pChar, nullptr );

    // Compile shader source
    glCompileShader( shaderID );

    // Check shader for errors
    int32_t success( GL_FALSE );
    glGetShaderiv( shaderID, GL_COMPILE_STATUS, &success );
    if( success != GL_TRUE )
    {
        int32_t maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        std::unique_ptr<char[]> upError( new char[maxLength] );
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, upError.get());

        throw NExcept::CCriticalException("Create Vertex Shader Error!",
            boost::str( boost::format("Error compiling vertex shader (%s).\n\n%s.\n\n%s\nLine: %s")
                % filePath % upError.get() % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Create the shader program
************************************************************************/
void CShaderMgr::createProgram()
{
    // Create the program - OpenGL shaders are called programs
    m_Iter->second.setProgramID( glCreateProgram() );
    if( m_Iter->second.getProgramID() == 0 )
    {
        throw NExcept::CCriticalException("Create Shader Error!",
            boost::str( boost::format("Error creating shader (%s).\n\n%s\nLine: %s")
                % m_Iter->first % __FUNCTION__ % __LINE__ ));
    }

    // Attach shaders to the program
    glAttachShader( m_Iter->second.getProgramID(), m_Iter->second.getVertexID() );
    glAttachShader( m_Iter->second.getProgramID(), m_Iter->second.getFragmentID() );
}


/************************************************************************
*    DESC:  Bind the attribute location
*           NOTE: This function is called if 0 or greater is specified in
*           the shader config to allow the defining of the attribute index
************************************************************************/
void CShaderMgr::bindAttributeLocation( const XMLNode & vertexNode )
{
    // Get the location ID for the vertex variables
    for( int i = 0; i < vertexNode.nChildNode(); ++i )
    {
        const XMLNode node = vertexNode.getChildNode(i);

        if( node.isAttributeSet( "location" ) )
        {
            const int location = std::atoi( node.getAttribute("location") );
            const std::string attributeName = node.getAttribute("name");

            // Save the attribute for use later
            m_Iter->second.setAttributeLocation( attributeName, location );

            // Bind a constant attribute location for positions of vertices
            glBindAttribLocation( m_Iter->second.getProgramID(), (uint32_t)location, attributeName.c_str() );

            uint32_t error(glGetError());
            if( error != GL_NO_ERROR)
                throw NExcept::CCriticalException("Create Shader Error!",
                    boost::str( boost::format("Error binding attribute (%s).\n\n%s\nLine: %s")
                        % attributeName % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
*    DESC:  Link the shader program
************************************************************************/
void CShaderMgr::linkProgram()
{
    // Link shader program
    glLinkProgram( m_Iter->second.getProgramID() );

    // Check for errors
    int32_t success( GL_TRUE );
    glGetProgramiv( m_Iter->second.getProgramID(), GL_LINK_STATUS, &success );
    if( success != GL_TRUE )
    {
        throw NExcept::CCriticalException("Link Shader Error!",
            boost::str( boost::format("Error linking shader (%s).\n\n%s\nLine: %s")
                % m_Iter->first % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Find the location of the custom shader variables
************************************************************************/
void CShaderMgr::locateShaderVariables(
    const XMLNode & vertexNode,
    const XMLNode & fragmentNode )
{
    // Get the location ID for the vertex attributes and uniforms
    for( int i = 0; i < vertexNode.nChildNode(); ++i )
    {
        const XMLNode node = vertexNode.getChildNode(i);

        if( !node.isAttributeSet( "location" ) )
            getUniformLocation( node );
    }

    // Get the location ID for the fragment variables
    for( int i = 0; i < fragmentNode.nChildNode(); ++i )
        getUniformLocation( fragmentNode.getChildNode(i) );
}


/************************************************************************
*    DESC:  Get the uniform location
************************************************************************/
void CShaderMgr::getUniformLocation( const XMLNode & node )
{
    std::string name = node.getAttribute("name");

    int32_t location = glGetUniformLocation( m_Iter->second.getProgramID(), name.c_str() );

    m_Iter->second.setUniformLocation( name, location );

    if( location < 0 )
    {
        throw NExcept::CCriticalException("Shader Uniform Location Error!",
            boost::str( boost::format("Error Uniform Location (%s) not found (%s).\n\n%s\nLine: %s")
                % name % m_Iter->first % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Get the shader data
************************************************************************/
CShaderData & CShaderMgr::getShaderData( const std::string & shaderId )
{
    // See if we can find the shader
    m_Iter = m_shaderMap.find( shaderId );
    if( m_Iter == m_shaderMap.end() )
    {
        throw NExcept::CCriticalException("Shader Data Error!",
            boost::str( boost::format("Shader of this name doesn't exists (%s).\n\n%s\nLine: %s")
                % shaderId % __FUNCTION__ % __LINE__ ));
    }

    return m_Iter->second;
}


/************************************************************************
*    DESC:  Function call used to manage what shader is currently bound.
*           This insures that we don't keep rebinding the same shader
*           Note: Vertex Attribute Arrays should always be defined in
*           the shader.cfg starting from 0 to max count. Makes live easy
************************************************************************/
void CShaderMgr::bind( CShaderData * pShaderData )
{
    if( m_pCurrentShaderData != pShaderData )
    {
        if( m_pCurrentShaderData == nullptr )
        {
            m_currentVertexAttribCount = pShaderData->getVertexAttribCount();

            for( size_t i = 0; i < m_currentVertexAttribCount; ++i )
                glEnableVertexAttribArray(i);
        }
        else if( m_currentVertexAttribCount != pShaderData->getVertexAttribCount() )
        {
            const size_t attribCount( pShaderData->getVertexAttribCount() );

            if( m_currentVertexAttribCount < attribCount )
            {
                for( size_t i = m_currentVertexAttribCount; i < attribCount; ++i )
                    glEnableVertexAttribArray(i);
            }
            else
            {
                for( size_t i = attribCount; i < m_currentVertexAttribCount; ++i )
                    glDisableVertexAttribArray(i);
            }

            m_currentVertexAttribCount = attribCount;
        }

        // save the current binding
        m_pCurrentShaderData = pShaderData;

        // Have OpenGL bind this shader now
        glUseProgram( pShaderData->getProgramID() );
    }
}


/************************************************************************
*    DESC:  Unbind the shader and reset the flag
************************************************************************/
void CShaderMgr::unbind()
{
    for( size_t i = 0; i < m_currentVertexAttribCount; ++i )
        glDisableVertexAttribArray(i);

    m_pCurrentShaderData = nullptr;
    m_currentVertexAttribCount = 0;
    glUseProgram( 0 );
}


/************************************************************************
*    DESC:  Free the shader
************************************************************************/
void CShaderMgr::freeShader( const std::string & shaderId )
{
    // See if we can find the shader
    m_Iter = m_shaderMap.find( shaderId );
    if( m_Iter != m_shaderMap.end() )
    {
        m_Iter->second.free();

        // Erase this shader
        m_shaderMap.erase( m_Iter );
    }
}


/************************************************************************
*    DESC:  Connect to the Init Shader signal
************************************************************************/
void CShaderMgr::connect_initShader( const InitShaderSignal::slot_type & slot )
{
    m_initShaderSignal.connect(slot);
}


/************************************************************************
*    DESC:  Set the shader member variable
************************************************************************/
void CShaderMgr::setShaderColor( const std::string & shaderId, const std::string & locationId, const CColor & color )
{
    setShaderColor( getShaderData( shaderId ), locationId, color );
}

void CShaderMgr::setShaderColor( const std::string & shaderId, const std::string & locationId, float r, float g, float b, float a )
{
    setShaderColor( getShaderData( shaderId ), locationId, CColor(r,g,b,a) );
}

void CShaderMgr::setShaderColor( CShaderData & shaderData, const std::string & locationId, const CColor & color )
{
    // Check for the shader location
    if( shaderData.hasUniformLocation( locationId ) )
    {
        // Get the location of the additive
        int32_t location = shaderData.getUniformLocation( locationId );

        // Bind the shader so that we can change the value of the member
        bind( &shaderData );

        // Set the color
        glUniform4fv( location, 1, (float *)&color );

        // Unbind now that we are done
        unbind();
    }
}


/************************************************************************
*    DESC:  Set all the shaders using this color location
************************************************************************/
void CShaderMgr::setAllShaderColor( const std::string & locationId, const CColor & color )
{
    for( auto & iter : m_shaderMap )
        setShaderColor( iter.second, locationId, color );
}

void CShaderMgr::setAllShaderColor( const std::string & locationId, float r, float g, float b, float a )
{
    for( auto & iter : m_shaderMap )
        setShaderColor( iter.second, locationId, CColor(r,g,b,a) );
}
