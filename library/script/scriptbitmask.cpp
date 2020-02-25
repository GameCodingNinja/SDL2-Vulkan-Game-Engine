
/************************************************************************
*    FILE NAME:       scriptbitmask.cpp
*
*    DESCRIPTION:     CBitmask script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptbitmask.h>

// Game lib dependencies
#include <utilities/bitmask.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptBitmask
{
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CBitmask", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CBitmask", "void add(uint)",             WRAP_MFN(CBitmask<int>, add),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CBitmask", "void remove(uint)",          WRAP_MFN(CBitmask<int>, remove),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CBitmask", "void removeAllExcept(uint)", WRAP_MFN(CBitmask<int>, removeAllExcept), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CBitmask", "void clear()",               WRAP_MFN(CBitmask<int>, clear),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CBitmask", "bool isEmpty()",             WRAP_MFN(CBitmask<int>, isEmpty),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CBitmask", "bool isSet(uint)",           WRAP_MFN(CBitmask<int>, isSet),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CBitmask", "bool areAllSet(uint)",       WRAP_MFN(CBitmask<int>, areAllSet),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CBitmask", "int getIncluding(uint)",     WRAP_MFN(CBitmask<int>, getIncluding),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CBitmask", "int getExcluding(uint)",     WRAP_MFN(CBitmask<int>, getExcluding),    asCALL_GENERIC) );
    }
}
