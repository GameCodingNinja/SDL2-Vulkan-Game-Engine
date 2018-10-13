
/************************************************************************
 *    FILE NAME:       ojectvisualdata3d.cpp
 *
 *    DESCRIPTION:     Class containing the 3D object's visual data
 ************************************************************************/

#if !(defined(__IOS__) || defined(__ANDROID__) || defined(__arm__))
// Glew dependencies (have to be defined first)
#include <GL/glew.h>
#endif

// Physical component dependency
#include <objectdata/objectvisualdata3d.h>

// Game lib dependencies
//#include <managers/meshmanager.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
 *    DESC:  Constructor
 ************************************************************************/
CObjectVisualData3D::CObjectVisualData3D()
{
}

/************************************************************************
 *    DESC:  Destructor                                                             
 ************************************************************************/
CObjectVisualData3D::~CObjectVisualData3D()
{
}

/************************************************************************
 *    DESC:  Load the object data from node
 ************************************************************************/
void CObjectVisualData3D::loadFromNode( const XMLNode & objectNode )
{
    const XMLNode visualNode = objectNode.getChildNode( "visual" );
    if( !visualNode.isEmpty() )
    {
        // Get the mesh file
        if (visualNode.isAttributeSet("file"))
            m_meshFile = visualNode.getAttribute("file");

        // Get the color
        m_color = NParseHelper::LoadColor( visualNode, m_color );

        // Get the shader id
        const XMLNode shaderNode = visualNode.getChildNode( "shader" );
        if( !shaderNode.isEmpty() )
            m_shaderID = shaderNode.getAttribute( "id" );

        // Raise an exception if there's a mesh but no shader id
        if( !m_meshFile.empty() && m_shaderID.empty() )
        {
            throw NExcept::CCriticalException("Shader effect or techique not set!",
                boost::str( boost::format("Shader object data missing.\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
 *    DESC:  Load the mesh data from file
 ************************************************************************/
void CObjectVisualData3D::loadMeshData( const std::string & group )
{
    // Temporary implementation to just get cube on screen - remove
    //if( !m_meshFile.empty() )
    //    CMeshMgr::Instance().loadFromFile( group, m_meshFile );
}

/************************************************************************
 *    DESC:  Create the object from data
 ************************************************************************/
void CObjectVisualData3D::createFromData( const std::string & group )
{
    // Temporary implementation to just get cube on screen - remove
    //if( !m_meshFile.empty() )
    //    CMeshMgr::Instance().createFromData( group, m_meshFile, m_mesh3d );
}

/************************************************************************
 *    DESC:  Get the name of the shader ID
 ************************************************************************/
const std::string & CObjectVisualData3D::getShaderID() const
{
    return m_shaderID;
}

/************************************************************************
 *    DESC:  Get the color
 ************************************************************************/
const CColor & CObjectVisualData3D::getColor() const
{
    return m_color;
}

/************************************************************************
 *    DESC:  Whether or not the visual tag was specified
 ************************************************************************/
bool CObjectVisualData3D::isActive() const
{
    return !m_meshFile.empty();
}

/************************************************************************
 *    DESC:  Get the mesh3d vector
 ************************************************************************/
const CMesh3D & CObjectVisualData3D::getMesh3D() const
{
    return m_mesh3d;
}
