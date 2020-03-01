
/************************************************************************
*    FILE NAME:       scriptactionmanager.cpp
*
*    DESCRIPTION:     CActionMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptactionmanager.h>

// Game lib dependencies
#include <managers/actionmanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <common/sensor.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptActionManager
{
    /************************************************************************
    *    DESC:  CSensor Constructor
    ************************************************************************/
    void SensorConstructor(void * thisPointer)
    {
        new(thisPointer) CSensor();
    }

    /************************************************************************
    *    DESC:  CSensor Copy Constructor
    ************************************************************************/
    void SensorCopyConstructor(const CSensor & other, void * pThisPointer)
    {
        new(pThisPointer) CSensor(other);
    }

    /************************************************************************
    *    DESC:  CSensor Destructor
    ************************************************************************/
    void SensorDestructor(void * pThisPointer)
    {
        ((CSensor*)pThisPointer)->~CSensor();
    }

    /************************************************************************
    *    DESC:  Was action function wrapper
    ************************************************************************/
    bool WasAction1(const SDL_Event & rEvent, const std::string & actionStr, uint actionPress, CActionMgr & actionMgr)
    {
        return actionMgr.wasAction(rEvent, actionStr, NDefs::EActionPress(actionPress));
    }

    uint WasAction2(const SDL_Event & rEvent, const std::string & actionStr, CActionMgr & actionMgr)
    {
        return actionMgr.wasAction(rEvent, actionStr);
    }

    /************************************************************************
    *    DESC:  Enumerate Mouse Wheel Events generic
    *    prototype: uint enumerateButtonEvents(
    *               uint & windowID, int & x, int & y, uint & direction, uint startIndex = 0)
    ************************************************************************/
    void EnumerateMouseWheelEvents(asIScriptGeneric * pScriptGen)
    {
        // Extract the arguments
        uint & windowID = *reinterpret_cast<uint*>(pScriptGen->GetArgAddress(0));
        int & x = *reinterpret_cast<int*>(pScriptGen->GetArgAddress(1));
        int & y = *reinterpret_cast<int*>(pScriptGen->GetArgAddress(2));
        uint & direction = *reinterpret_cast<uint*>(pScriptGen->GetArgAddress(3));
        uint startIndex = pScriptGen->GetArgDWord(4);

        pScriptGen->SetReturnDWord( CActionMgr::Instance().enumerateMouseWheelEvents( windowID, x, y, direction, startIndex ) );
    }

    /************************************************************************
    *    DESC:  Enumerate Mouse Wheel Events generic
    *    prototype: uint enumerateWindowEvents(
    *               uint & event, uint & windowID, int & data1, int & data2, uint startIndex = 0 )
    ************************************************************************/
    void EnumerateWindowEvents(asIScriptGeneric * pScriptGen)
    {
        // Extract the arguments
        uint & event = *reinterpret_cast<uint*>(pScriptGen->GetArgAddress(0));
        uint & windowID = *reinterpret_cast<uint*>(pScriptGen->GetArgAddress(1));
        int & data1 = *reinterpret_cast<int*>(pScriptGen->GetArgAddress(2));
        int & data2 = *reinterpret_cast<int*>(pScriptGen->GetArgAddress(3));
        uint startIndex = pScriptGen->GetArgDWord(4);

        pScriptGen->SetReturnDWord( CActionMgr::Instance().enumerateWindowEvents( event, windowID, data1, data2, startIndex ) );
    }

    /************************************************************************
    *    DESC:  Enumerate Touch Finger Events generic
    *    prototype: uint enumerateTouchFingerEvents(
    *               uint & event, int64_t & touchId, int64_t & fingerId, float & x, float & y, float & dx, float & dy, float & pressure, uint startIndex = 0)
    ************************************************************************/
    void EnumerateTouchFingerEvents(asIScriptGeneric * pScriptGen)
    {
        // Extract the arguments
        uint & event = *reinterpret_cast<uint*>(pScriptGen->GetArgAddress(0));
        int64_t & touchId = *reinterpret_cast<int64_t*>(pScriptGen->GetArgAddress(1));
        int64_t & fingerId = *reinterpret_cast<int64_t*>(pScriptGen->GetArgAddress(2));
        float & x = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(3));
        float & y = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(4));
        float & dx = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(5));
        float & dy = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(6));
        float & pressure = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(7));
        uint startIndex = pScriptGen->GetArgDWord(8);

        pScriptGen->SetReturnDWord( CActionMgr::Instance().enumerateTouchFingerEvents( event, touchId, fingerId, x, y, dx, dy, pressure, startIndex ) );
    }

    /************************************************************************
    *    DESC:  Enumerate Multiple Finger Events generic
    *    prototype: uint enumerateMultipleFingerEvents(
    *               int64_t & touchId, float & dTheta, float & dDist, float & x, float & y, uint & numFingers, uint startIndex = 0 )
    ************************************************************************/
    void EnumerateMultipleFingerEvents(asIScriptGeneric * pScriptGen)
    {
        // Extract the arguments
        int64_t & touchId = *reinterpret_cast<int64_t*>(pScriptGen->GetArgAddress(0));
        float & dTheta = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(1));
        float & dDist = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(2));
        float & x = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(3));
        float & y = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(4));
        uint & numFingers = *reinterpret_cast<uint*>(pScriptGen->GetArgAddress(5));
        uint startIndex = pScriptGen->GetArgDWord(6);

        pScriptGen->SetReturnDWord( CActionMgr::Instance().enumerateMultipleFingerEvents( touchId, dTheta, dDist, x, y, numFingers, startIndex ) );
    }

    /************************************************************************
    *    DESC:  Enumerate Dollar Gesture Events generic
    *    prototype: uint enumerateDollarGestureEvents(
    *               int64_t & touchId, int64_t & gestureId, uint & numFingers, float & error, float & x, float & y, uint startIndex = 0 )
    ************************************************************************/
    void EnumerateDollarGestureEvents(asIScriptGeneric * pScriptGen)
    {
        // Extract the arguments
        int64_t & touchId = *reinterpret_cast<int64_t*>(pScriptGen->GetArgAddress(0));
        int64_t & gestureId = *reinterpret_cast<int64_t*>(pScriptGen->GetArgAddress(1));
        uint & numFingers = *reinterpret_cast<uint*>(pScriptGen->GetArgAddress(2));
        float & error = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(3));
        float & x = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(4));
        float & y = *reinterpret_cast<float*>(pScriptGen->GetArgAddress(5));
        uint startIndex = pScriptGen->GetArgDWord(6);

        pScriptGen->SetReturnDWord( CActionMgr::Instance().enumerateDollarGestureEvents( touchId, gestureId, numFingers, error, x, y, startIndex ) );
    }

    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CActionMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        Throw( pEngine->RegisterObjectType("CSensor", sizeof(CSensor), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<CSensor>() | asOBJ_APP_CLASS_ALLFLOATS ) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CSensor", asBEHAVE_CONSTRUCT, "void f()",                   WRAP_OBJ_LAST(SensorConstructor),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CSensor", asBEHAVE_CONSTRUCT, "void f(const CSensor & in)", WRAP_OBJ_LAST(SensorCopyConstructor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CSensor", asBEHAVE_DESTRUCT,  "void f()",                   WRAP_OBJ_LAST(SensorDestructor),      asCALL_GENERIC) );

        // Register property
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v1", asOFFSET(CSensor, v1)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v2", asOFFSET(CSensor, v2)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v3", asOFFSET(CSensor, v3)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v4", asOFFSET(CSensor, v4)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v5", asOFFSET(CSensor, v5)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v6", asOFFSET(CSensor, v6)) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CSensor", "CSensor & opAssign(const CSensor & in)", WRAP_MFN_PR(CSensor, operator =, (const CSensor &), CSensor &),       asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CEvent & pollEvent()",                               WRAP_MFN(CActionMgr, pollEvent),                 asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasAction(const CEvent &in, string &in, uint)",       WRAP_OBJ_LAST(WasAction1),                       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint wasAction(const CEvent &in, string &in)",             WRAP_OBJ_LAST(WasAction2),                       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void enableAction(bool value = true)",                     WRAP_MFN(CActionMgr, enableAction),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool isAction()",                                          WRAP_MFN(CActionMgr, isAction),                  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void load(string &in)",                                    WRAP_MFN(CActionMgr, loadActionFromXML),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasActionEvent(string &in, int actionPress = 1)",     WRAP_MFN(CActionMgr, wasActionEvent),            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasGameEvent(uint type, int code = 0)",               WRAP_MFN(CActionMgr, wasGameEvent),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasKeyboardEvent(string &in, int actionPress = 1)",   WRAP_MFN(CActionMgr, wasKeyboardEvent),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasMouseBtnEvent(string &in, int actionPress = 1)",   WRAP_MFN(CActionMgr, wasMouseBtnEvent),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasGamepadBtnEvent(string &in, int actionPress = 1)", WRAP_MFN(CActionMgr, wasGamepadBtnEvent),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasEvent(uint)",                                      WRAP_MFN(CActionMgr, wasEvent),                  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getMouseAbsolutePos()",                     WRAP_MFN(CActionMgr, getMouseAbsolutePos),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getMouseRelativePos()",                     WRAP_MFN(CActionMgr, getMouseRelativePos),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getControllerPosLeft()",                    WRAP_MFN(CActionMgr, getControllerPosLeft),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getControllerPosRight()",                   WRAP_MFN(CActionMgr, getControllerPosRight),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceGamepad()",                              WRAP_MFN(CActionMgr, wasLastDeviceGamepad),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceKeyboard()",                             WRAP_MFN(CActionMgr, wasLastDeviceKeyboard),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceMouse()",                                WRAP_MFN(CActionMgr, wasLastDeviceMouse),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool isQueueEmpty()",                                      WRAP_MFN(CActionMgr, isQueueEmpty),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void saveToFile()",                                        WRAP_MFN(CActionMgr, saveToFile),                asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void resetKeyBindingsToDefault()",                         WRAP_MFN(CActionMgr, resetKeyBindingsToDefault), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasWindowEvent(uint, uint &out, int &out, int &out)", WRAP_MFN(CActionMgr, wasWindowEvent),            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "string getDeviceActionStr(int, string &in, bool &out)",    WRAP_MFN(CActionMgr, getDeviceActionStr),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "string resetAction(int, string &in, int, bool &out)",      WRAP_MFN(CActionMgr, resetAction),               asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateButtonEvents(uint &out, int &out, int &out, uint startIndex = 0)", WRAP_MFN(CActionMgr, enumerateButtonEvents), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateDisplayEvents(uint &out, uint &out, int &out, uint startIndex = 0)", WRAP_MFN(CActionMgr, enumerateDisplayEvents), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateMouseWheelEvents(uint &out, int &out, int &out, uint &out, uint startIndex = 0)", asFUNCTION(EnumerateMouseWheelEvents), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateWindowEvents(uint &out, uint &out, int &out, int &out, uint startIndex = 0)", asFUNCTION(EnumerateWindowEvents), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateTouchFingerEvents(uint &out, int64 &out, int64 &out, float &out, float &out, float &out, float &out, float &out, uint startIndex = 0)", asFUNCTION(EnumerateTouchFingerEvents), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateMultipleFingerEvents(int64 &out, float &out, float &out, float &out, float &out, uint &out, uint startIndex = 0)", asFUNCTION(EnumerateMultipleFingerEvents), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateDollarGestureEvents(int64 &out, int64 &out, uint &out, float &out, float &out, float &out, uint startIndex = 0)", asFUNCTION(EnumerateDollarGestureEvents), asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CActionMgr ActionMgr", &CActionMgr::Instance()) );
    }
}
