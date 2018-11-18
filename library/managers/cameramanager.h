
/************************************************************************
*    FILE NAME:       cameramanager.h
*
*    DESCRIPTION:     camera manager class singleton
************************************************************************/  

#ifndef __camera_manager_h__
#define __camera_manager_h__

// Game lib dependencies
#include <common/camera.h>

// Standard lib dependencies
#include <string>
#include <map>

class CCameraMgr
{
public:

    // Get the instance of the singleton class
    static CCameraMgr & Instance()
    {
        static CCameraMgr cameraMgr;
        return cameraMgr;
    }
    
    // Create the projection matrixes
    // NOTE: Also called when the resolution changes for the new screen size
    void createProjMatrix();
    
    // Get the projection matrix
    const CMatrix & getProjMatrix( NDefs::EProjectionType type = NDefs::EPT_ORTHOGRAPHIC ) const;
    
    // Create an orthographic camera
    CCamera & createOrthographic( const std::string & id, float minZDist = 5, float maxZDist = 1000, float scale = 1.f );
    
    // Create a perspective camera
    CCamera & createPerspective( const std::string & id, float angle = 45, float minZDist = 5, float maxZDist = 1000, float scale = 1.f );
    
    // Get the camera
    CCamera & getCamera( const std::string & id );
    
    // Get the camera final matrix
    const CMatrix & getFinalMatrix() const;
    
    // Switch to a camera
    void switchCamera( const std::string & id = "" );
    
    // Set the active camera position
    void setPos( const CPoint<CWorldValue> & position );
    void setPos( CWorldValue x, CWorldValue y, CWorldValue z );
    void setPos( const CPoint<float> & position );
    void setPos( float x = 0, float y = 0, float z = 0 );
    
    // Inc the active camera position
    void incPos( const CPoint<CWorldValue> & position );
    void incPos( CWorldValue x, CWorldValue y, CWorldValue z );
    void incPos( const CPoint<float> & position );
    void incPos( float x = 0, float y = 0, float z = 0 );
    
    // Set the active camera rotation
    void setRot( const CPoint<float> & position );
    void setRot( float x = 0, float y = 0, float z = 0 );
    
    // Inc the active camera rotation
    void incRot( const CPoint<float> & position );
    void incRot( float x = 0, float y = 0, float z = 0 );
    
    // Set the active camera scale
    void setScale( const CPoint<float> & position );
    void setScale( float x = 1, float y = 1, float z = 1 );
    
    // Inc the active camera scale
    void incScale( const CPoint<float> & position );
    void incScale( float x = 1, float y = 1, float z = 1 );
    
    // Transform all the cameras
    void transformAll();
    void transform();

private:

    CCameraMgr();
    ~CCameraMgr();

private:

    // map list of cameras
    std::map<const std::string, CCamera> m_cameraMap;
    //std::map< const std::string, std::map< const std::string, CCamera > > m_cameraMapMap;
    
    // Default Projection matrixes
    CMatrix m_perspectiveMatrix;
    CMatrix m_orthographicMatrix;
    
    // Default camera
    CCamera m_defaultCamera;
    
    // Active camera pointer
    CCamera * m_pActiveCamera;
};

#endif  // __camera_manager_h__
