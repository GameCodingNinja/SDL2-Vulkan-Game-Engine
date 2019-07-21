
/************************************************************************
*    FILE NAME:       cameradata.h
*
*    DESCRIPTION:     Camera Data Class
************************************************************************/

// Physical component dependency
#include <common/cameradata.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/settings.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CCameraData::CCameraData() :
    m_projType(CSettings::Instance().getProjectionType()),
    m_minZDist(CSettings::Instance().getMinZdist()),
    m_maxZDist(CSettings::Instance().getMaxZdist()),
    m_angle(CSettings::Instance().getViewAngle())
{
}

CCameraData::CCameraData( const XMLNode & node ) :
    m_projType(CSettings::Instance().getProjectionType()),
    m_minZDist(CSettings::Instance().getMinZdist()),
    m_maxZDist(CSettings::Instance().getMaxZdist()),
    m_angle(CSettings::Instance().getViewAngle())
{
    loadFromNode( node );
}

CCameraData::CCameraData( const CCameraData & data ) :
    m_projType( data.m_projType ),
    m_minZDist( data.m_minZDist ),
    m_maxZDist( data.m_maxZDist ),
    m_angle( data.m_angle )
{
    copyTransform( &data );
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CCameraData::~CCameraData()
{
}


/************************************************************************
*    DESC:  Load the camera data
************************************************************************/
void CCameraData::loadFromNode( const XMLNode & node )
{
    if( node.isAttributeSet("minZDist") )
        m_minZDist = std::atof(node.getAttribute("minZDist"));

    if( node.isAttributeSet("maxZDist") )
        m_maxZDist = std::atof(node.getAttribute("maxZDist"));

    if( node.isAttributeSet("view_angle") )
        m_angle = std::atof(node.getAttribute("view_angle"));

    if( node.isAttributeSet("projectType") )
    {
        if( std::strcmp( node.getAttribute("projectType"), "orthographic" ) == 0 )
            m_projType = NDefs::EPT_ORTHOGRAPHIC;
        
        else if( std::strcmp( node.getAttribute("projectType"), "perspective" ) == 0 )
            m_projType = NDefs::EPT_PERSPECTIVE;
    }
    
    loadTransFromNode( node );
}


/************************************************************************
*    DESC:  Get the projection type
************************************************************************/
NDefs::EProjectionType CCameraData::getProjectionType() const
{
    return m_projType;
}


/************************************************************************
*    DESC:  Get the minimum z distance
************************************************************************/
float CCameraData::getMinZdist() const
{
    return m_minZDist;
}


/************************************************************************
*    DESC:  Get the maximum z distance
************************************************************************/
float CCameraData::getMaxZdist() const
{
    return m_maxZDist;
}


/************************************************************************
*    DESC:  Get the view angle
************************************************************************/
float CCameraData::getViewAngle() const
{
    return m_angle;
}
