
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
CCameraData::CCameraData( const XMLNode & node )
{
    // Init with the defaults
    m_minZdist = CSettings::Instance().getMinZdist();
    m_maxZdist = CSettings::Instance().getMaxZdist();
    m_viewAngle = CSettings::Instance().getViewAngle();
    m_projectionType = CSettings::Instance().getProjectionType();
    
    if( node.isAttributeSet("minZDist") )
        m_minZdist = std::atof(node.getAttribute("minZDist"));

    if( node.isAttributeSet("maxZDist") )
        m_maxZdist = std::atof(node.getAttribute("maxZDist"));

    if( node.isAttributeSet("view_angle") )
        m_viewAngle = std::atof(node.getAttribute("view_angle"));

    if( node.isAttributeSet("projectType") )
    {
        if( std::strcmp( node.getAttribute("projectType"), "orthographic" ) == 0 )
            m_projectionType = NDefs::EPT_ORTHOGRAPHIC;
        
        else if( std::strcmp( node.getAttribute("projectType"), "perspective" ) == 0 )
            m_projectionType = NDefs::EPT_PERSPECTIVE;
    }
}

CCameraData::CCameraData( const CCameraData & data ) :
    m_projectionType( data.m_projectionType ),
    m_minZdist( data.m_minZdist ),
    m_maxZdist( data.m_maxZdist ),
    m_viewAngle( data.m_viewAngle )
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CCameraData::~CCameraData()
{
}


/************************************************************************
*    DESC:  Get the projection type
************************************************************************/
NDefs::EProjectionType CCameraData::getProjectionType() const
{
    return m_projectionType;
}


/************************************************************************
*    DESC:  Get the minimum z distance
************************************************************************/
float CCameraData::getMinZdist() const
{
    return m_minZdist;
}


/************************************************************************
*    DESC:  Get the maximum z distance
************************************************************************/
float CCameraData::getMaxZdist() const
{
    return m_maxZdist;
}


/************************************************************************
*    DESC:  Get the view angle
************************************************************************/
float CCameraData::getViewAngle() const
{
    return m_viewAngle;
}
