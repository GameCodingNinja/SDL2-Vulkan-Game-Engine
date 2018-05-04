
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
    CPoint<float> point;

    /************************************************************************
    *    DESC:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetAlpha(float value, CUIControl & control)
    {
        control.setAlpha(value);
    }

    float GetAlpha(CUIControl & control)
    {
        return control.getAlpha();
    }

    void SetVisible(bool value, CUIControl & control)
    {
        control.setVisible(value);
    }

    bool IsVisible(CUIControl & control)
    {
        return control.isVisible();
    }

    void SetPos1(const CPoint<float> & pos, CUIControl & control)
    {
        control.setPos(pos);
    }

    void SetPos2(float x, float y, float z, CUIControl & control)
    {
        control.setPos(x,y,z);
    }

    void IncPos1(const CPoint<float> & pos, CUIControl & control)
    {
        control.incPos(pos);
    }

    void IncPos2(float x, float y, float z, CUIControl & control)
    {
        control.incPos(x,y,z);
    }

    const CPoint<float> & GetPos(CUIControl & control)
    {
        point = control.getPos();
        return point;
    }

    void SetRot1(const CPoint<float> & rot, bool convertToRadians, CUIControl & control)
    {
        control.setRot(rot, convertToRadians);
    }

    void SetRot2(float x, float y, float z, bool convertToRadians, CUIControl & control)
    {
        control.setRot(x,y,z, convertToRadians);
    }

    void IncRot1(const CPoint<float> & rot, bool convertToRadians, CUIControl & control)
    {
        control.incRot(rot, convertToRadians);
    }

    void IncRot2(float x, float y, float z, bool convertToRadians, CUIControl & control)
    {
        control.incRot(x,y,z, convertToRadians);
    }

    const CPoint<float> & GetRot(CUIControl & control)
    {
        return control.getRot();
    }

    void SetScale1(const CPoint<float> & rot, CUIControl & control)
    {
        control.setScale(rot);
    }

    void SetScale2(float x, float y, float z, CUIControl & control)
    {
        control.setScale(x,y,z);
    }

    void IncScale1(const CPoint<float> & rot, CUIControl & control)
    {
        control.incScale(rot);
    }

    void IncScale2(float x, float y, float z, CUIControl & control)
    {
        control.incScale(x,y,z);
    }

    const CPoint<float> & GetScale(CUIControl & control)
    {
        return control.getScale();
    }

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
        
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isDisabled()",           asMETHOD(CUIControl, isDisabled),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isInactive()",           asMETHOD(CUIControl, isInactive),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isActive()",             asMETHOD(CUIControl, isActive),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isSelected()",           asMETHOD(CUIControl, isSelected),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isSelectable()",         asMETHOD(CUIControl, isSelectable), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setAlpha(float alpha)",  asFUNCTION(SetAlpha), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "float getAlpha()",            asFUNCTION(GetAlpha), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "const string & getName()",    asFUNCTION(GetName), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "const string & getGroup()",   asFUNCTION(GetGroup), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "const string & getFaction()", asFUNCTION(GetFaction), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "const string & getExecutionAction()", asMETHOD(CUIControl, getExecutionAction), asCALL_THISCALL) );

        // Object pos, rot & scale
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setVisible(bool)",                                 asFUNCTION(SetVisible), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "bool isVisible()",                                      asFUNCTION(IsVisible), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setPos(CPoint &in)",                               asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setPos(float x = 0, float y = 0, float z = 0)",    asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "void incPos(CPoint & in)",                              asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void incPos(float x = 0, float y = 0, float z = 0)",    asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "const CPoint & getPos()",                               asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "void incRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "const CPoint & getRot()",                               asFUNCTION(GetRot),    asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void setScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "void incScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CUIControl", "void incScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CUIControl", "const CPoint & getScale()",                             asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );
    }
}
