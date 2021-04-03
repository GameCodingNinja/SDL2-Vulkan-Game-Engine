/************************************************************************
*    FILE NAME:       object.h
*
*    DESCRIPTION:     object class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/size.h>
#include <common/point.h>
#include <utilities/bitmask.h>
#include <utilities/matrix.h>
#include <script/scriptcomponent.h>
#include <script/scriptpreparefunc.h>

// Standard lib dependencies
#include <cstdint>
#include <string>
#include <map>
#include <tuple>

// Forward declaration(s)
struct XMLNode;

class CObject
{
public:

    enum
    {
        // No parameters
        NONE                = 0x00,

        // Transform parameters
        TRANSLATE           = 0x01,
        ROTATE	            = 0x02,
        SCALE	            = 0x04,
        CENTER_POINT        = 0x08,
        CROP_OFFSET         = 0x10,

        // Translate parameters
        TRANSFORM           = 0x20,
        WAS_TRANSFORMED     = 0x40,

        // Matrix rotation
        PHYSICS_TRANSFORM   = 0x80,

        // Visible bit
        VISIBLE             = 0x100,

        // States
        STATE1              = 0x200,
        STATE2              = 0x400,
        STATE3              = 0x800,
        STATE4              = 0x1000,
        STATE5              = 0x2000,
        STATE6              = 0x4000,
        STATE7              = 0x8000
    };
    
    CObject();
    virtual ~CObject();

    // Load the transform data from node
    void loadTransFromNode( const XMLNode & node );

    // Set/Get the object's position
    void setPos( const CPoint<float> & position );
    void setPos( float x = 0, float y = 0, float z = 0 );
    const CPoint<float> & getPos() const;

    // Increment the object's position
    void incPos( const CPoint<float> & position );
    void incPos( float x = 0, float y = 0, float z = 0 );

    // Invert the object's position
    void invertPos();

    // Set/Get the object's rotation
    // NOTE: Rotation is stored as radians
    void setRot( const CPoint<float> & rotation, bool convertToRadians = true );
    void setRot( float x = 0, float y = 0, float z = 0, bool convertToRadians = true );
    const CPoint<float> & getRot() const;

    // Increment the object's rotation
    // NOTE: Rotation is stored as radians
    void incRot( const CPoint<float> & rotation, bool convertToRadians = true );
    void incRot( float x = 0, float y = 0, float z = 0, bool convertToRadians = true );

    // Set the object's scale
    void setScale( const CPoint<float> & scale );
    void setScale( float x = 1, float y = 1, float z = 1 );

    // Increment the object's scale
    void incScale( const CPoint<float> & scale );
    void incScale( float x = 1, float y = 1, float z = 1 );

    // Get the object's scale
    const CPoint<float> & getScale() const;
    
    // Set/Get the object's position
    void setCenterPos( const CPoint<float> & position );
    void setCenterPos( float x = 0, float y = 0, float z = 0 );
    const CPoint<float> & getCenterPos() const;
    
    // Set the object's crop offset
    void setCropOffset( const CSize<int16_t> & offset );

    // Set the object's visibility
    void setVisible( bool value = true );

    // Is the object visible
    bool isVisible() const;

    // Remove the script function from the map
    void removeScriptFunction( const std::string & scriptFuncId );

    // Add a script function to the map
    void addScriptFunction(
        const std::string & scriptFuncId,
        const std::string & group,
        const std::string & funcName,
        const bool prepareOnInit = false,
        const bool forceUpdate = false,
        const bool overwrite = false );
    
    // Does this object have script functions
    bool hasScriptFunctions();

public: // transform related members
    
    // Copy the transform to the passed in object
    void copyTransform( const CObject * pObject );
    
    // Get the parameters
    CBitmask<uint16_t> & getParameters();

    // Transform - One call for those objects that don't have parents
    virtual void transform();
    virtual void transform( const CObject & object );

    // Get the object's matrix
    const CMatrix & getMatrix() const;
    
    // Get the object's rotation matrix
    virtual const CMatrix & getRotMatrix() const;
    virtual CMatrix & getRotMatrix();

    // Was this object transformed?
    bool wasTranformed() const;

    // Force the transform
    void forceTransform();
    
    // Get the object's translated position
    const CPoint<float> & getTransPos() const;
    
    // Use a point to set a column - used for 3d physics
    virtual void setRotMatrixColumn( const int col, const float x, const float y, const float z ){};

public: // script related members

    // Prepare any script functions that are flagged to prepareOnInit
    void prepareOnInit();

    // Prepare the script Id function to run
    bool prepare( const std::string & scriptFuncId );

    // Stop the script and recycle the context
    bool stopAndRecycle( const std::string & scriptFuncId );

    // Stop the script, recycle and start the execution
    bool stopAndRestart( const std::string & scriptFuncId );

    // Copy over the script functions
    void copyScriptFunctions( const std::map<std::string, CScriptPrepareFunc> & scriptFunctionMap );

protected: // transform related members

    // Transform the object in local space
    void transformLocal( CMatrix & matrix );
    
    // Apply the scale
    virtual void applyScale( CMatrix & matrix );

    // Apply the rotation
    virtual void applyRotation( CMatrix & matrix );

protected: // script related members

    // Load the script functions from node and add them to the map
    void loadScriptFromNode( const XMLNode & node, const std::string & group );

protected:
    
    // Bitmask settings to record if the object needs to be transformed
    CBitmask<uint16_t> m_parameters;

    // Local position
    CPoint<float> m_pos;

    // Local Rotation stored in radians
    CPoint<float> m_rot;

    // Local scale
    CPoint<float> m_scale;
    
    // The center point. Point of rotation
    // This is used for defining a different center point
    CPoint<float> m_centerPos;
    
    // Offset due to a sprite sheet crop.
    CSize<int16_t> m_cropOffset;

    // Translated position
    CPoint<float> m_transPos;

protected: // transform related members

    // local matrix
    CMatrix m_matrix;

    // The script part of the sprite
    CScriptComponent m_scriptComponent;
    
    // Script function map. Execute scripts with an id
    std::map<const std::string, CScriptPrepareFunc> m_scriptFunctionMap;
};
