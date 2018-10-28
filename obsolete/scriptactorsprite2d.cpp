
/************************************************************************
*    FILE NAME:       scriptactorsprite2d.cpp
*
*    DESCRIPTION:     ActorSprite2D script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptactorsprite2d.h>

// Game lib dependencies
#include <2d/actorsprite2d.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptActorSprite2D
{
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CActorSprite2D", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "int GetId()",                                        asMETHOD(CActorSprite2D,   getId),            asCALL_THISCALL) );

        // Need to use CObject for the below to compile under MSVSC++
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void SetVisible(bool)",                              asMETHOD(CObject,   setVisible),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "bool IsVisible()",                                   asMETHOD(CObject,   isVisible),            asCALL_THISCALL) );
        
        
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void SetPos(CPoint &in)",                             asMETHODPR(CObject,   setPos, (const CPoint<float> &), void), asCALL_THISCALL) );
        //Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void SetPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, SetPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void IncPos(CPoint & in)",                            asMETHODPR(CObject, incPos, (const CPoint<float> &), void),   asCALL_THISCALL) );
        //Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void IncPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, IncPos, (float,float,float), void), asCALL_THISCALL) );
        
        //Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "CPoint GetPos()",                                       asMETHOD(CObject, getPosFloat),   asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void SetRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   setRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void SetRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject,   setRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void IncRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   incRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void IncRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject, incRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "const CPoint & GetRot()",                               asMETHOD(CObject, getRot),               asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void SetScale(CPoint & in)",                            asMETHODPR(CObject, setScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void SetScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, setScale, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void IncScale(CPoint & in)",                            asMETHODPR(CObject, incScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "void IncScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, incScale, (float,float,float), void), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CActorSprite2D", "const CPoint & GetScale()",     asMETHOD(CObject, getScale),             asCALL_THISCALL) );
    }
}