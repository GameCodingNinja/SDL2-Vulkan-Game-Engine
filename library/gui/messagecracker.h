
/************************************************************************
*    FILE NAME:       messagecracker.h
*
*    DESCRIPTION:     Message crackers
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/defs.h>
#include <common/point.h>

class CSelectMsgCracker
{
public:

    CSelectMsgCracker()
    { m_unionValue.m_uintValue = 0; }

    // Set/Get the press type
    void setPressType( uint value )
    { m_unionValue.m_packedUint.m_pressType = value; }
    EActionPress getPressType() const
    { return static_cast<EActionPress>(m_unionValue.m_packedUint.m_pressType); }

    // Set/Get the device Id
    void setDeviceId( uint value )
    { m_unionValue.m_packedUint.m_deviceId = value; }
    EDeviceId getDeviceId() const
    { return static_cast<EDeviceId>(m_unionValue.m_packedUint.m_deviceId); }

    // Set/Get the x
    void setX( uint value )
    { m_unionValue.m_packedUint.m_x = value; }
    uint getX() const
    { return m_unionValue.m_packedUint.m_x; }

    // Set/Get the y
    void setY( uint value )
    { m_unionValue.m_packedUint.m_y = value; }
    uint getY() const
    { return m_unionValue.m_packedUint.m_y; }

    // Set/Get the packed uint
    void setPackedUnit( uint value )
    { m_unionValue.m_uintValue = value; }
    uint getPackedUnit() const
    { return m_unionValue.m_uintValue; }
    
    // Is this device a mouse?
    bool isDeviceMouse() const
    { return (getDeviceId() == EDeviceId::MOUSE); }
    
    // Is the a press down?
    bool isPressDown() const
    { return (getPressType() == EActionPress::DOWN); }
    
    // Is the a press down?
    bool isPressUp() const
    { return (getPressType() == EActionPress::UP); }
    
    // Get the mouse position
    CPoint<float> getPos() const
    { CPoint<float> pos( getX(), getY() );
      return pos; }

private:

    union
    {
        struct
        {
            uint m_pressType : 2;
            uint m_deviceId  : 2;
            uint m_x         : 14;
            uint m_y         : 14;
        } m_packedUint;

        uint m_uintValue;

    } m_unionValue;
};
