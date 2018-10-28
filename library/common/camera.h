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
#include <common/defs.h>
#include <common/size.h>
#include <utilities/matrix.h>

class CCamera : public CObject3D
{
public:

    // Constructor
    CCamera();
    CCamera( float minZDist, float maxZDist, float scale );
    CCamera( float angle, float minZDist, float maxZDist, float scale );

    // Destructor
    virtual ~CCamera();

    // Recreate the projection matrix
    void recreateProjMatrix();

    // Set/Inc the camera's world position
    void setPos( const CPoint<CWorldValue> & position );
    void setPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );

    void incPos( const CPoint<CWorldValue> & position );
    void incPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );

    // Generate a custom perspective projection for this camera
    void generatePerspectiveProjection( float angle, float minZDist, float maxZDist, float scale = 1.f );

    // Generate a custom orthographic projection for this camera
    void generateOrthographicProjection( float minZDist, float maxZDist, float scale = 1.f );

    // Get the projected matrix
    const CMatrix & getProjectionMatrix() const;

    // Height and width screen ratio for render 2D objects
    float getOrthoHeightAspectRatio() const;

    // Get the orthographic projected size
    const CSize<float> & getOrthoProjSize() const;

    // Get the orthographic projected size half
    const CSize<float> & getOrthoProjSizeHalf() const;

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

    // Pre-calculated aspect ratios for orthographic projection
    float m_orthoHeightAspectRatio;

    // Orthographic projection size
    CSize<float> m_orthoProjSize;
    CSize<float> m_orthoProjSizeHalf;

    // The projection type
    NDefs::EProjectionType m_projType;

    // Projection settings
    float m_angle;
    float m_minZDist;
    float m_maxZDist;
    float m_scale;
};

#endif
