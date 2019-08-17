
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
        Throw( pEngine->RegisterObjectBehaviour("CSensor", asBEHAVE_CONSTRUCT, "void f()",                           asFUNCTION(SensorConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSensor", asBEHAVE_CONSTRUCT, "void f(const CSensor & in)",         asFUNCTION(SensorCopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSensor", asBEHAVE_DESTRUCT,  "void f()",                           asFUNCTION(SensorDestructor), asCALL_CDECL_OBJLAST) );

        // Register property
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v1", asOFFSET(CSensor, v1)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v2", asOFFSET(CSensor, v2)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v3", asOFFSET(CSensor, v3)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v4", asOFFSET(CSensor, v4)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v5", asOFFSET(CSensor, v5)) );
        Throw( pEngine->RegisterObjectProperty("CSensor", "float v6", asOFFSET(CSensor, v6)) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CSensor", "CSensor & opAssign(const CSensor & in)", asMETHODPR(CSensor, operator =, (const CSensor &), CSensor &), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void enableAction(bool value = true)",                     asMETHOD(CActionMgr, enableAction),          asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool isAction()",                                          asMETHOD(CActionMgr, isAction),              asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void load(string &in)",                                    asMETHOD(CActionMgr, loadActionFromXML),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasAction(string &in, int actionPress = 1)",          asMETHOD(CActionMgr, wasActionEvent),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasGameEvent(uint type, int code = 0)",               asMETHOD(CActionMgr, wasGameEvent),          asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasKeyboardEvent(string &in, int actionPress = 1)",   asMETHOD(CActionMgr, wasKeyboardEvent),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasMouseBtnEvent(string &in, int actionPress = 1)",   asMETHOD(CActionMgr, wasMouseBtnEvent),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasGamepadBtnEvent(string &in, int actionPress = 1)", asMETHOD(CActionMgr, wasGamepadBtnEvent),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasEvent(uint)",                                      asMETHOD(CActionMgr, wasEvent),              asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getMouseAbsolutePos()",                     asMETHOD(CActionMgr, getMouseAbsolutePos),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getMouseRelativePos()",                     asMETHOD(CActionMgr, getMouseRelativePos),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getControllerPosLeft()",                    asMETHOD(CActionMgr, getControllerPosLeft),  asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getControllerPosRight()",                   asMETHOD(CActionMgr, getControllerPosRight), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceGamepad()",                              asMETHOD(CActionMgr, wasLastDeviceGamepad),  asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceKeyboard()",                             asMETHOD(CActionMgr, wasLastDeviceKeyboard), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceMouse()",                                asMETHOD(CActionMgr, wasLastDeviceMouse),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool isQueueEmpty()",                                      asMETHOD(CActionMgr, isQueueEmpty),          asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void saveToFile()",                                        asMETHOD(CActionMgr, saveToFile),            asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void resetKeyBindingsToDefault()",                         asMETHOD(CActionMgr, resetKeyBindingsToDefault), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasWindowEvent(uint, uint &out, int &out, int &out)", asMETHOD(CActionMgr, wasWindowEvent),        asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "string getDeviceActionStr(int, string &in, bool &out)", asMETHOD(CActionMgr, getDeviceActionStr), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "string resetAction(int, string &in, int, bool &out)",   asMETHOD(CActionMgr, resetAction), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateButtonEvents(uint &out, int &out, int &out, uint startIndex = 0)", asMETHOD(CActionMgr, enumerateButtonEvents), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateDisplayEvents(uint &out, uint &out, int &out, uint startIndex = 0)", asMETHOD(CActionMgr, enumerateDisplayEvents), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateMouseWheelEvents(uint &out, int &out, int &out, uint &out, uint startIndex = 0)", asMETHOD(CActionMgr, enumerateMouseWheelEvents), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateWindowEvents(uint &out, uint &out, int &out, int &out, uint startIndex = 0)", asMETHOD(CActionMgr, enumerateWindowEvents), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateTouchFingerEvents(uint &out, int64 &out, int64 &out, float &out, float &out, float &out, float &out, float &out, uint startIndex = 0)", asMETHOD(CActionMgr, enumerateTouchFingerEvents), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateMultipleFingerEvents(int64 &out, float &out, float &out, float &out, float &out, uint &out, uint startIndex = 0)", asMETHOD(CActionMgr, enumerateMultipleFingerEvents), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "uint enumerateDollarGestureEvents(int64 &out, int64 &out, uint &out, float &out, float &out, float &out, uint startIndex = 0)", asMETHOD(CActionMgr, enumerateDollarGestureEvents), asCALL_THISCALL) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CActionMgr ActionMgr", &CActionMgr::Instance()) );
    }
}
