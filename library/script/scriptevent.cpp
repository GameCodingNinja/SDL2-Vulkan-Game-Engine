
/************************************************************************
*    FILE NAME:       scriptevent.h
*
*    DESCRIPTION:     Event script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptevent.h>

// Game lib dependencies
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>
#include <scriptarray/scriptarray.h>

// SDL lib dependencies
#include <SDL3/SDL_events.h>

namespace NScriptEvent
{
    std::string string;

    /************************************************************************
    *    DESC:  Get wrapper functions                                                          
    ************************************************************************/
    const std::string & GetTextEditingString( SDL_TextEditingEvent & rText )
    {
        string = rText.text;
        return string;
    }

    const std::string & GetTextInputString( SDL_TextInputEvent & rText )
    {
        string = rText.text;
        return string;
    }

    /************************************************************************
    *    DESC:  Get data sensor array wrapper function                                                           
    ************************************************************************/
    void GetSensorDataArray( asIScriptGeneric * pScriptGen )
    {
        SDL_SensorEvent * pSensor = static_cast<SDL_SensorEvent *>(pScriptGen->GetObject());

        // Get a pointer to type declaration
        asITypeInfo * arrayType = CScriptMgr::Instance().getPtrToTypeInfo( "array<float>" );

        CScriptArray* ary = CScriptArray::Create(arrayType, 6);
        for( uint i = 0; i < 6; ++i )
            ary->SetValue(i, &pSensor->data[i]);

        // Set the return value
        pScriptGen->SetReturnObject( ary );
    }

    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType( "CEvent",                 0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CCommonEvent",           0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CDisplayEvent",          0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CWindowEvent",           0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CKeyboardEvent",         0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "SDL_Keycode",            0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CTextEditingEvent",      0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CTextInputEvent",        0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CMouseMotionEvent",      0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CMouseButtonEvent",      0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CMouseWheelEvent",       0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CJoyAxisEvent",          0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CJoyBallEvent",          0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CJoyHatEvent",           0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CJoyButtonEvent",        0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CJoyDeviceEvent",        0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CControllerAxisEvent",   0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CControllerButtonEvent", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CControllerDeviceEvent", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CAudioDeviceEvent",      0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CSensorEvent",           0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CQuitEvent",             0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CUserEvent",             0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType( "CTouchFingerEvent",      0, asOBJ_REF|asOBJ_NOCOUNT) );

        // Register property
        Throw( pEngine->RegisterObjectProperty("CEvent", "uint type",                      asOFFSET(SDL_Event, type)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CCommonEvent common",            asOFFSET(SDL_Event, common)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CDisplayEvent display",          asOFFSET(SDL_Event, display)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CWindowEvent window",            asOFFSET(SDL_Event, window)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CKeyboardEvent key",             asOFFSET(SDL_Event, key)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CTextEditingEvent edit",         asOFFSET(SDL_Event, edit)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CTextInputEvent text",           asOFFSET(SDL_Event, text)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CMouseMotionEvent motion",       asOFFSET(SDL_Event, motion)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CMouseButtonEvent button",       asOFFSET(SDL_Event, button)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CMouseWheelEvent wheel",         asOFFSET(SDL_Event, wheel)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CJoyAxisEvent jaxis",            asOFFSET(SDL_Event, jaxis)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CJoyBallEvent jball",            asOFFSET(SDL_Event, jball)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CJoyHatEvent jhat",              asOFFSET(SDL_Event, jhat)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CJoyButtonEvent jbutton",        asOFFSET(SDL_Event, jbutton)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CJoyDeviceEvent jdevice",        asOFFSET(SDL_Event, jdevice)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CControllerAxisEvent gaxis",     asOFFSET(SDL_Event, gaxis)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CControllerButtonEvent gbutton", asOFFSET(SDL_Event, gbutton)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CControllerDeviceEvent gdevice", asOFFSET(SDL_Event, gdevice)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CAudioDeviceEvent adevice",      asOFFSET(SDL_Event, adevice)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CSensorEvent sensor",            asOFFSET(SDL_Event, sensor)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CQuitEvent quit",                asOFFSET(SDL_Event, quit)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CUserEvent user",                asOFFSET(SDL_Event, user)) );
        Throw( pEngine->RegisterObjectProperty("CEvent", "CTouchFingerEvent tfinger",      asOFFSET(SDL_Event, tfinger)) );

        Throw( pEngine->RegisterObjectProperty("CCommonEvent", "uint type",                asOFFSET(SDL_CommonEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CCommonEvent", "uint timestamp",           asOFFSET(SDL_CommonEvent, timestamp)) );

        Throw( pEngine->RegisterObjectProperty("CDisplayEvent", "uint type",               asOFFSET(SDL_DisplayEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CDisplayEvent", "uint timestamp",          asOFFSET(SDL_DisplayEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CDisplayEvent", "uint displayID",          asOFFSET(SDL_DisplayEvent, displayID)) );
        Throw( pEngine->RegisterObjectProperty("CDisplayEvent", "int data1",               asOFFSET(SDL_DisplayEvent, data1)) );
        Throw( pEngine->RegisterObjectProperty("CDisplayEvent", "int data2",               asOFFSET(SDL_DisplayEvent, data2)) );

        Throw( pEngine->RegisterObjectProperty("CWindowEvent", "uint type",                asOFFSET(SDL_WindowEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CWindowEvent", "uint timestamp",           asOFFSET(SDL_WindowEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CWindowEvent", "uint windowID",            asOFFSET(SDL_WindowEvent, windowID)) );
        Throw( pEngine->RegisterObjectProperty("CWindowEvent", "int data1",                asOFFSET(SDL_WindowEvent, data1)) );
        Throw( pEngine->RegisterObjectProperty("CWindowEvent", "int data2",                asOFFSET(SDL_WindowEvent, data2)) );

        Throw( pEngine->RegisterObjectProperty("CKeyboardEvent", "uint type",              asOFFSET(SDL_KeyboardEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CKeyboardEvent", "uint timestamp",         asOFFSET(SDL_KeyboardEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CKeyboardEvent", "uint windowID",          asOFFSET(SDL_KeyboardEvent, windowID)) );
        Throw( pEngine->RegisterObjectProperty("CKeyboardEvent", "bool down",              asOFFSET(SDL_KeyboardEvent, down)) );
        Throw( pEngine->RegisterObjectProperty("CKeyboardEvent", "uint8 repeat",           asOFFSET(SDL_KeyboardEvent, repeat)) );
        Throw( pEngine->RegisterObjectProperty("CKeyboardEvent", "SDL_Keycode key",        asOFFSET(SDL_KeyboardEvent, key)) );

        Throw( pEngine->RegisterObjectProperty("CTextEditingEvent", "uint type",           asOFFSET(SDL_TextEditingEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CTextEditingEvent", "uint timestamp",      asOFFSET(SDL_TextEditingEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CTextEditingEvent", "uint windowID",       asOFFSET(SDL_TextEditingEvent, windowID)) );
        Throw( pEngine->RegisterObjectProperty("CTextEditingEvent", "int start",           asOFFSET(SDL_TextEditingEvent, start)) );
        Throw( pEngine->RegisterObjectProperty("CTextEditingEvent", "int length",          asOFFSET(SDL_TextEditingEvent, length)) );
        Throw( pEngine->RegisterObjectMethod("CTextEditingEvent", "const string & getStr()", WRAP_OBJ_LAST(GetTextEditingString), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectProperty("CTextInputEvent", "uint type",             asOFFSET(SDL_TextInputEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CTextInputEvent", "uint timestamp",        asOFFSET(SDL_TextInputEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CTextInputEvent", "uint windowID",         asOFFSET(SDL_TextInputEvent, windowID)) );
        Throw( pEngine->RegisterObjectMethod("CTextInputEvent", "const string & getStr()", WRAP_OBJ_LAST(GetTextInputString), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "uint type",           asOFFSET(SDL_MouseMotionEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "uint timestamp",      asOFFSET(SDL_MouseMotionEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "uint windowID",       asOFFSET(SDL_MouseMotionEvent, windowID)) );
        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "uint which",          asOFFSET(SDL_MouseMotionEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "uint state",          asOFFSET(SDL_MouseMotionEvent, state)) );
        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "int x",               asOFFSET(SDL_MouseMotionEvent, x)) );
        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "int y",               asOFFSET(SDL_MouseMotionEvent, y)) );
        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "int xrel",            asOFFSET(SDL_MouseMotionEvent, xrel)) );
        Throw( pEngine->RegisterObjectProperty("CMouseMotionEvent", "int yrel",            asOFFSET(SDL_MouseMotionEvent, yrel)) );

        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "uint type",           asOFFSET(SDL_MouseButtonEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "uint timestamp",      asOFFSET(SDL_MouseButtonEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "uint windowID",       asOFFSET(SDL_MouseButtonEvent, windowID)) );
        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "uint which",          asOFFSET(SDL_MouseButtonEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "uint8 button",        asOFFSET(SDL_MouseButtonEvent, button)) );
        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "bool down",           asOFFSET(SDL_MouseButtonEvent, down)) );
        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "uint8 clicks",        asOFFSET(SDL_MouseButtonEvent, clicks)) );
        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "int x",               asOFFSET(SDL_MouseButtonEvent, x)) );
        Throw( pEngine->RegisterObjectProperty("CMouseButtonEvent", "int y",               asOFFSET(SDL_MouseButtonEvent, y)) );

        Throw( pEngine->RegisterObjectProperty("CMouseWheelEvent", "uint type",            asOFFSET(SDL_MouseWheelEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CMouseWheelEvent", "uint timestamp",       asOFFSET(SDL_MouseWheelEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CMouseWheelEvent", "uint windowID",        asOFFSET(SDL_MouseWheelEvent, windowID)) );
        Throw( pEngine->RegisterObjectProperty("CMouseWheelEvent", "uint which",           asOFFSET(SDL_MouseWheelEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CMouseWheelEvent", "int x",                asOFFSET(SDL_MouseWheelEvent, x)) );
        Throw( pEngine->RegisterObjectProperty("CMouseWheelEvent", "int y",                asOFFSET(SDL_MouseWheelEvent, y)) );
        Throw( pEngine->RegisterObjectProperty("CMouseWheelEvent", "uint direction",       asOFFSET(SDL_MouseWheelEvent, direction)) );

        Throw( pEngine->RegisterObjectProperty("CJoyAxisEvent", "uint type",               asOFFSET(SDL_JoyAxisEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CJoyAxisEvent", "uint timestamp",          asOFFSET(SDL_JoyAxisEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CJoyAxisEvent", "int which",               asOFFSET(SDL_JoyAxisEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CJoyAxisEvent", "uint8 axis",              asOFFSET(SDL_JoyAxisEvent, axis)) );
        Throw( pEngine->RegisterObjectProperty("CJoyAxisEvent", "int16 value",             asOFFSET(SDL_JoyAxisEvent, value)) );

        Throw( pEngine->RegisterObjectProperty("CJoyBallEvent", "uint type",               asOFFSET(SDL_JoyBallEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CJoyBallEvent", "uint timestamp",          asOFFSET(SDL_JoyBallEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CJoyBallEvent", "int which",               asOFFSET(SDL_JoyBallEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CJoyBallEvent", "int16 xrel",              asOFFSET(SDL_JoyBallEvent, xrel)) );
        Throw( pEngine->RegisterObjectProperty("CJoyBallEvent", "int16 yrel",              asOFFSET(SDL_JoyBallEvent, yrel)) );

        Throw( pEngine->RegisterObjectProperty("CJoyHatEvent", "uint type",                asOFFSET(SDL_JoyHatEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CJoyHatEvent", "uint timestamp",           asOFFSET(SDL_JoyHatEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CJoyHatEvent", "int which",                asOFFSET(SDL_JoyHatEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CJoyHatEvent", "uint8 hat",                asOFFSET(SDL_JoyHatEvent, hat)) );
        Throw( pEngine->RegisterObjectProperty("CJoyHatEvent", "uint8 value",              asOFFSET(SDL_JoyHatEvent, value)) );

        Throw( pEngine->RegisterObjectProperty("CJoyButtonEvent", "uint type",             asOFFSET(SDL_JoyButtonEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CJoyButtonEvent", "uint timestamp",        asOFFSET(SDL_JoyButtonEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CJoyButtonEvent", "int which",             asOFFSET(SDL_JoyButtonEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CJoyButtonEvent", "uint8 button",          asOFFSET(SDL_JoyButtonEvent, button)) );
        Throw( pEngine->RegisterObjectProperty("CJoyButtonEvent", "bool down",             asOFFSET(SDL_JoyButtonEvent, down)) );

        Throw( pEngine->RegisterObjectProperty("CJoyDeviceEvent", "uint type",             asOFFSET(SDL_JoyDeviceEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CJoyDeviceEvent", "uint timestamp",        asOFFSET(SDL_JoyDeviceEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CJoyDeviceEvent", "int which",             asOFFSET(SDL_JoyDeviceEvent, which)) );

        Throw( pEngine->RegisterObjectProperty("CControllerAxisEvent", "uint type",        asOFFSET(SDL_GamepadAxisEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CControllerAxisEvent", "uint timestamp",   asOFFSET(SDL_GamepadAxisEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CControllerAxisEvent", "int which",        asOFFSET(SDL_GamepadAxisEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CControllerAxisEvent", "uint8 axis",       asOFFSET(SDL_GamepadAxisEvent, axis)) );
        Throw( pEngine->RegisterObjectProperty("CControllerAxisEvent", "int16 value",      asOFFSET(SDL_GamepadAxisEvent, value)) );

        Throw( pEngine->RegisterObjectProperty("CControllerButtonEvent", "uint type",      asOFFSET(SDL_GamepadButtonEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CControllerButtonEvent", "uint timestamp", asOFFSET(SDL_GamepadButtonEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CControllerButtonEvent", "int which",      asOFFSET(SDL_GamepadButtonEvent, which)) );
        Throw( pEngine->RegisterObjectProperty("CControllerButtonEvent", "uint8 button",   asOFFSET(SDL_GamepadButtonEvent, button)) );
        Throw( pEngine->RegisterObjectProperty("CControllerButtonEvent", "bool down",      asOFFSET(SDL_GamepadButtonEvent, down)) );

        Throw( pEngine->RegisterObjectProperty("CControllerDeviceEvent", "uint type",      asOFFSET(SDL_GamepadDeviceEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CControllerDeviceEvent", "uint timestamp", asOFFSET(SDL_GamepadDeviceEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CControllerDeviceEvent", "int which",      asOFFSET(SDL_GamepadDeviceEvent, which)) );

        Throw( pEngine->RegisterObjectProperty("CAudioDeviceEvent", "uint type",           asOFFSET(SDL_AudioDeviceEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CAudioDeviceEvent", "uint timestamp",      asOFFSET(SDL_AudioDeviceEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CAudioDeviceEvent", "uint which",          asOFFSET(SDL_AudioDeviceEvent, which)) );

        Throw( pEngine->RegisterObjectProperty("CSensorEvent", "uint type",                asOFFSET(SDL_SensorEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CSensorEvent", "uint timestamp",           asOFFSET(SDL_SensorEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CSensorEvent", "int which",                asOFFSET(SDL_SensorEvent, which)) );
        Throw( pEngine->RegisterObjectMethod("CSensorEvent", "array<float> @ getDataArray()", asFUNCTION(GetSensorDataArray), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectProperty("CQuitEvent", "uint type",                  asOFFSET(SDL_QuitEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CQuitEvent", "uint timestamp",             asOFFSET(SDL_QuitEvent, timestamp)) );

        Throw( pEngine->RegisterObjectProperty("CUserEvent", "uint type",                  asOFFSET(SDL_UserEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CUserEvent", "uint timestamp",             asOFFSET(SDL_UserEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CUserEvent", "uint windowID",              asOFFSET(SDL_UserEvent, windowID)) );
        Throw( pEngine->RegisterObjectProperty("CUserEvent", "int code",                   asOFFSET(SDL_UserEvent, code)) );

        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "uint type",           asOFFSET(SDL_TouchFingerEvent, type)) );
        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "uint timestamp",      asOFFSET(SDL_TouchFingerEvent, timestamp)) );
        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "int64 touchId",       asOFFSET(SDL_TouchFingerEvent, touchID)) );
        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "int64 fingerId",      asOFFSET(SDL_TouchFingerEvent, fingerID)) );
        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "float x",             asOFFSET(SDL_TouchFingerEvent, x)) );
        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "float y",             asOFFSET(SDL_TouchFingerEvent, y)) );
        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "float dx",            asOFFSET(SDL_TouchFingerEvent, dx)) );
        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "float dy",            asOFFSET(SDL_TouchFingerEvent, dy)) );
        Throw( pEngine->RegisterObjectProperty("CTouchFingerEvent", "float pressure",      asOFFSET(SDL_TouchFingerEvent, pressure)) );
     }
}