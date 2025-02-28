
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
        return actionMgr.wasAction(rEvent, actionStr, EActionPress(actionPress));
    }

    uint WasAction2(const SDL_Event & rEvent, const std::string & actionStr, CActionMgr & actionMgr)
    {
        return static_cast<int>(actionMgr.wasAction(rEvent, actionStr));
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
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "string getDeviceActionStr(int, string &in, bool &out)",    WRAP_MFN(CActionMgr, getDeviceActionStr),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "string resetAction(int, string &in, int, bool &out)",      WRAP_MFN(CActionMgr, resetAction),               asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void resetLastUsedDevice()",                               WRAP_MFN(CActionMgr, resetLastUsedDevice),       asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateButtonEvents(uint &out, int &out, int &out, uint startIndex = 0)", WRAP_MFN(CActionMgr, enumerateButtonEvents), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateMouseWheelEvents(uint &out, int &out, int &out, uint &out, uint startIndex = 0)", WRAP_MFN(CActionMgr, enumerateMouseWheelEvents), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateTouchFingerEvents(uint &out, int64 &out, int64 &out, float &out, float &out, float &out, float &out, float &out, uint startIndex = 0)", WRAP_MFN(CActionMgr, enumerateTouchFingerEvents), asCALL_GENERIC) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CActionMgr ActionMgr", &CActionMgr::Instance()) );
    }
}
