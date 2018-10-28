
/************************************************************************
*    FILE NAME:       cameramanager.cpp
*
*    DESCRIPTION:     camera manager class singleton
************************************************************************/

// Physical component dependency
#include <managers/cameramanager.h>

// Game lib dependencies
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CCameraMgr::CCameraMgr() :
    m_pActiveCamera(nullptr)
{
    createProjMatrix();

    // Set the default position which allows everything to render
    m_defaultCamera.setPos( 0, 0, 20 );
    m_defaultCamera.transform();
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CCameraMgr::~CCameraMgr()
{
}


/************************************************************************
*    DESC:  Create or recreate the projection matrixes for resolution change
*           NOTE: Also called when the resolution changes for the new screen size
************************************************************************/
void CCameraMgr::createProjMatrix()
{
    m_perspectiveMatrix.perspectiveFovRH(
        CSettings::Instance().getViewAngle(),
        CSettings::Instance().getScreenAspectRatio().w,
        CSettings::Instance().getMinZdist(),
        CSettings::Instance().getMaxZdist() );

    m_orthographicMatrix.orthographicRH(
        CSettings::Instance().getDefaultSize().getW(),
        CSettings::Instance().getDefaultSize().getH(),
        CSettings::Instance().getMinZdist(),
        CSettings::Instance().getMaxZdist() );

    for( auto & iter : m_cameraMap )
        iter.second.recreateProjMatrix();

    // Generate the default camera projection
    if( CSettings::Instance().getProjectionType() == NDefs::EPT_ORTHOGRAPHIC )
        m_defaultCamera.generateOrthographicProjection(
            CSettings::Instance().getMinZdist(),
            CSettings::Instance().getMaxZdist(),
            CSettings::Instance().getProjectionScale() );
    else
        m_defaultCamera.generatePerspectiveProjection(
            CSettings::Instance().getViewAngle(),
            CSettings::Instance().getMinZdist(),
            CSettings::Instance().getMaxZdist(),
            CSettings::Instance().getProjectionScale() );
}


/************************************************************************
*    DESC:  Get the projection matrix
************************************************************************/
const CMatrix & CCameraMgr::getProjMatrix( NDefs::EProjectionType type ) const
{
    if( type == NDefs::EPT_ORTHOGRAPHIC )
        return m_orthographicMatrix;
    else
        return m_perspectiveMatrix;
}


/************************************************************************
*    DESC:  Create an orthographic camera
************************************************************************/
CCamera & CCameraMgr::createOrthographic( const std::string & id, float minZDist, float maxZDist, float scale )
{
    auto iter = m_cameraMap.emplace(
        std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(minZDist, maxZDist, scale) );

    m_pActiveCamera = &iter.first->second;

    return iter.first->second;
}


/************************************************************************
*    DESC:  Create a perspective camera
************************************************************************/
CCamera & CCameraMgr::createPerspective( const std::string & id, float angle, float minZDist, float maxZDist, float scale )
{
    auto iter = m_cameraMap.emplace(
        std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(angle, minZDist, maxZDist, scale) );

    m_pActiveCamera = &iter.first->second;

    return iter.first->second;
}


/************************************************************************
*    DESC:  Switch to a camera
************************************************************************/
void CCameraMgr::switchCamera( const std::string & id )
{
    m_pActiveCamera = nullptr;

    auto iter = m_cameraMap.find( id );
    if( iter != m_cameraMap.end() )
        m_pActiveCamera = &iter->second;

    else if( id != "" )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Camera id does not exist (%s).") % id ) );
}


/************************************************************************
*    DESC:  Get the camera final matrix
************************************************************************/
const CMatrix & CCameraMgr::getFinalMatrix() const
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->getFinalMatrix();

    return m_defaultCamera.getFinalMatrix();
}


/************************************************************************
*    DESC:  Set the active camera position
************************************************************************/
void CCameraMgr::setPos( const CPoint<CWorldValue> & position )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->setPos(position);

    else
        m_defaultCamera.setPos(position);
}

void CCameraMgr::setPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->setPos(x,y,z);
    else
        m_defaultCamera.setPos(x,y,z);
}

void CCameraMgr::setPos( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->setPos(position);
    else
        m_defaultCamera.setPos(position);
}

void CCameraMgr::setPos( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->setPos(x,y,z);
    else
        m_defaultCamera.setPos(x,y,z);
}


/************************************************************************
*    DESC:  Inc the active camera position
************************************************************************/
void CCameraMgr::incPos( const CPoint<CWorldValue> & position )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->incPos(position);
    else
        m_defaultCamera.incPos(position);
}

void CCameraMgr::incPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->incPos(x,y,z);
    else
        m_defaultCamera.incPos(x,y,z);
}

void CCameraMgr::incPos( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->incPos(position);
    else
        m_defaultCamera.incPos(position);
}

void CCameraMgr::incPos( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->incPos(x,y,z);
    else
        m_defaultCamera.incPos(x,y,z);
}


/************************************************************************
*    DESC:  Set the active camera rotation
************************************************************************/
void CCameraMgr::setRot( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->setRot(position);
    else
        m_defaultCamera.setRot(position);
}

void CCameraMgr::setRot( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->setRot(x,y,z);
    else
        m_defaultCamera.setRot(x,y,z);
}


/************************************************************************
*    DESC:  Inc the active camera rotation
************************************************************************/
void CCameraMgr::incRot( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->incRot(position);
    else
        m_defaultCamera.incRot(position);
}

void CCameraMgr::incRot( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->incRot(x,y,z);
    else
        m_defaultCamera.incRot(x,y,z);
}


/************************************************************************
*    DESC:  Set the active camera scale
************************************************************************/
void CCameraMgr::setScale( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->setScale(position);
    else
        m_defaultCamera.setScale(position);
}

void CCameraMgr::setScale( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->setScale(x,y,z);
    else
        m_defaultCamera.setScale(x,y,z);
}


/************************************************************************
*    DESC:  Inc the active camera scale
************************************************************************/
void CCameraMgr::incScale( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->incScale(position);
    else
        m_defaultCamera.incScale(position);
}

void CCameraMgr::incScale( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->incScale(x,y,z);
    else
        m_defaultCamera.incScale(x,y,z);
}


/************************************************************************
*    DESC:  Transform the all cameras
************************************************************************/
void CCameraMgr::transformAll()
{
    for( auto & iter : m_cameraMap )
        iter.second.transform();
}


/************************************************************************
*    DESC:  Transform the camera
************************************************************************/
void CCameraMgr::transform()
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->transform();
    else
        m_defaultCamera.transform();
}
