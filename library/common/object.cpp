/************************************************************************
*    FILE NAME:       object.cpp
*
*    DESCRIPTION:     object class
************************************************************************/

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CObject::CObject() :
    m_parameters(VISIBLE),
    m_scale(1,1,1)
{
}

CObject::~CObject()
{}

/************************************************************************
*    DESC:  Get the object's position
************************************************************************/
const CPoint<float> & CObject::getPos() const
{
    return m_pos;
}

/************************************************************************
*    DESC:  Set the object's position
************************************************************************/
void CObject::setPos( const CPoint<float> & position )
{
    m_parameters.add( TRANSLATE | TRANSFORM );

    m_pos = position;
}

void CObject::setPos( float x, float y, float z )
{
    m_parameters.add( TRANSLATE | TRANSFORM );

    m_pos.set( x, y, z );
}

/************************************************************************
*    DESC:  Inc the object's float position
************************************************************************/
void CObject::incPos( const CPoint<float> & position )
{
    m_parameters.add( TRANSLATE | TRANSFORM );

    m_pos += position;
}

void CObject::incPos( float x, float y, float z )
{
    m_parameters.add( TRANSLATE | TRANSFORM );

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
    m_parameters.add( ROTATE | TRANSFORM );

    m_rot = rotation;
    
    if( convertToRadians )
        m_rot = rotation * defs_DEG_TO_RAD;
    else
        m_rot = rotation;
}

void CObject::setRot( float x, float y, float z, bool convertToRadians )
{
    m_parameters.add( ROTATE | TRANSFORM );
    
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
    m_parameters.add( ROTATE | TRANSFORM );

    if( convertToRadians )
        m_rot += rotation * defs_DEG_TO_RAD;
    else
        m_rot += rotation;

    m_rot.cap(360.0f * defs_DEG_TO_RAD);
}

void CObject::incRot( float x, float y, float z, bool convertToRadians )
{
    m_parameters.add( ROTATE | TRANSFORM );
    
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
    m_parameters.add( SCALE | TRANSFORM );

    m_scale = scale;
}

void CObject::setScale( float x, float y, float z )
{
    m_parameters.add( SCALE | TRANSFORM );

    m_scale.set( x, y, z );
}

/************************************************************************
*    DESC:  Inc the object's scale
************************************************************************/
void CObject::incScale( const CPoint<float> & scale )
{
    m_parameters.add( SCALE | TRANSFORM );

    m_scale += scale;
}

void CObject::incScale( float x, float y, float z )
{
    m_parameters.add( SCALE | TRANSFORM );

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
    m_parameters.add( CENTER_POINT | TRANSFORM );

    m_centerPos = position;
}

void CObject::setCenterPos( float x, float y, float z )
{
    m_parameters.add( CENTER_POINT | TRANSFORM );

    m_centerPos.set( x, y, z );
}

/************************************************************************
*    DESC:  Set the object's crop offset
************************************************************************/
void CObject::setCropOffset( const CSize<int16_t> & offset )
{
    if( !m_centerPos.isEmpty() || !offset.isEmpty() )
    {
        m_parameters.add( CROP_OFFSET | TRANSFORM );

        m_cropOffset = offset;
    }
}

/************************************************************************
*    DESC:  Set the object visible
************************************************************************/
void CObject::setVisible( bool value )
{
    if( value )
        m_parameters.add( VISIBLE );
    else
        m_parameters.remove( VISIBLE );
}

/************************************************************************
*    DESC:  Is the object visible
************************************************************************/
bool CObject::isVisible() const
{
    return m_parameters.isSet( VISIBLE );
}

/************************************************************************
*    DESC:  Copy the transform to the passed in object
************************************************************************/
void CObject::copyTransform( const CObject * pObject )
{
    if( pObject->m_parameters.isSet( TRANSLATE ) )
        setPos( pObject->m_pos );

    if( pObject->m_parameters.isSet( ROTATE ) )
        setRot( pObject->m_rot, false );

    if( pObject->m_parameters.isSet( SCALE ) )
        setScale( pObject->m_scale );
    
    if( pObject->m_parameters.isSet( CENTER_POINT ) )
        setCenterPos( pObject->m_centerPos );
    
    if( pObject->m_parameters.isSet( CROP_OFFSET ) )
        setCropOffset( pObject->m_cropOffset );
}

/************************************************************************
*    DESC:  Load the transform data from node
************************************************************************/
void CObject::loadTransFromNode( const XMLNode & node )
{
    bool loadedFlag;

    // Set if visible. Is visible by default
    if( node.isAttributeSet( "visible" ) )
        setVisible( std::strcmp( node.getAttribute("visible"), "true" ) == 0 );

    CPoint<float> pos = NParseHelper::LoadPosition( node, loadedFlag );
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
CBitmask<uint16_t> & CObject::getParameters()
{
    return m_parameters;
}

/************************************************************************
*    DESC:  Get the object's translated position
************************************************************************/
const CPoint<float> & CObject::getTransPos() const
{
    return m_transPos;
}

/************************************************************************
*    DESC:  Transform the object in local space
************************************************************************/
void CObject::transformLocal( CMatrix & matrix )
{
    // Reset the matrices
    matrix.initilizeMatrix();

    // Apply the crop offset
    if( m_parameters.isSet( CROP_OFFSET ) )
        matrix.translate( m_cropOffset );

    // Apply the scale
    if( m_parameters.isSet( SCALE ) )
        applyScale( matrix );

    // Apply the rotation
    if( m_parameters.isSet( ROTATE ) )
        applyRotation( matrix );

    // Apply the translation
    if( m_parameters.isSet( TRANSLATE ) )
        matrix.translate( m_pos );

    // Clear the check parameter
    m_parameters.remove( TRANSFORM | PHYSICS_TRANSFORM );

    // Indicate that translation was done
    m_parameters.add( WAS_TRANSFORMED );
}

/************************************************************************
*    DESC:  Transform
************************************************************************/
void CObject::transform()
{
    m_parameters.remove( WAS_TRANSFORMED );
    
    if( m_parameters.isSet( TRANSFORM ) )
    {
        transformLocal( m_matrix );
    
        m_transPos = m_pos;
    }
}

void CObject::transform( const CObject & object )
{
    m_parameters.remove( WAS_TRANSFORMED );
    
    if( m_parameters.isSet( TRANSFORM ) || object.wasTranformed() )
    {
        CMatrix localMatrix;
    
        transformLocal( localMatrix );
    
        m_matrix = localMatrix * object.getMatrix();

        m_matrix.transform( m_transPos, CPoint<float>() );
    }
}

/************************************************************************
*    DESC:  Apply the scale
************************************************************************/
void CObject::applyScale( CMatrix & matrix )
{
    matrix.setScale( m_scale );
}

/************************************************************************
*    DESC:  Apply the rotation
************************************************************************/
void CObject::applyRotation( CMatrix & matrix )
{
    // Add in the center point prior to rotation
    if( m_parameters.isSet( CENTER_POINT ) )
        matrix.translate( m_centerPos );

    matrix.rotate( m_rot );

    // Subtract the center point after rotation to put back in original position
    if( m_parameters.isSet( CENTER_POINT ) )
        matrix.translate( -m_centerPos );
}

/************************************************************************
*    DESC:  Get the object's matrix
************************************************************************/
const CMatrix & CObject::getMatrix() const
{
    return m_matrix;
}

/************************************************************************
*    DESC:  Get the object's rotation matrix
*           NOTE: For 2d, it's the same matrix
************************************************************************/
const CMatrix & CObject::getRotMatrix() const
{
    return m_matrix;
}

CMatrix & CObject::getRotMatrix()
{
    return m_matrix;
}

/************************************************************************
*    DESC:  Was this object transformed?
************************************************************************/
bool CObject::wasTranformed() const
{
    return m_parameters.isSet( WAS_TRANSFORMED );
}

/************************************************************************
*    DESC:  Force a transform from this point all the way up the line
************************************************************************/
void CObject::forceTransform()
{
    m_parameters.add( TRANSFORM );
}

/************************************************************************
*    DESC:  Remove the script function from the map
************************************************************************/
void CObject::removeScriptFunction( const std::string & scriptFuncId )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
        m_scriptFunctionMap.erase( iter );
}

/************************************************************************
*    DESC:  Add a script function to the map
************************************************************************/
void CObject::addScriptFunction(
    const std::string & scriptFuncId,
    const std::string & group,
    const std::string & funcName,
    const bool prepareOnInit,
    const bool forceUpdate,
    const bool overwrite )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        if( !overwrite )
            NGenFunc::PostDebugMsg( boost::str( boost::format("WARNING: Add script function already exists (%s).") %scriptFuncId ) );
        m_scriptFunctionMap.erase( iter );
    }

    m_scriptFunctionMap.emplace( 
        std::piecewise_construct, std::forward_as_tuple(scriptFuncId), std::forward_as_tuple(group, funcName, prepareOnInit, forceUpdate) );
}

/************************************************************************
*    DESC:  Load the script functions from node and add them to the map
************************************************************************/
void CObject::loadScriptFromNode( const XMLNode & node, const std::string & group )
{
    NParseHelper::initScriptFunctions( node, m_scriptFunctionMap, group );
}

/************************************************************************
*    DESC:  Prepare any script functions that are flagged to prepareOnInit
************************************************************************/
void CObject::prepareOnInit()
{
    for( auto & iter : m_scriptFunctionMap )
        if( iter.second.prepareOnInit )
            prepare( iter.first );
}

/************************************************************************
*    DESC:  Prepare the script function Id to run
************************************************************************/
bool CObject::prepare( const std::string & scriptFuncId )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.prepare( iter->second.group, iter->second.funcName, {this} );

        // Allow the script to execute and return it's context to the queue
        // for the scripts that don't animate
        if( iter->second.forceUpdate )
            m_scriptComponent.update( true );

        return true;
    }

    return false;
}

/************************************************************************
*    DESC:  Stop the script and recycle the context
************************************************************************/
bool CObject::stopAndRecycle( const std::string & scriptFuncId )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.stopAndRecycle( iter->second.funcName );

        return true;
    }

    return false;
}

/************************************************************************
*    DESC:  Stop the script, recycle and start the execution
************************************************************************/
bool CObject::stopAndRestart( const std::string & scriptFuncId )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.stopAndRestart( iter->second.group, iter->second.funcName, {this} );

        // Force an update
        if( iter->second.forceUpdate )
            m_scriptComponent.update();

        return true;
    }

    return false;
}

/************************************************************************
*    DESC:  Copy over the script functions
************************************************************************/
void CObject::copyScriptFunctions( const std::map<std::string, CScriptPrepareFunc> & scriptFunctionMap )
{
    for( auto & iter : scriptFunctionMap )
        m_scriptFunctionMap.emplace( iter );
}

/************************************************************************
*    DESC:  Does this object have script functions
************************************************************************/
bool CObject::hasScriptFunctions()
{
    return !m_scriptFunctionMap.empty();
}
