/************************************************************************
*    FILE NAME:       camera.cpp
*
*    DESCRIPTION:     Class that holds the camera position and rotation
************************************************************************/

// Physical component dependency
#include <common/camera.h>

// Game lib dependencies
#include <utilities/settings.h>
#include <utilities/xmlParser.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CCamera::CCamera() :
    m_projType(CSettings::Instance().getProjectionType()),
    m_angle(CSettings::Instance().getViewAngle()),
    m_minZDist(CSettings::Instance().getMinZdist()),
    m_maxZDist(CSettings::Instance().getMaxZdist())
{
    init();
}

CCamera::CCamera( const XMLNode & node ) :
    m_projType(CSettings::Instance().getProjectionType()),
    m_angle(CSettings::Instance().getViewAngle()),
    m_minZDist(CSettings::Instance().getMinZdist()),
    m_maxZDist(CSettings::Instance().getMaxZdist())
{
    loadFromNode( node );
    init();
}

CCamera::CCamera( float minZDist, float maxZDist ) :
    m_projType(NDefs::EPT_ORTHOGRAPHIC),
    m_angle(0),
    m_minZDist(minZDist),
    m_maxZDist(maxZDist)
{
    init();
}

CCamera::CCamera( float angle, float minZDist, float maxZDist ) :
    m_projType(NDefs::EPT_PERSPECTIVE),
    m_angle(angle),
    m_minZDist(minZDist),
    m_maxZDist(maxZDist)
{
    init();
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CCamera::~CCamera()
{
}


/************************************************************************
*    DESC:  Init the camera
************************************************************************/  
void CCamera::loadFromNode( const XMLNode & node )
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

    if( m_parameters.isSet( NDefs::TRANSFORM ) )
        invertPos();
}


/************************************************************************
*    DESC:  Init the camera
************************************************************************/
void CCamera::init( NDefs::EProjectionType projType, float angle, float minZDist, float maxZDist )
{
    m_projType = projType;
    m_angle = angle;
    m_minZDist = minZDist;
    m_maxZDist = maxZDist;
    
    init();
}

void CCamera::init()
{
    // Create the projection matrix
    createProjectionMatrix();
    
    // Do the initial transform
    CObjectTransform::transform();
    
    // Calculate the final matrix
    calcFinalMatrix();
}


/************************************************************************
*    DESC:  Create the projection matrix
************************************************************************/
void CCamera::createProjectionMatrix()
{
    if( m_projType == NDefs::EPT_PERSPECTIVE )
    {
        m_projectionMatrix.perspectiveFovRH(
            m_angle,
            CSettings::Instance().getScreenAspectRatio().w,
            m_minZDist,
            m_maxZDist );
    }
    else
    {
        const auto defSize = CSettings::Instance().getDefaultSize();
        
        m_projectionMatrix.orthographicRH(
            defSize.w,
            defSize.h,
            m_minZDist,
            m_maxZDist );
    }
}


/************************************************************************
*    DESC:  Get the projected matrix
************************************************************************/  
const CMatrix & CCamera::getProjectionMatrix() const
{
    return m_projectionMatrix;
}


/************************************************************************
*    DESC:  Set the camera's world position
************************************************************************/  
void CCamera::setPos( const CPoint<CWorldValue> & position )
{
    CObjectTransform::setPos( -position );
}

void CCamera::setPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    CObjectTransform::setPos( -x, -y, -z );
}


/************************************************************************
*    DESC:  Increment the camera's world position
************************************************************************/  
void CCamera::incPos( const CPoint<CWorldValue> & position )
{
    CObjectTransform::incPos( -position );
}

void CCamera::incPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    CObjectTransform::incPos( -x, -y, -z );
}


/************************************************************************
*    DESC:  Transform
************************************************************************/
void CCamera::transform()
{
    const bool wasTransformed( m_parameters.isSet( NDefs::TRANSFORM ) );
    
    CObjectTransform::transform();
    
    if( wasTransformed )
        calcFinalMatrix();
}


/************************************************************************
*    DESC:  Calculate the final matrix
************************************************************************/
void CCamera::calcFinalMatrix()
{
    m_finalMatrix.initilizeMatrix();
    m_finalMatrix.mergeMatrix( m_matrix );
    m_finalMatrix.mergeMatrix( m_projectionMatrix );
}


/************************************************************************
*    DESC:  Get the final matrix
************************************************************************/  
const CMatrix & CCamera::getFinalMatrix() const
{
    return m_finalMatrix;
}


/************************************************************************
*    DESC:  Convert to orthographic screen coordinates
************************************************************************/  
CPoint<float> CCamera::toOrthoCoord( const CPoint<float> & position )
{
    CPoint<float> pos;

    auto & ratio = CSettings::Instance().getOrthoAspectRatio();
    auto & sizeHalf = CSettings::Instance().getSizeHalf();

    pos.x = (position.x - sizeHalf.w) / (ratio.w * m_scale.x);
    pos.y = (position.y - sizeHalf.h) / (ratio.h * m_scale.y);

    return pos;
}


/************************************************************************
*    DESC:  Apply the rotation
************************************************************************/
void CCamera::applyRotation( CMatrix & matrix )
{
    m_rotMatrix.initilizeMatrix();
    m_rotMatrix.rotate( m_rot );

    // Since the rotation has already been done, multiply it into the matrix
    matrix.multiply3x3( m_rotMatrix );
}


/************************************************************************
*    DESC:  Get the rotation matrix
************************************************************************/
const CMatrix & CCamera::getRotMatrix() const
{
    return m_rotMatrix;
}
