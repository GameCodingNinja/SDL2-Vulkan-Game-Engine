
/************************************************************************
*    FILE NAME:       scriptisprite.cpp
*
*    DESCRIPTION:     iSprite script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptisprite.h>

// Game lib dependencies
#include <common/isprite.h>
#include <common/ivisualcomponent.h>
#include <physics/iphysicscomponent.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptiSprite
{
    CPoint<float> point;

    /************************************************************************
    *    DESC:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetVisible(bool value, iSprite & sprite)
    {
        sprite.setVisible(value);
    }

    bool IsVisible(iSprite & sprite)
    {
        return sprite.isVisible();
    }

    void SetPos1(const CPoint<float> & pos, iSprite & sprite)
    {
        sprite.setPos(pos);
    }

    void SetPos2(float x, float y, float z, iSprite & sprite)
    {
        sprite.setPos(x,y,z);
    }

    void IncPos1(const CPoint<float> & pos, iSprite & sprite)
    {
        sprite.incPos(pos);
    }

    void IncPos2(float x, float y, float z, iSprite & sprite)
    {
        sprite.incPos(x,y,z);
    }

    const CPoint<float> & GetPos(iSprite & sprite)
    {
        point = sprite.getPos();
        return point;
    }

    void SetRot1(const CPoint<float> & rot, bool convertToRadians, iSprite & sprite)
    {
        sprite.setRot(rot, convertToRadians);
    }

    void SetRot2(float x, float y, float z, bool convertToRadians, iSprite & sprite)
    {
        sprite.setRot(x,y,z, convertToRadians);
    }

    void IncRot1(const CPoint<float> & rot, bool convertToRadians, iSprite & sprite)
    {
        sprite.incRot(rot, convertToRadians);
    }

    void IncRot2(float x, float y, float z, bool convertToRadians, iSprite & sprite)
    {
        sprite.incRot(x,y,z, convertToRadians);
    }

    const CPoint<float> & GetRot(iSprite & sprite)
    {
        return sprite.getRot();
    }

    void SetScale1(const CPoint<float> & rot, iSprite & sprite)
    {
        sprite.setScale(rot);
    }

    void SetScale2(float x, float y, float z, iSprite & sprite)
    {
        sprite.setScale(x,y,z);
    }

    void IncScale1(const CPoint<float> & rot, iSprite & sprite)
    {
        sprite.incScale(rot);
    }

    void IncScale2(float x, float y, float z, iSprite & sprite)
    {
        sprite.incScale(x,y,z);
    }

    const CPoint<float> & GetScale(iSprite & sprite)
    {
        return sprite.getScale();
    }

    void SetColor1(const CColor & color, iSprite & sprite)
    {
        sprite.getVisualInterface()->setColor( color );
    }
    
    void SetColor2(const CColor & color, iSprite & sprite)
    {
        sprite.getVisualInterface()->setColor( color );
    }
    
    const CColor & GetColor(iSprite & sprite)
    {
        return sprite.getVisualInterface()->getColor();
    }
    
    void SetDefaultColor(iSprite & sprite)
    {
        sprite.getVisualInterface()->setDefaultColor();
    }
    
    const CColor & GetDefaultColor(iSprite & sprite)
    {
        return sprite.getVisualInterface()->getDefaultColor();
    }
    
    void SetAlpha(float alpha, bool allowToExceed, iSprite & sprite)
    {
        sprite.getVisualInterface()->setAlpha( alpha, allowToExceed );
    }
    
    float GetAlpha(iSprite & sprite)
    {
        return sprite.getVisualInterface()->getAlpha();
    }

    void SetDefaultAlpha(iSprite & sprite)
    {
        sprite.getVisualInterface()->setDefaultAlpha();
    }

    float GetDefaultAlpha(iSprite & sprite)
    {
        return sprite.getVisualInterface()->getDefaultAlpha();
    }

    void CreateFontString(const std::string & fontStr, iSprite & sprite)
    {
        sprite.getVisualInterface()->createFontString(fontStr);
    }

    const std::string & GetFontString(iSprite & sprite)
    {
        return sprite.getVisualInterface()->getFontString();
    }
    
    const CSize<float> & GetFontSize(iSprite & sprite)
    {
        return sprite.getVisualInterface()->getFontSize();
    }

    bool IsFontSprite(iSprite & sprite)
    {
        return sprite.getVisualInterface()->isFontSprite();
    }

    void SetTransform(float x, float y, float angle, bool resetVelocity, iSprite & sprite)
    {
        sprite.getPhysicsInterface()->setTransform( x, y, angle, resetVelocity );
    }

    void SetLinearVelocity(float x, float y, iSprite & sprite)
    {
        sprite.getPhysicsInterface()->setLinearVelocity( x, y );
    }
    
    void SetAngularVelocity(float angle, iSprite & sprite)
    {
        sprite.getPhysicsInterface()->setAngularVelocity( angle );
    }
    
    void ApplyAngularImpulse(float value, bool wake, iSprite & sprite)
    {
        sprite.getPhysicsInterface()->applyAngularImpulse( value, wake );
    }
    
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "iSprite", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        // Visual component functions
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setColor(const CColor &in)",           asFUNCTION(SetColor1),       asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setColor(float, float, float, float)", asFUNCTION(SetColor2),       asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "const CColor & getColor()",                 asFUNCTION(GetColor),        asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setDefaultColor()",                    asFUNCTION(SetDefaultColor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "const CColor & getDefaultColor()",          asFUNCTION(GetDefaultColor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setAlpha(float, bool allowToExceed = false)", asFUNCTION(SetAlpha), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "float getAlpha()",                                 asFUNCTION(GetAlpha), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setDefaultAlpha()",                           asFUNCTION(SetDefaultAlpha), asCALL_CDECL_OBJLAST)  );
        Throw( pEngine->RegisterObjectMethod("iSprite", "float getDefaultAlpha()",                          asFUNCTION(GetDefaultAlpha), asCALL_CDECL_OBJLAST)  );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void createFontString(string &in)",       asFUNCTION(CreateFontString), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "const string & getFontString()",          asFUNCTION(GetFontString), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "const CSize & getFontSize()",    asFUNCTION(GetFontSize), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "bool isFontSprite()",            asFUNCTION(IsFontSprite), asCALL_CDECL_OBJLAST) );
        
        // Physics component functions
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setPhysicsTransform(float, float, float angle = 0, bool resetVelocity = true)", asFUNCTION(SetTransform), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setLinearVelocity(float, float)", asFUNCTION(SetLinearVelocity), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setAngularVelocity(float)",       asFUNCTION(SetAngularVelocity), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void applyAngularImpulse(float, bool wake = false)",      asFUNCTION(ApplyAngularImpulse), asCALL_CDECL_OBJLAST) );

        // Sprite specific functions
        Throw( pEngine->RegisterObjectMethod("iSprite", "uint getFrameCount()",                               asMETHOD(iSprite, getFrameCount),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setFrame(uint)",                                asMETHOD(iSprite, setFrame),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "uint getCurrentFrame()",                             asMETHOD(iSprite, getCurrentFrame),      asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "int getId()",                                        asMETHOD(iSprite,   getId),            asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "void setVisible(bool)",                              asFUNCTION(SetVisible), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "bool isVisible()",                                   asFUNCTION(IsVisible), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "void setPos(CPoint &in)",                             asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "void incPos(CPoint & in)",                            asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "const CPoint & getPos()",                             asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "void setRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "void incRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "const CPoint & getRot()",                               asFUNCTION(GetRot),    asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "void setScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "void incScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "const CPoint & getScale()",     asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, iSprite @)", asMETHOD(CScriptMgr, prepareSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void LocalSpawn(string &in, iSprite @)", asMETHOD(CScriptMgr, prepareLocalSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptMgr::Instance()) );
    }
}