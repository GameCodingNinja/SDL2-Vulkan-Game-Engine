/************************************************************************
*    FILE NAME:       camera.h
*
*    DESCRIPTION:     class that holds the camera position and rotation
************************************************************************/

#pragma once

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/worldvalue.h>

// Forward Declarations
struct XMLNode;

class CCamera : public CObject
{
public:

    // Constructor
    CCamera();
    CCamera( const XMLNode & node );
    //CCamera( float minZDist, float maxZDist );
    CCamera( EProjectionType projType, float angle, float minZDist, float maxZDist );

    // Destructor
    virtual ~CCamera();

    // Load the camera from node
    void loadFromNode( const XMLNode & node );
    
    // Init the camera
    void init( EProjectionType projType, float angle, float minZDist, float maxZDist );
    void init();

    // Create the projection matrix
    void createProjectionMatrix();

    // Get the world value position
    const CPoint<CWorldValue> & getWorldValuePos() const;

    // Set/Inc the world value position
    void setPos( const CPoint<CWorldValue> & position );
    void setPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );

    void incPos( const CPoint<CWorldValue> & position );
    void incPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );

    // Get the projected matrix
    const CMatrix & getProjectionMatrix() const;

    // Transform - One call for those objects that don't have parents
    void transform() final;

    // Get the final matrix
    const CMatrix & getFinalMatrix() const;

    // Convert to orthographic screen coordinates
    CPoint<float> toOrthoCoord( const CPoint<float> & position );

    // Get the rotation matrix
    const CMatrix & getRotMatrix() const final;

protected:
    
    // Apply the rotation
    void applyRotation( CMatrix & matrix ) final;

private:

    // Calculate the final matrix
    void calcFinalMatrix();

private:

    // World position value
    CPoint<CWorldValue> m_worldValPos;

    // Group name
    std::string m_group;

    // Custom projection matrix
    CMatrix m_projectionMatrix;

    // Custom projection matrix
    CMatrix m_finalMatrix;

    // Matrix for rotations only
    // Basically used for normal calculations
    CMatrix m_rotMatrix;

    // The projection type
    EProjectionType m_projType;

    // Projection settings
    float m_angle;
    float m_minZDist;
    float m_maxZDist;
};
