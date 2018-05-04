
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
    void createProjMatrix();
    
    // Get the default projection matrix
    const CMatrix & getDefaultProjMatrix() const;
    
    // Get the camera final matrix
    const CMatrix & getCameraMatrix( const std::string & id ) const;
    
    // Get the projection matrix
    const CMatrix & getProjectionMatrix( NDefs::EProjectionType type ) const;
    
    // Create an orthographic camera
    CCamera & createOrthographic( const std::string & id, float minZDist = 5, float maxZDist = 1000, float scale = 1.f );
    
    // Create a perspective camera
    CCamera & createPerspective( const std::string & id, float angle = 45, float minZDist = 5, float maxZDist = 1000, float scale = 1.f );
    
    // Set/Get the active camera
    void setActiveCamera( const std::string & id );
    CCamera & getActiveCamera();
    
    // Get the camera
    CCamera & getCamera( const std::string & id );
    const CCamera & getCamera( const std::string & id ) const;
    
    // Set the active camera position
    void setActiveCameraPos( const CPoint<CWorldValue> & position );
    void setActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z );
    void setActiveCameraPos( const CPoint<float> & position );
    void setActiveCameraPos( float x = 0, float y = 0, float z = 0 );
    
    // Inc the active camera position
    void incActiveCameraPos( const CPoint<CWorldValue> & position );
    void incActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z );
    void incActiveCameraPos( const CPoint<float> & position );
    void incActiveCameraPos( float x = 0, float y = 0, float z = 0 );
    
    // Set the active camera rotation
    void setActiveCameraRot( const CPoint<float> & position );
    void setActiveCameraRot( float x = 0, float y = 0, float z = 0 );
    
    // Inc the active camera rotation
    void incActiveCameraRot( const CPoint<float> & position );
    void incActiveCameraRot( float x = 0, float y = 0, float z = 0 );
    
    // Set the active camera scale
    void setActiveCameraScale( const CPoint<float> & position );
    void setActiveCameraScale( float x = 1, float y = 1, float z = 1 );
    
    // Inc the active camera scale
    void incActiveCameraScale( const CPoint<float> & position );
    void incActiveCameraScale( float x = 1, float y = 1, float z = 1 );
    
    // Get the final matrix
    const CMatrix & getActiveCameraMatrix() const;
    
    // Transform all the cameras
    void transform();
    void transformCamera( const std::string & id );

private:

    CCameraMgr();
    ~CCameraMgr();

private:

    // map list of cameras
    std::map<const std::string, CCamera> m_cameraMap;
    
    // Default Projection matrixes
    CMatrix m_perspectiveMatrix;
    CMatrix m_orthographicMatrix;
    
    // Default camera
    CCamera m_defaultCamera;
    
    // Active camera pointer
    CCamera * m_pActiveCamera;
};

#endif  // __camera_manager_h__
