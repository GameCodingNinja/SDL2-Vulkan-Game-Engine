
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
#include <managers/meshmanager.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
 *    desc:  Constructor
 ************************************************************************/
CObjectVisualData3D::CObjectVisualData3D()
{
}   // constructor

/************************************************************************
 *    desc:  Destructor                                                             
 ************************************************************************/
CObjectVisualData3D::~CObjectVisualData3D()
{
    // NOTE: Nothing should ever be deleted here
}   // Destructor

/************************************************************************
 *    desc:  Load the object data from node
 ************************************************************************/
void CObjectVisualData3D::LoadFromNode( const XMLNode & objectNode )
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

}   // LoadFromNode

/************************************************************************
 *    desc:  Create the object from data
 ************************************************************************/
void CObjectVisualData3D::CreateFromData( const std::string & group )
{
    // Temporary implementation to just get cube on screen - remove
    if( !m_meshFile.empty() )
        CMeshMgr::Instance().LoadFromFile( group, m_meshFile, m_meshVec );

}   // CreateFromData

/************************************************************************
 *    desc:  Get the name of the shader ID
 ************************************************************************/
const std::string & CObjectVisualData3D::GetShaderID() const
{
    return m_shaderID;
}

/************************************************************************
 *    desc:  Get the color
 ************************************************************************/
const CColor & CObjectVisualData3D::GetColor() const
{
    return m_color;
}

/************************************************************************
 *    desc:  Whether or not the visual tag was specified
 ************************************************************************/
bool CObjectVisualData3D::IsActive() const
{
    return !m_meshFile.empty();
}

/************************************************************************
 *    desc:  Get the mesh3d vector
 ************************************************************************/
const std::vector<CMesh3D> & CObjectVisualData3D::GetMesh3D() const
{
    return m_meshVec;
}
