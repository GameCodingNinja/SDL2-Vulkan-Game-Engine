
/************************************************************************
*    FILE NAME:       shaderdata.h
*
*    DESCRIPTION:     shader data class
************************************************************************/

#ifndef __shader_data_h__
#define __shader_data_h__

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

class CShaderData
{
public:

    CShaderData();
    ~CShaderData();

    // Set/Get the program location
    void setProgramID( uint32_t location );
    uint32_t getProgramID() const;

    // Set/Get the vertex ID
    void setVertexID( uint32_t location );
    uint32_t getVertexID() const;

    // Set/Get the fragment ID
    void setFragmentID( uint32_t location );
    uint32_t getFragmentID() const;

    // Get the attribute location
    void setAttributeLocation( const std::string & name, int32_t location );
    int32_t getAttributeLocation( const std::string & name ) const;

    // Get the uniform location
    void setUniformLocation( const std::string & name, int32_t location );
    int32_t getUniformLocation( const std::string & name ) const;
    
    // Check for the uniform location
    bool hasUniformLocation( const std::string & name ) const;
    
    // Get the vertex attribute count
    size_t getVertexAttribCount();

    // Free the data
    void free();

private:

    // Clear the data
    void clear();

private:

    // OpenGL ID's
    uint32_t m_programID;
    uint32_t m_vertexID;
    uint32_t m_fragmentID;

    // attribute location shader map
    std::map<const std::string, int32_t > m_attributeMap;

    // uniform location shader map
    std::map<const std::string, int32_t > m_uniformMap;

};

#endif  // __shader_data_h__


