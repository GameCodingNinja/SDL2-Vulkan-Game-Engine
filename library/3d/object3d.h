
/************************************************************************
*    FILE NAME:       object.cpp
*
*    DESCRIPTION:     object class for 3D
************************************************************************/

#ifndef __object_3d_h__
#define __object_3d_h__

// Physical component dependency
#include <2d/object2d.h>

// Forward declaration(s)
class btTransform;

class CObject3D : public virtual CObject2D
{
public:

    // Constructor
    CObject3D();
    CObject3D( const CObject3D & obj );

    // Destructor
    virtual ~CObject3D();
    
    // Get the object's rotation matrix
    const CMatrix & getRotMatrix() const;
    
protected:
    
    // Apply the rotation
    virtual void applyRotation( CMatrix & matrix ) override;
    
protected:

    // Matrix for rotations only
    // Basically used for normal calculations
    CMatrix m_rotMatrix;

};

#endif  // __object_3d_h__
