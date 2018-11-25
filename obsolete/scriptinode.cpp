
/************************************************************************
*    FILE NAME:       scriptinode.cpp
*
*    DESCRIPTION:     iNode script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptinode.h>

// Game lib dependencies
#include <node/inode.h>
#include <sprite/sprite.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptiNode
{
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "iNode", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        // iNode specific functions
        Throw( pEngine->RegisterObjectMethod("iNode", "CSprite & getSprite()", asMETHOD(iNode, getSprite), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "iNode & next()",        asMETHOD(iNode, next), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "int getType()",         asMETHOD(iNode, getType), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "int getId()",           asMETHOD(iNode, getId), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iNode", "void resetIterators()", asMETHOD(iNode, resetIterators), asCALL_THISCALL) );
    }
}