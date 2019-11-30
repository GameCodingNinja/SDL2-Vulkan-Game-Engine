
/************************************************************************
*    FILE NAME:       scriptstatcounter.cpp
*
*    DESCRIPTION:     CStatCounter script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptstatcounter.h>

// Game lib dependencies
#include <utilities/statcounter.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptStatCounter
{
    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CStatCounter", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CStatCounter", "void incCycle()",  WRAP_MFN(CStatCounter, incCycle), asCALL_GENERIC) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CStatCounter StatCounter", &CStatCounter::Instance()) );
    }
}