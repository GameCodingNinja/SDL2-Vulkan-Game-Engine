
/************************************************************************
*    FILE NAME:       scripttimer.cpp
*
*    DESCRIPTION:     CTimer script object registration
************************************************************************/

// Physical component dependency
#include <script/scripttimer.h>

// Game lib dependencies
#include <utilities/timer.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptTimer
{
    /************************************************************************
    *    DESC:  Constructor
    ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CTimer();
    }

    void ConstructorFromParam(double interval, bool startExpired, void * pThisPointer)
    {
        new(pThisPointer) CTimer(interval, startExpired);
    }

    /************************************************************************
    *    DESC:  Destructor
    ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CTimer*)pThisPointer)->~CTimer();
    }

    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CTimer", sizeof(CTimer), asOBJ_VALUE|asOBJ_APP_CLASS) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CTimer", asBEHAVE_CONSTRUCT, "void f()",                       WRAP_OBJ_LAST(Constructor),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CTimer", asBEHAVE_CONSTRUCT, "void f(double, bool s = false)", WRAP_OBJ_LAST(ConstructorFromParam),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CTimer", asBEHAVE_DESTRUCT,  "void f()",                       WRAP_OBJ_LAST(Destructor),            asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CTimer", "void setExpired()",                        WRAP_MFN(CTimer, setExpired),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimer", "bool expired(bool resetOnExpire = false)", WRAP_MFN(CTimer, expired),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimer", "void reset()",                             WRAP_MFN(CTimer, reset),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimer", "void set(double)",                         WRAP_MFN(CTimer, set),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimer", "void disable(bool disabled = true)",       WRAP_MFN(CTimer, disable),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimer", "void setDisableValue(bool)",               WRAP_MFN(CTimer, setDisableValue), asCALL_GENERIC) );
    }
}
