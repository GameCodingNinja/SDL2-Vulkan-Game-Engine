
/************************************************************************
*    FILE NAME:       shadermanager.h
*
*    DESCRIPTION:     shader class singleton
************************************************************************/

#ifndef __shader_manager_h__
#define __shader_manager_h__

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <common/shaderdata.h>
#include <common/color.h>

// Boost lib dependencies
#include <boost/signals2.hpp>

// Standard lib dependencies
#include <string>
#include <map>

class CShaderMgr
{
public:
    
    // Boost signal definition
    typedef boost::signals2::signal<void (const std::string)> InitShaderSignal;

    // Get the instance of the singleton class
    static CShaderMgr & Instance()
    {
        static CShaderMgr shader;
        return shader;
    }

    // Load the shader from xml file path
    void load( const std::string & filePath );

    // Get the shader data
    CShaderData & getShaderData( const std::string & shaderId );

    // Function call used to manage what shader is currently bound.
    void bind( CShaderData * pShaderData );

    // Unbind the shader and reset the flag
    void unbind();
    
    // Free the shader
    void freeShader( const std::string & shaderId );
    
    // Set the shader member variable
    void setShaderColor( const std::string & shaderId, const std::string & locationId, const CColor & color );
    void setShaderColor( const std::string & shaderId, const std::string & locationId, float r, float g, float b, float a );
    
    // Set all the shaders using this color location
    void setAllShaderColor( const std::string & locationId, const CColor & color );
    void setAllShaderColor( const std::string & locationId, float r, float g, float b, float a );
    
    // Connect to the Init Shader signal
    void connect_initShader( const InitShaderSignal::slot_type & slot );

private:

    // Constructor
    CShaderMgr();

    // Destructor
    ~CShaderMgr();

    // Create the shader
    void createShader( const XMLNode & node );

    // Create the shader
    void createShader( uint32_t shaderType, const std::string & filePath );

    // Create the shader program
    void createProgram();

    // Bind the attribute location
    void bindAttributeLocation( const XMLNode & vertexNode );

    // Link the shader program
    void linkProgram();

    // Find the location of the shader variables
    void locateShaderVariables(
        const XMLNode & vertexNode,
        const XMLNode & fragmentNode );
    
    // Get the uniform location
    void getUniformLocation( const XMLNode & node );
    
    // Set the shader member variable
    void setShaderColor( CShaderData & shaderData, const std::string & locationId, const CColor & color );

private:

    // Map containing shader programs
    std::map< const std::string, CShaderData > m_shaderMap;
    std::map< const std::string, CShaderData >::iterator m_Iter;

    // Current shader data pointer - We DON'T own this pointer, don't free
    CShaderData * m_pCurrentShaderData;
    
    // Current vertex attribute count
    size_t m_currentVertexAttribCount;
    
    // Init Shader signal
    InitShaderSignal m_initShaderSignal;

};

#endif  // __shader_manager_h__
