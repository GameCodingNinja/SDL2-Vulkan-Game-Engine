
/************************************************************************
*    FILE NAME:       scriptmenu.cpp
*
*    DESCRIPTION:     CMenu script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptmenu.h>

// Game lib dependencies
#include <gui/menu.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptMenu
{
    CPoint<float> point;

    /************************************************************************
    *    DESC:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetVisible(bool value, CMenu & menu)
    {
        menu.setVisible(value);
    }

    bool IsVisible(CMenu & menu)
    {
        return menu.isVisible();
    }

    void SetPos1(const CPoint<float> & pos, CMenu & menu)
    {
        menu.setPos(pos);
    }

    void SetPos2(float x, float y, float z, CMenu & menu)
    {
        menu.setPos(x,y,z);
    }

    void IncPos1(const CPoint<float> & pos, CMenu & menu)
    {
        menu.incPos(pos);
    }

    void IncPos2(float x, float y, float z, CMenu & menu)
    {
        menu.incPos(x,y,z);
    }

    const CPoint<float> & GetPos(CMenu & menu)
    {
        point = menu.getPos();
        return point;
    }

    void SetRot1(const CPoint<float> & rot, bool convertToRadians, CMenu & menu)
    {
        menu.setRot(rot, convertToRadians);
    }

    void SetRot2(float x, float y, float z, bool convertToRadians, CMenu & menu)
    {
        menu.setRot(x,y,z, convertToRadians);
    }

    void IncRot1(const CPoint<float> & rot, bool convertToRadians, CMenu & menu)
    {
        menu.incRot(rot, convertToRadians);
    }

    void IncRot2(float x, float y, float z, bool convertToRadians, CMenu & menu)
    {
        menu.incRot(x,y,z, convertToRadians);
    }

    const CPoint<float> & GetRot(CMenu & menu)
    {
        return menu.getRot();
    }

    void SetScale1(const CPoint<float> & rot, CMenu & menu)
    {
        menu.setScale(rot);
    }

    void SetScale2(float x, float y, float z, CMenu & menu)
    {
        menu.setScale(x,y,z);
    }

    void IncScale1(const CPoint<float> & rot, CMenu & menu)
    {
        menu.incScale(rot);
    }

    void IncScale2(float x, float y, float z, CMenu & menu)
    {
        menu.incScale(x,y,z);
    }

    const CPoint<float> & GetScale(CMenu & menu)
    {
        return menu.getScale();
    }

    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CMenu", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "void setAlpha(float alpha)",                            WRAP_MFN(CMenu, setAlpha),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "float getAlpha()",                                      WRAP_MFN(CMenu, getAlpha),              asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "uiControl & getControl(string &in)",                    WRAP_MFN(CMenu, getPtrToControl),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "uiControl & getActiveControl()",                        WRAP_MFN(CMenu, getPtrToActiveControl), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "bool isActive()",                                       WRAP_MFN(CMenu, isActive),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "void activateFirstInactiveControl()",                   WRAP_MFN(CMenu, activateFirstInactiveControl), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "void deactivateAllControls()",                          WRAP_MFN(CMenu, deactivateAllControls), asCALL_GENERIC) );

        // Object pos, rot & scale
        Throw( pEngine->RegisterObjectMethod("CMenu", "void setVisible(bool)",                                 WRAP_OBJ_LAST(SetVisible), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "bool isVisible()",                                      WRAP_OBJ_LAST(IsVisible),  asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "void setPos(CPoint &in)",                               WRAP_OBJ_LAST(SetPos1),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "void setPos(float x = 0, float y = 0, float z = 0)",    WRAP_OBJ_LAST(SetPos2),    asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "void incPos(CPoint & in)",                              WRAP_OBJ_LAST(IncPos1),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "void incPos(float x = 0, float y = 0, float z = 0)",    WRAP_OBJ_LAST(IncPos2),    asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "const CPoint & getPos()",                               WRAP_OBJ_LAST(GetPos),     asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "void setRot(CPoint &in, bool convertToRadians = true)", WRAP_OBJ_LAST(SetRot1),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", WRAP_OBJ_LAST(SetRot2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "void incRot(CPoint &in, bool convertToRadians = true)", WRAP_OBJ_LAST(IncRot1),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", WRAP_OBJ_LAST(IncRot2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "const CPoint & getRot()",                               WRAP_OBJ_LAST(GetRot),     asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "void setScale(CPoint & in)",                            WRAP_OBJ_LAST(SetScale1),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "void setScale(float x = 1, float y = 1, float z = 1)",  WRAP_OBJ_LAST(SetScale2),  asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "void incScale(CPoint & in)",                            WRAP_OBJ_LAST(IncScale1),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "void incScale(float x = 1, float y = 1, float z = 1)",  WRAP_OBJ_LAST(IncScale2),  asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CMenu", "const CPoint & getScale()",                             WRAP_OBJ_LAST(GetScale),   asCALL_GENERIC) );
    }
}
