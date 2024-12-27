
/************************************************************************
*    FILE NAME:   defs.h
*
*    DESCRIPTION: misc defines
************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdint>

// define an unsigned int
typedef unsigned int uint;
typedef uint16_t handle16_t;

#define defs_DEG_TO_RAD 0.0174532925199432957
#define defs_RAD_TO_DEG 57.29577951308232
#define defs_EPSILON    8.854187817e-12
#define defs_RGB_TO_DEC 0.00390625f

#define defs_DEFAULT_ID       -1
#define defs_DEFAULT_NODE_ID  0
#define defs_DEFAULT_HANDLE   0
#define defs_NULL_BODY_TYPE   -1

// Analog stick max values -32768 to 32767 but to simplify it, we'll just use 32767
#define defs_MAX_ANALOG_AXIS_VALUE        32767
#define defs_ANALOG_PERCENTAGE_CONVERTION 327.67f

enum class EOrientation
{
    HORZ,
    LANDSCAPE=HORZ,
    VERT,
    PORTRAIT=VERT
};

enum class EHorzAlignment
{
    HORZ_LEFT,
    HORZ_CENTER,
    HORZ_RIGHT
};

enum class EVertAlignment
{
    VERT_TOP,
    VERT_CENTER,
    VERT_BOTTOM
};

enum class EGenType
{
    _NULL_,
    QUAD,
    SPRITE_SHEET,
    SCALED_FRAME,
    MESH_FILE,
    FONT
};

enum class EDeviceId
{
    _NULL_=-1,
    KEYBOARD,
    MOUSE,
    GAMEPAD
};

enum class EActionPress
{
    _NULL_ = -1,
    IDLE = 0,
    DOWN,
    UP
};

enum class ELightType
{
    _NULL_ = 0,
    DIRECTIONAL,
    POINT_INFINITE,
    POINT_RADIUS,
};

enum class EMirror
{
    _NULL_ = 0,
    HORIZONTAL,
    VERTICAL,
    HORIZONTAL_VERTICAL
};

enum class ETextFilter
{
    ANISOTROPIC_0X = 0,
    ANISOTROPIC_2X = 2,
    ANISOTROPIC_4X = 4,
    ANISOTROPIC_8X = 8,
    ANISOTROPIC_16X = 16
};

enum class ETimeFormat
{
    ETF_MIL = 0b0001,
    ETF_S_MIL = 0b0011,
    ETF_M_S_MIL = 0b0111,
    ETF_H_M_S_MIL = 0b1111,
    ETF_S = 0b0010,
    ETF_M_S = 0b0110,
    ETF_H_M_S = 0b1110,
};

enum class EProjectionType
{
    _NULL_,
    PERSPECTIVE,
    ORTHOGRAPHIC
};

enum class ENodeType: uint8_t
{
    _NULL_ = 0,
    OBJECT,
    SPRITE,
    UI_CONTROL,
};

enum class ECullType
{
    _NULL_,
    CULL_FULL,
    CULL_X_ONLY,
    CULL_Y_ONLY
};

namespace NOffset
{
    enum EOffset
    {
        _NULL       = 0,
        LEFT        = 1,
        RIGHT       = 2,
        HORZ_CENTER = 4,
        TOP         = 8,
        BOTTOM      = 16,
        VERT_CENTER = 32
    };
}
