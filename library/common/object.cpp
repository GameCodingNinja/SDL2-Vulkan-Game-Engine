/************************************************************************
*    FILE NAME:       object.cpp
*
*    DESCRIPTION:     object class
************************************************************************/

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObject::CObject() :
    m_parameters(NDefs::VISIBLE),
    m_scale(1,1,1)
{
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CObject::~CObject()
{
}


/************************************************************************
*    DESC:  Get the object's world position
************************************************************************/
const CPoint<CWorldValue> & CObject::getPos() const
{
    return m_pos;
}


/************************************************************************
*    DESC:  Set the object's position
************************************************************************/
void CObject::setPos( const CPoint<CWorldValue> & position )
{
    m_parameters.add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos = position;
}

void CObject::setPos( const CPoint<float> & position )
{
    m_parameters.add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos = position;
}

void CObject::setPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    m_parameters.add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos.set( x, y, z );
}


/************************************************************************
*    DESC:  Inc the object's float position
************************************************************************/
void CObject::incPos( const CPoint<CWorldValue> & position )
{
    m_parameters.add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos += position;
}

void CObject::incPos( const CPoint<float> & position )
{
    m_parameters.add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos += position;
}

void CObject::incPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    m_parameters.add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos.inc( x, y, z );
}


/************************************************************************
*    DESC:  Invert the object's position
************************************************************************/
void CObject::invertPos()
{
    m_pos.invert();
}


/************************************************************************
*    DESC:  Set the object's rotation
*           NOTE: Rotation is stored as radians
************************************************************************/
void CObject::setRot( const CPoint<float> & rotation, bool convertToRadians )
{
    m_parameters.add( NDefs::ROTATE | NDefs::TRANSFORM );

    m_rot = rotation;
    
    if( convertToRadians )
        m_rot = rotation * defs_DEG_TO_RAD;
    else
        m_rot = rotation;
}

void CObject::setRot( float x, float y, float z, bool convertToRadians )
{
    m_parameters.add( NDefs::ROTATE | NDefs::TRANSFORM );
    
    if( convertToRadians )
        m_rot.set( x * defs_DEG_TO_RAD, y * defs_DEG_TO_RAD, z * defs_DEG_TO_RAD );
    else
        m_rot.set( x, y, z );
}


/************************************************************************
*    DESC:  Inc the pre-translation matrix
*           NOTE: Rotation is stored as radians
************************************************************************/
void CObject::incRot( const CPoint<float> & rotation, bool convertToRadians )
{
    m_parameters.add( NDefs::ROTATE | NDefs::TRANSFORM );

    if( convertToRadians )
        m_rot += rotation * defs_DEG_TO_RAD;
    else
        m_rot += rotation;

    m_rot.cap(360.0f * defs_DEG_TO_RAD);
}

void CObject::incRot( float x, float y, float z, bool convertToRadians )
{
    m_parameters.add( NDefs::ROTATE | NDefs::TRANSFORM );
    
    if( convertToRadians )
        m_rot.inc( x * defs_DEG_TO_RAD, y * defs_DEG_TO_RAD, z * defs_DEG_TO_RAD );
    else
        m_rot.inc( x, y, z );
}


/************************************************************************
*    DESC:  Get the object's rotation in radians
*           NOTE: Rotation is stored as radians
************************************************************************/
const CPoint<float> & CObject::getRot() const
{
    return m_rot;
}


/************************************************************************
*    DESC:  Set the object's scale
************************************************************************/
void CObject::setScale( const CPoint<float> & scale )
{
    m_parameters.add( NDefs::SCALE | NDefs::TRANSFORM );

    m_scale = scale;
}

void CObject::setScale( float x, float y, float z )
{
    m_parameters.add( NDefs::SCALE | NDefs::TRANSFORM );

    m_scale.set( x, y, z );
}


/************************************************************************
*    DESC:  Inc the object's scale
************************************************************************/
void CObject::incScale( const CPoint<float> & scale )
{
    m_parameters.add( NDefs::SCALE | NDefs::TRANSFORM );

    m_scale += scale;
}

void CObject::incScale( float x, float y, float z )
{
    m_parameters.add( NDefs::SCALE | NDefs::TRANSFORM );

    m_scale.inc( x, y, z );
}


/************************************************************************
*    DESC:  Get the object's scale
************************************************************************/
const CPoint<float> & CObject::getScale() const
{
    return m_scale;
}


/************************************************************************
*    DESC:  Get the object's center position
************************************************************************/
const CPoint<float> & CObject::getCenterPos() const
{
    return m_centerPos;
}


/************************************************************************
*    DESC:  Set the object's center position
************************************************************************/
void CObject::setCenterPos( const CPoint<float> & position )
{
    m_parameters.add( NDefs::CENTER_POINT | NDefs::TRANSFORM );

    m_centerPos = position;
}

void CObject::setCenterPos( float x, float y, float z )
{
    m_parameters.add( NDefs::CENTER_POINT | NDefs::TRANSFORM );

    m_centerPos.set( x, y, z );
}


/************************************************************************
*    DESC:  Set the object's crop offset
************************************************************************/
void CObject::setCropOffset( const CSize<int16_t> & offset )
{
    if( !m_centerPos.isEmpty() || !offset.isEmpty() )
    {
        m_parameters.add( NDefs::CROP_OFFSET | NDefs::TRANSFORM );

        m_cropOffset = offset;
    }
}


/************************************************************************
*    DESC:  Set the object visible
************************************************************************/
void CObject::setVisible( bool value )
{
    if( value )
        m_parameters.add( NDefs::VISIBLE );
    else
        m_parameters.remove( NDefs::VISIBLE );
}


/************************************************************************
*    DESC:  Is the object visible
************************************************************************/
bool CObject::isVisible() const
{
    return m_parameters.isSet( NDefs::VISIBLE );
}


/************************************************************************
*    DESC:  Copy the transform to the passed in object
************************************************************************/
void CObject::copyTransform( const CObject * pObject )
{
    if( pObject->m_parameters.isSet( NDefs::TRANSLATE ) )
        setPos( pObject->m_pos );

    if( pObject->m_parameters.isSet( NDefs::ROTATE ) )
        setRot( pObject->m_rot, false );

    if( pObject->m_parameters.isSet( NDefs::SCALE ) )
        setScale( pObject->m_scale );
    
    if( pObject->m_parameters.isSet( NDefs::CENTER_POINT ) )
        setCenterPos( pObject->m_centerPos );
    
    if( pObject->m_parameters.isSet( NDefs::CROP_OFFSET ) )
        setCropOffset( pObject->m_cropOffset );
}


/************************************************************************
*    DESC:  Load the transform data from node
************************************************************************/
void CObject::loadTransFromNode( const XMLNode & node )
{
    bool loadedFlag;

    CPoint<CWorldValue> pos = NParseHelper::LoadPosition( node, loadedFlag );
    if( loadedFlag )
        setPos( pos );

    CPoint<float> rot = NParseHelper::LoadRotation( node, loadedFlag );
    if( loadedFlag )
        setRot( rot );

    CPoint<float> scale = NParseHelper::LoadScale( node, loadedFlag );
    if( loadedFlag )
        setScale( scale );
    
    CPoint<float> centerPos = NParseHelper::LoadCenterPos( node, loadedFlag );
    if( loadedFlag )
        setCenterPos( centerPos );
    
    CDynamicOffset dynamicOffset = NParseHelper::LoadDynamicOffset( node, loadedFlag );
    if( loadedFlag )
        setPos( dynamicOffset.getPos() );
}


/************************************************************************
*    DESC:  Get the parameters
************************************************************************/
CBitmask<int16_t> & CObject::getParameters()
{
    return m_parameters;
}
