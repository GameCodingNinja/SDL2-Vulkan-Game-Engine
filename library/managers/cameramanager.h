
/************************************************************************
*    FILE NAME:       cameramanager.h
*
*    DESCRIPTION:     camera manager class singleton
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/camera.h>

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

class CCameraMgr
{
public:

    // Get the instance of the singleton class
    static CCameraMgr & Instance()
    {
        static CCameraMgr cameraMgr;
        return cameraMgr;
    }

    // Load the camera data
    void load( const std::string & filePath );

    // Get the default camera
    CCamera & getDefault();

    // Get the camera
    CCamera & get( const std::string & id );

    // Add to transform list
    void addToTransListVec( const std::vector<std::string> & cameraIdVec );
    void addToTransList( const std::string & id );

    // Remove camera from transform list
    void removeFromTransList( const std::string & id );

    // Transform the active camera's in the list
    void transform();

    // Clear all data
    void clear();

    // Clear the transform list
    void clearTransList();

    // Rebuild all camera's projection matrixes
    void rebuildProjectionMatrix();

private:

    CCameraMgr();
    ~CCameraMgr();

private:

    // map list of cameras
    std::map<const std::string, CCamera> m_cameraMap;
    
    // Default camera
    CCamera m_defCamera;

    // Transform vector. List of camera's 
    // that need to be transformed
    std::vector<CCamera *> m_transVec;
};
