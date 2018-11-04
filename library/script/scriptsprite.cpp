
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

    void IncPos1(const CPoint<float> & pos, CSprite & sprite)
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

    void SetAdditiveColor1(const CColor & color, CSprite & sprite)
    {
        sprite.getVisualComponent()->setAdditiveColor( color );
    }

    void SetAdditiveColor2(float r, float g, float b, float a, CSprite & sprite)
    {
        sprite.getVisualComponent()->setAdditiveColor( r, g, b, a );
    }

    const CColor & GetAdditiveColor(CSprite & sprite)
    {
        return sprite.getVisualComponent()->getAdditiveColor();
    }

    void CreateFontString(const std::string & fontStr, CSprite & sprite)
    {
        sprite.getVisualComponent()->createFontString(fontStr);
    }

    const std::string & GetFontString(CSprite & sprite)
    {
        return sprite.getVisualComponent()->getFontString();
    }

    const CSize<float> & GetFontSize(CSprite & sprite)
    {
        return sprite.getVisualComponent()->getFontSize();
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
        Throw( pEngine->RegisterObjectType(  "Sprite", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        // Visual component functions
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setColor(const CColor &in)",                   asFUNCTION(SetColor1),       asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setColor(float, float, float, float)",         asFUNCTION(SetColor2),       asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "const CColor & getColor()",                         asFUNCTION(GetColor),        asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setAdditiveColor(const CColor &in)",           asFUNCTION(SetAdditiveColor1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setAdditiveColor(float, float, float, float)", asFUNCTION(SetAdditiveColor2), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "const CColor & getAdditiveColor()",                 asFUNCTION(GetAdditiveColor),  asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setDefaultColor()",                            asFUNCTION(SetDefaultColor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "const CColor & getDefaultColor()",                  asFUNCTION(GetDefaultColor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setAlpha(float, bool allowToExceed = false)",  asFUNCTION(SetAlpha), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "float getAlpha()",                                  asFUNCTION(GetAlpha), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setDefaultAlpha()",                            asFUNCTION(SetDefaultAlpha), asCALL_CDECL_OBJLAST)  );
        Throw( pEngine->RegisterObjectMethod("Sprite", "float getDefaultAlpha()",                           asFUNCTION(GetDefaultAlpha), asCALL_CDECL_OBJLAST)  );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void createFontString(string &in)",                 asFUNCTION(CreateFontString), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "const string & getFontString()",                    asFUNCTION(GetFontString), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "const CSize & getFontSize()",                       asFUNCTION(GetFontSize), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "bool isFontSprite()",                               asFUNCTION(IsFontSprite), asCALL_CDECL_OBJLAST) );

        // Physics component functions
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setPhysicsTransform(float, float, float angle = 0, bool resetVelocity = true)", asFUNCTION(SetTransform), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setLinearVelocity(float, float)", asFUNCTION(SetLinearVelocity), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setAngularVelocity(float)",       asFUNCTION(SetAngularVelocity), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void applyAngularImpulse(float, bool wake = false)",      asFUNCTION(ApplyAngularImpulse), asCALL_CDECL_OBJLAST) );

        // Sprite specific functions
        Throw( pEngine->RegisterObjectMethod("Sprite", "uint getFrameCount()",                               asMETHOD(CSprite, getFrameCount),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setFrame(uint)",                                asMETHOD(CSprite, setFrame),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "uint getCurrentFrame()",                             asMETHOD(CSprite, getCurrentFrame),      asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "int getId()",                                        asMETHOD(CSprite,   getId),            asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "void setVisible(bool)",                              asFUNCTION(SetVisible), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "bool isVisible()",                                   asFUNCTION(IsVisible), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "void setPos(CPoint &in)",                             asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "void incPos(CPoint & in)",                            asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void incPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "const CPoint & getPos()",                             asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "void setRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "void incRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "const CPoint & getRot()",                               asFUNCTION(GetRot),    asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "void setScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void setScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "void incScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("Sprite", "void incScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("Sprite", "const CPoint & getScale()",     asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );

        //Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, Sprite @)", asMETHOD(CScriptMgr, prepareSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptMgr::Instance()) );
        //Throw( pEngine->RegisterGlobalFunction("void LocalSpawn(string &in, Sprite @)", asMETHOD(CScriptMgr, prepareLocalSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptMgr::Instance()) );
    }
}