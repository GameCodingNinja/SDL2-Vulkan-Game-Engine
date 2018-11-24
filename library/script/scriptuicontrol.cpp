
/************************************************************************
*    FILE NAME:       scriptuicontrol.cpp
*
*    DESCRIPTION:     CUIControl script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptuicontrol.h>

// Game lib dependencies
#include <gui/uicontrol.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptUIControl
{
    const std::string & GetName(CUIControl & control)
    {
        return control.getName();
    }

    const std::string & GetGroup(CUIControl & control)
    {
        return control.getGroup();
    }

    const std::string & GetFaction(CUIControl & control)
    {
        return control.getFaction();
    }

    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CUIControl", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setStringToList(string &in)",   asMETHOD(CUIControl, setStringToList), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void createFontString(string &in, int spriteIndex = 0)",          asMETHODPR(CUIControl, createFontString, (const std::string &, int), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void createFontString(int stringIndex = 0, int spriteIndex = 0)", asMETHODPR(CUIControl, createFontString, (int, int), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setActionType(string &in)",      asMETHODPR(CUIControl, setActionType, (const std::string &), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setExecutionAction(string &in)", asMETHOD(CUIControl, setExecutionAction),   asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isDisabled()",                   asMETHOD(CUIControl, isDisabled),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isInactive()",                   asMETHOD(CUIControl, isInactive),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isActive()",                     asMETHOD(CUIControl, isActive),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isSelected()",                   asMETHOD(CUIControl, isSelected),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isSelectable()",                 asMETHOD(CUIControl, isSelectable), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "const string & getName()",            asFUNCTION(GetName), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "const string & getGroup()",           asFUNCTION(GetGroup), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "const string & getFaction()",         asFUNCTION(GetFaction), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "const string & getExecutionAction()", asMETHOD(CUIControl, getExecutionAction), asCALL_THISCALL) );
        
        // Interface to complex controls for scripting
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void updateDisplay(int)",             asMETHOD(CUIControl, updateDisplay), asCALL_THISCALL) );
    }
}
