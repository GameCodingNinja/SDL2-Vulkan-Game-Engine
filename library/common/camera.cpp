/************************************************************************
*    FILE NAME:       camera.cpp
*
*    DESCRIPTION:     Class that holds the camera position and rotation
************************************************************************/

// Physical component dependency
#include <common/camera.h>

// Game lib dependencies
#include <utilities/settings.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CCamera::CCamera() :
    m_orthoHeightAspectRatio(0.f),
    m_projType(NDefs::EPT_NULL),
    m_angle(0),
    m_minZDist(0),
    m_maxZDist(0)
{
}

CCamera::CCamera( float minZDist, float maxZDist ) :
    m_orthoHeightAspectRatio(0.f)
{
    generateOrthographicProjection( minZDist, maxZDist ); 
}

CCamera::CCamera( float angle, float minZDist, float maxZDist ) :
    m_orthoHeightAspectRatio(0.f)
{
    generatePerspectiveProjection( angle, minZDist, maxZDist );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CCamera::~CCamera()
{
}


/************************************************************************
*    DESC:  Recreate the projection matrix
************************************************************************/
void CCamera::recreateProjMatrix()
{
    if( m_projType == NDefs::EPT_PERSPECTIVE )
        generatePerspectiveProjection( m_angle, m_minZDist, m_maxZDist );
    else
        generateOrthographicProjection( m_minZDist, m_maxZDist );
}


/************************************************************************
*    DESC:  Init the camera
************************************************************************/  
void CCamera::init( NDefs::EProjectionType projType, float angle, float minZDist, float maxZDist )
{
    if( projType == NDefs::EPT_PERSPECTIVE )
        generatePerspectiveProjection( angle, minZDist, maxZDist );
    
    else
        generateOrthographicProjection( minZDist, maxZDist );
}


/************************************************************************
*    DESC:  Generate a custom perspective projection for this camera
************************************************************************/  
void CCamera::generatePerspectiveProjection( float angle, float minZDist, float maxZDist )
{
    m_projType = NDefs::EPT_PERSPECTIVE;
    m_angle = angle;
    m_minZDist = minZDist;
    m_maxZDist = maxZDist;
    
    m_projectionMatrix.perspectiveFovRH(
        angle,
        CSettings::Instance().getScreenAspectRatio().w,
        minZDist,
        maxZDist );
    
    calcFinalMatrix();
}


/************************************************************************
*    DESC:  Generate a custom orthographic projection for this camera
************************************************************************/  
void CCamera::generateOrthographicProjection( float minZDist, float maxZDist )
{
    m_projType = NDefs::EPT_ORTHOGRAPHIC;
    m_minZDist = minZDist;
    m_maxZDist = maxZDist;

    const auto defSize = CSettings::Instance().getDefaultSize();
    
    // Calc the new width and height
    m_orthoProjSize.set( defSize.getW(), defSize.getH() );
    m_orthoProjSizeHalf = m_orthoProjSize / 2.f;
    
    // Calc the new height aspect ratio
    m_orthoHeightAspectRatio = CSettings::Instance().getSize().h / m_orthoProjSize.h;

    m_projectionMatrix.orthographicRH(
        m_orthoProjSize.w,
        m_orthoProjSize.h,
        minZDist,
        maxZDist );
    
    calcFinalMatrix();
}


/************************************************************************
*    DESC:  Get the orthographic projected size
************************************************************************/
const CSize<float> & CCamera::getOrthoProjSize() const
{
    return m_orthoProjSize;
}


/************************************************************************
*    DESC:  Get the orthographic projected size half
************************************************************************/
const CSize<float> & CCamera::getOrthoProjSizeHalf() const
{
    return m_orthoProjSizeHalf;
}


/************************************************************************
*    DESC:  Height and width screen ratio for render 2D objects
*           The difference between screen height and the default height
************************************************************************/
float CCamera::getOrthoHeightAspectRatio() const
{
    return m_orthoHeightAspectRatio; 
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
