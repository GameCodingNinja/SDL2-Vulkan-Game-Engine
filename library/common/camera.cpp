/************************************************************************
*    FILE NAME:       camera.cpp
*
*    DESCRIPTION:     Class that holds the camera position and rotation
************************************************************************/

// Physical component dependency
#include <common/camera.h>

// Game lib dependencies
#include <utilities/settings.h>
#include <common/cameradata.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CCamera::CCamera() :
    m_projType(CSettings::Instance().getProjectionType()),
    m_angle(CSettings::Instance().getViewAngle()),
    m_minZDist(CSettings::Instance().getMinZdist()),
    m_maxZDist(CSettings::Instance().getMaxZdist())
{
    // Create the projection matrix
    createProjectionMatrix();
    
    // Calculate the final matrix
    calcFinalMatrix();
}

CCamera::CCamera( float minZDist, float maxZDist ) :
    m_projType(NDefs::EPT_ORTHOGRAPHIC),
    m_angle(0),
    m_minZDist(minZDist),
    m_maxZDist(maxZDist)
{
    // Create the projection matrix
    createProjectionMatrix();
    
    // Calculate the final matrix
    calcFinalMatrix(); 
}

CCamera::CCamera( float angle, float minZDist, float maxZDist ) :
    m_projType(NDefs::EPT_PERSPECTIVE),
    m_angle(angle),
    m_minZDist(minZDist),
    m_maxZDist(maxZDist)
{
    // Create the projection matrix
    createProjectionMatrix();
    
    // Calculate the final matrix
    calcFinalMatrix();
}

CCamera::CCamera( const CCameraData & cameraData ) :
    m_projType( cameraData.getProjectionType() ),
    m_angle( cameraData.getViewAngle() ),
    m_minZDist( cameraData.getMinZdist() ),
    m_maxZDist( cameraData.getMaxZdist() )
{
    copyTransform( &cameraData );
    
    // Create the projection matrix
    createProjectionMatrix();
    
    // Do the initial transform
    CObject3D::transform();
    
    // Calculate the final matrix
    calcFinalMatrix();
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
void CCamera::init( NDefs::EProjectionType projType, float angle, float minZDist, float maxZDist )
{
    m_projType = projType;
    m_angle = angle;
    m_minZDist = minZDist;
    m_maxZDist = maxZDist;
    
    // Create the projection matrix
    createProjectionMatrix();
    
    // Do the initial transform
    CObject3D::transform();
    
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
    CObject3D::setPos( -position );
}

void CCamera::setPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    CObject3D::setPos( -x, -y, -z );
}


/************************************************************************
*    DESC:  Increment the camera's world position
************************************************************************/  
void CCamera::incPos( const CPoint<CWorldValue> & position )
{
    CObject3D::incPos( -position );
}

void CCamera::incPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    CObject3D::incPos( -x, -y, -z );
}


/************************************************************************
*    DESC:  Transform
************************************************************************/
void CCamera::transform()
{
    const bool wasTransformed( m_parameters.isSet( NDefs::TRANSFORM ) );
    
    CObject3D::transform();
    
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
