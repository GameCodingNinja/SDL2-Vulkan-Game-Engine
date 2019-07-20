
/************************************************************************
*    FILE NAME:   defs.h
*
*    DESCRIPTION: misc defines
************************************************************************/

#ifndef __defs_h__
#define __defs_h__

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdint>

// define an unsigned int
typedef unsigned int uint;

#define defs_DEG_TO_RAD 0.0174532925199432957
#define defs_RAD_TO_DEG 57.29577951308232
#define defs_EPSILON 8.854187817e-12
#define defs_RGB_TO_DEC 0.00390625f

#define defs_DEFAULT_ID       -1

// Analog stick max values -32768 to 32767 but to simplify it, we'll just use 32767
#define defs_MAX_ANALOG_AXIS_VALUE 32767
#define defs_ANALOG_PERCENTAGE_CONVERTION 327.67f

namespace NDefs
{
    enum EProjectionType
    {
        EPT_NULL,
        EPT_PERSPECTIVE,
        EPT_ORTHOGRAPHIC
    };
    
    enum EOrentation
    {
        EO_HORZ,
        EO_LANDSCAPE=EO_HORZ,
        EO_VERT,
        EO_PORTRAIT=EO_VERT
    };

    enum EHorzAlignment
    {
        EHA_HORZ_LEFT,
        EHA_HORZ_CENTER,
        EHA_HORZ_RIGHT
    };

    enum EVertAlignment
    {
        EVA_VERT_TOP,
        EVA_VERT_CENTER,
        EVA_VERT_BOTTOM
    };

    enum EGenerationType
    {
        EGT_NULL,
        EGT_QUAD,
        EGT_SPRITE_SHEET,
        EGT_SCALED_FRAME,
        EGT_MESH_FILE,
        EGT_FONT
    };

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
    };
    
    enum EAspectRatio
    {
        EAR_4_3,
        EAR_3_2,
        EAR_8_5,
        EAR_5_3,
        EAR_16_9
    };

    enum EDeviceId
    {
        DEVICE_NULL=-1,
        KEYBOARD,
        MOUSE,
        GAMEPAD,
        MAX_UNIQUE_DEVICES
    };

    enum EActionPress
    {
        EAP_IDLE = 0,
        EAP_DOWN,
        EAP_UP
    };

    enum ETextFilter
    {
        ETF_ANISOTROPIC_0X = 0,
        ETF_ANISOTROPIC_2X = 2,
        ETF_ANISOTROPIC_4X = 4,
        ETF_ANISOTROPIC_8X = 8,
        ETF_ANISOTROPIC_16X = 16
    };
    
    enum ELightType
    {
        ELT_NONE = 0,
        ELT_DIRECTIONAL,
        ELT_POINT_INFINITE,
        ELT_POINT_RADIUS,
    };
    
    enum EMirror
    {
        EM_NULL = 0,
        EM_HORIZONTAL,
        EM_VERTICAL,
        EM_HORIZONTAL_VERTICAL
    };
    
    enum ENodeType
    {
        ENT_NULL = 0,
        ENT_OBJECT,
        ENT_SPRITE,
        ENT_UI_CONTROL,
    };

}

#endif
