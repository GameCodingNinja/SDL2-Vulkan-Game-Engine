
/************************************************************************
*    FILE NAME:       cameradata.h
*
*    DESCRIPTION:     Camera Data Class
************************************************************************/

#ifndef __camera_data_h__
#define __camera_data_h__

// Physical component dependency
#include <common/object.h>

// Standard lib dependencies
#include <string>

class CCameraData : public CObject
{
public:

    // Constructor
    CCameraData( const XMLNode & node );
    CCameraData( const CCameraData & data );
    
    // Destructor
    virtual ~CCameraData();
    
    // Get the projection type
    NDefs::EProjectionType getProjectionType() const;
    
    // Get the minimum z distance
    float getMinZdist() const;
    
    // Get the maximum z distance
    float getMaxZdist() const;
    
    // Get the view angle
    float getViewAngle() const;

private:
    
    // The projection type
    NDefs::EProjectionType m_projectionType;

    // minimum Z distance
    float m_minZdist;

    // maximum Z distance
    float m_maxZdist;
    
    // view angle
    float m_viewAngle;
};

#endif
