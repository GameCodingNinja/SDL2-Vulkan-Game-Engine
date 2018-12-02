
/************************************************************************
 *    FILE NAME:       scriptvisual.cpp
 *
 *    DESCRIPTION:     CVisual script object registration
 ************************************************************************/

// Physical component dependency
#include <script/scriptvisual.h>

// Game lib dependencies
#include <common/visual.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptVisual
{
    /************************************************************************
     *    DESC:  Constructor
     ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CVisual();
    }

    /************************************************************************
     *    DESC:  Destructor
     ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CVisual*)pThisPointer)->~CVisual();
    }
    
    
    /************************************************************************
    *    DESC:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetAdditiveColor1(const CColor & color, CVisual & visual)
    {
        visual.setAdditiveColor( color );
    }

    void SetAdditiveColor2(float r, float g, float b, float a, CVisual & visual)
    {
        visual.setAdditiveColor( r, g, b, a );
    }
    
    const CColor & GetAdditiveColor(CVisual & visual)
    {
        return visual.getAdditiveColor();
    }

    /************************************************************************
     *    DESC:  Register the class with AngelScript
     ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CVisual", sizeof(CVisual), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Class members
        Throw( pEngine->RegisterObjectMethod("CVisual", "void setAdditiveColor(const CColor &in)",           asFUNCTION(SetAdditiveColor1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CVisual", "void setAdditiveColor(float, float, float, float)", asFUNCTION(SetAdditiveColor2), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CVisual", "const CColor & getAdditiveColor() const",           asFUNCTION(GetAdditiveColor),  asCALL_CDECL_OBJLAST) );
    }
}
