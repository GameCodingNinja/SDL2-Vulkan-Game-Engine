
/************************************************************************
*    FILE NAME:       scriptsprite.cpp
*
*    DESCRIPTION:     Sprite script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsprite.h>

// Game lib dependencies
#include <2d/object2d.h>
#include <sprite/sprite.h>
#include <common/ivisualcomponent.h>
#include <common/point.h>
#include <physics/iphysicscomponent.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSprite
{
    CPoint<float> point;

    /************************************************************************
    *    DESC:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetVisible(bool value, CSprite & sprite)
    {
        sprite.getObject()->setVisible(value);
    }

    bool IsVisible(CSprite & sprite)
    {
        return sprite.getObject()->isVisible();
    }

    void SetPos1(const CPoint<float> & pos, CSprite & sprite)
    {
        sprite.getObject()->setPos(pos);
    }

    void SetPos2(float x, float y, float z, CSprite & sprite)
    {
        sprite.getObject()->setPos(x,y,z);
    }

    void IncPos1(CPoint<float> & pos, CSprite & sprite)
    {
        sprite.getObject()->incPos(pos);
    }

    void IncPos2(float x, float y, float z, CSprite & sprite)
    {
        sprite.getObject()->incPos(x,y,z);
    }

    const CPoint<float> & GetPos(CSprite & sprite)
    {
        point = sprite.getObject()->getPos();
        return point;
    }
    
    const CPoint<float> & GetWorldPos(CSprite & sprite)
    {
        point = sprite.getObject()->getWorldPos();
        return point;
    }
    
    const CPoint<float> & GetCenterPos(CSprite & sprite)
    {
        point = sprite.getObject()->getCenterPos();
        return point;
    }

    void SetRot1(const CPoint<float> & rot, bool convertToRadians, CSprite & sprite)
    {
        sprite.getObject()->setRot(rot, convertToRadians);
    }

    void SetRot2(float x, float y, float z, bool convertToRadians, CSprite & sprite)
    {
        sprite.getObject()->setRot(x,y,z, convertToRadians);
    }

    void IncRot1(const CPoint<float> & rot, bool convertToRadians, CSprite & sprite)
    {
        sprite.getObject()->incRot(rot, convertToRadians);
    }

    void IncRot2(float x, float y, float z, bool convertToRadians, CSprite & sprite)
    {
        sprite.getObject()->incRot(x,y,z, convertToRadians);
    }

    const CPoint<float> & GetRot(CSprite & sprite)
    {
        return sprite.getObject()->getRot();
    }

    void SetScale1(const CPoint<float> & rot, CSprite & sprite)
    {
        sprite.getObject()->setScale(rot);
    }

    void SetScale2(float x, float y, float z, CSprite & sprite)
    {
        sprite.getObject()->setScale(x,y,z);
    }

    void IncScale1(const CPoint<float> & rot, CSprite & sprite)
    {
        sprite.getObject()->incScale(rot);
    }

    void IncScale2(float x, float y, float z, CSprite & sprite)
    {
        sprite.getObject()->incScale(x,y,z);
    }

    const CPoint<float> & GetScale(CSprite & sprite)
    {
        return sprite.getObject()->getScale();
    }

    void SetColor1(const CColor & color, CSprite & sprite)
    {
        sprite.getVisualComponent()->setColor( color );
    }

    void SetColor2(float r, float g, float b, float a, CSprite & sprite)
    {
        sprite.getVisualComponent()->setColor( r, g, b, a );
    }

    const CColor & GetColor(CSprite & sprite)
    {
        return sprite.getVisualComponent()->getColor();
    }

    void SetDefaultColor(CSprite & sprite)
    {
        sprite.getVisualComponent()->setDefaultColor();
    }

    const CColor & GetDefaultColor(CSprite & sprite)
    {
        return sprite.getVisualComponent()->getDefaultColor();
    }

    void SetAlpha(float alpha, bool allowToExceed, CSprite & sprite)
    {
        sprite.getVisualComponent()->setAlpha( alpha, allowToExceed );
    }

    float GetAlpha(CSprite & sprite)
    {
        return sprite.getVisualComponent()->getAlpha();
    }

    void SetDefaultAlpha(CSprite & sprite)
    {
        sprite.getVisualComponent()->setDefaultAlpha();
    }

    float GetDefaultAlpha(CSprite & sprite)
    {
        return sprite.getVisualComponent()->getDefaultAlpha();
    }

    void CreateFontString(const std::string & fontStr, CSprite & sprite)
    {
        sprite.getVisualComponent()->createFontString(fontStr);
    }

    const std::string & GetFontString(CSprite & sprite)
    {
        return sprite.getVisualComponent()->getFontString();
    }

    bool IsFontSprite(CSprite & sprite)
    {
        return sprite.getVisualComponent()->isFontSprite();
    }

    void SetTransform(float x, float y, float angle, bool resetVelocity, CSprite & sprite)
    {
        sprite.getPhysicsComponent()->setTransform( x, y, angle, resetVelocity );
    }

    void SetLinearVelocity(float x, float y, CSprite & sprite)
    {
        sprite.getPhysicsComponent()->setLinearVelocity( x, y );
    }

    void SetAngularVelocity(float angle, CSprite & sprite)
    {
        sprite.getPhysicsComponent()->setAngularVelocity( angle );
    }

    void ApplyAngularImpulse(float value, bool wake, CSprite & sprite)
    {
        sprite.getPhysicsComponent()->applyAngularImpulse( value, wake );
    }

    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CSprite", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        // Visual component functions
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setColor(const CColor &in)",                   asFUNCTION(SetColor1),       asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setColor(float, float, float, float)",         asFUNCTION(SetColor2),       asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const CColor & getColor() const",                   asFUNCTION(GetColor),        asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setDefaultColor()",                            asFUNCTION(SetDefaultColor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const CColor & getDefaultColor() const",            asFUNCTION(GetDefaultColor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setAlpha(float, bool allowToExceed = false)",  asFUNCTION(SetAlpha), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "float getAlpha() const",                            asFUNCTION(GetAlpha), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setDefaultAlpha()",                            asFUNCTION(SetDefaultAlpha), asCALL_CDECL_OBJLAST)  );
        Throw( pEngine->RegisterObjectMethod("CSprite", "float getDefaultAlpha() const",                     asFUNCTION(GetDefaultAlpha), asCALL_CDECL_OBJLAST)  );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void createFontString(string &in)",                 asFUNCTION(CreateFontString), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const string & getFontString() const",              asFUNCTION(GetFontString), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "bool isFontSprite() const",                         asFUNCTION(IsFontSprite), asCALL_CDECL_OBJLAST) );

        // Physics component functions
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setPhysicsTransform(float, float, float angle = 0, bool resetVelocity = true)", asFUNCTION(SetTransform), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setLinearVelocity(float, float)", asFUNCTION(SetLinearVelocity), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setAngularVelocity(float)",       asFUNCTION(SetAngularVelocity), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void applyAngularImpulse(float, bool wake = false)",      asFUNCTION(ApplyAngularImpulse), asCALL_CDECL_OBJLAST) );

        // Sprite specific functions
        Throw( pEngine->RegisterObjectMethod("CSprite", "CSize getSize() const",                               asMETHOD(CSprite, getSize),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "uint getFrameCount() const",                          asMETHOD(CSprite, getFrameCount),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setFrame(uint)",                                 asMETHOD(CSprite, setFrame),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "uint getCurrentFrame() const",                        asMETHOD(CSprite, getCurrentFrame),      asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "int getId() const",                                   asMETHOD(CSprite,   getId),            asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite", "bool prepare(string &in, bool forceUpdate = false)",  asMETHOD(CSprite,   prepare),            asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void setVisible(bool)",                               asFUNCTION(SetVisible), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "bool isVisible() const",                              asFUNCTION(IsVisible), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void setPos(CPoint &in)",                             asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void incPos(CPoint & in)",                            asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void incPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getPos() const",                       asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getWorldPos() const",                  asFUNCTION(GetWorldPos),  asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getCenterPos() const",                 asFUNCTION(GetCenterPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void setRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void incRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getRot() const",                         asFUNCTION(GetRot),    asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void setScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void incScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void incScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getScale() const",                      asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );
    }
}