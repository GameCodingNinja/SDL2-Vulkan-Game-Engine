/************************************************************************
*    FILE NAME:       camera.h
*
*    DESCRIPTION:     class that holds the camera position and rotation
************************************************************************/

#ifndef __camera_h__
#define __camera_h__

// Physical component dependency
#include <3d/object3d.h>

// Game lib dependencies
#include <utilities/matrix.h>

// Forward Declarations
struct XMLNode;

class CCamera : public CObject3D
{
public:

    // Constructor
    CCamera();
    CCamera( const XMLNode & node );
    CCamera( float minZDist, float maxZDist );
    CCamera( float angle, float minZDist, float maxZDist );

    // Destructor
    virtual ~CCamera();

    // Load the camera from node
    void loadFromNode( const XMLNode & node );
    
    // Init the camera
    void init( NDefs::EProjectionType projType, float angle, float minZDist, float maxZDist );
    void init();

    // Create the projection matrix
    void createProjectionMatrix();

    // Set/Inc the camera's world position
    void setPos( const CPoint<CWorldValue> & position );
    void setPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );

    void incPos( const CPoint<CWorldValue> & position );
    void incPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );

    // Get the projected matrix
    const CMatrix & getProjectionMatrix() const;

    // Transform - One call for those objects that don't have parents
    virtual void transform();

    // Get the final matrix
    const CMatrix & getFinalMatrix() const;

private:

    // Calculate the final matrix
    void calcFinalMatrix();

private:

    // Custom projection matrix
    CMatrix m_projectionMatrix;

    // Custom projection matrix
    CMatrix m_finalMatrix;

    // The projection type
    NDefs::EProjectionType m_projType;

    // Projection settings
    float m_angle;
    float m_minZDist;
    float m_maxZDist;
};

#endif
