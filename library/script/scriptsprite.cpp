
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
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptSprite
{
    CPoint<float> point;
    CSize<float> size;
    CColor color;
    std::string string;

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
    
    const CSize<float> & GetSize(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            return sprite.getVisualComponent()->getSize();
        
        NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );
        return size;
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
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            component->setColor( color );
        else
            NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );
    }

    void SetColor2(float r, float g, float b, float a, CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            component->setColor( r, g, b, a );
        else
            NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );
    }

    const CColor & GetColor(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            return component->getColor();
        
        NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );

        return color;
    }

    void SetDefaultColor(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            component->setDefaultColor();
        else
            NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );
    }

    const CColor & GetDefaultColor(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            return component->getDefaultColor();
        
        NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );

        return color;
    }

    void SetAlpha(float alpha, bool allowToExceed, CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            component->setAlpha( alpha, allowToExceed );
        else
            NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );
    }

    float GetAlpha(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            return component->getAlpha();
        
        NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );

        return 1.f;
    }

    void SetDefaultAlpha(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            component->setDefaultAlpha();
        else
            NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );
    }

    float GetDefaultAlpha(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            return component->getDefaultAlpha();
        
        NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );

        return 1.f;
    }

    void CreateFontString(const std::string & fontStr, CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            component->createFontString(fontStr);
        else
            NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );
    }

    const std::string & GetFontString(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            return component->getFontString();
        
        NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );

        return string;
    }

    bool IsFontSprite(CSprite & sprite)
    {
        auto component = sprite.getVisualComponent();
        if( component != nullptr )
            return component->isFontSprite();
        
        NGenFunc::PostDebugMsg( "WARNING: Visual component does not exist." );

        return false;
    }

    void SetTransform(float x, float y, float angle, bool resetVelocity, CSprite & sprite)
    {
        auto component = sprite.getPhysicsComponent();
        if( component != nullptr )
            component->setTransform( x, y, angle, resetVelocity );
        else
            NGenFunc::PostDebugMsg( "WARNING: Physics component does not exist." );
    }

    void SetLinearVelocity(float x, float y, CSprite & sprite)
    {
        auto component = sprite.getPhysicsComponent();
        if( component != nullptr )
            component->setLinearVelocity( x, y );
        else
            NGenFunc::PostDebugMsg( "WARNING: Physics component does not exist." );
    }

    void SetAngularVelocity(float angle, CSprite & sprite)
    {
        auto component = sprite.getPhysicsComponent();
        if( component != nullptr )
            component->setAngularVelocity( angle );
        else
            NGenFunc::PostDebugMsg( "WARNING: Physics component does not exist." );
    }

    void ApplyAngularImpulse(float value, bool wake, CSprite & sprite)
    {
        auto component = sprite.getPhysicsComponent();
        if( component != nullptr )
            component->applyAngularImpulse( value, wake );
        else
            NGenFunc::PostDebugMsg( "WARNING: Physics component does not exist." );
    }

    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType( "CSprite", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        // Visual component functions
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setColor(const CColor &in)",                  WRAP_OBJ_LAST(SetColor1),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setColor(float, float, float, float)",        WRAP_OBJ_LAST(SetColor2),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const CColor & getColor() const",                  WRAP_OBJ_LAST(GetColor),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setDefaultColor()",                           WRAP_OBJ_LAST(SetDefaultColor),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const CColor & getDefaultColor() const",           WRAP_OBJ_LAST(GetDefaultColor),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setAlpha(float, bool allowToExceed = false)", WRAP_OBJ_LAST(SetAlpha),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "float getAlpha() const",                           WRAP_OBJ_LAST(GetAlpha),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setDefaultAlpha()",                           WRAP_OBJ_LAST(SetDefaultAlpha),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "float getDefaultAlpha() const",                    WRAP_OBJ_LAST(GetDefaultAlpha),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void createFontString(string &in)",                WRAP_OBJ_LAST(CreateFontString), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const string & getFontString() const",             WRAP_OBJ_LAST(GetFontString),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "bool isFontSprite() const",                        WRAP_OBJ_LAST(IsFontSprite),     asCALL_GENERIC) );

        // Physics component functions
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setPhysicsTransform(float, float, float angle = 0, bool resetVelocity = true)", WRAP_OBJ_LAST(SetTransform),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setLinearVelocity(float, float)",                                               WRAP_OBJ_LAST(SetLinearVelocity),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setAngularVelocity(float)",                                                     WRAP_OBJ_LAST(SetAngularVelocity),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void applyAngularImpulse(float, bool wake = false)",                                 WRAP_OBJ_LAST(ApplyAngularImpulse), asCALL_GENERIC) );

        // Sprite specific functions
        Throw( pEngine->RegisterObjectMethod("CSprite", "CSize getSize() const",                               WRAP_OBJ_LAST(GetSize),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "uint getFrameCount() const",                          WRAP_MFN(CSprite, getFrameCount),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setFrame(uint)",                                 WRAP_MFN(CSprite, setFrame),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "uint getCurrentFrame() const",                        WRAP_MFN(CSprite, getCurrentFrame), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "int getId() const",                                   WRAP_MFN(CSprite,   getId),         asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite", "bool prepare(string &in, bool forceUpdate = false)",  WRAP_MFN(CSprite,   prepare),       asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void setVisible(bool)",                               WRAP_OBJ_LAST(SetVisible),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "bool isVisible() const",                              WRAP_OBJ_LAST(IsVisible),    asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void setPos(CPoint &in)",                             WRAP_OBJ_LAST(SetPos1),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setPos(float x = 0, float y = 0, float z = 0)",  WRAP_OBJ_LAST(SetPos2),      asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void incPos(CPoint & in)",                            WRAP_OBJ_LAST(IncPos1),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void incPos(float x = 0, float y = 0, float z = 0)",  WRAP_OBJ_LAST(IncPos2),      asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getPos() const",                       WRAP_OBJ_LAST(GetPos),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getWorldPos() const",                  WRAP_OBJ_LAST(GetWorldPos),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getCenterPos() const",                 WRAP_OBJ_LAST(GetCenterPos), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void setRot(CPoint &in, bool convertToRadians = true)", WRAP_OBJ_LAST(SetRot1),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", WRAP_OBJ_LAST(SetRot2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void incRot(CPoint &in, bool convertToRadians = true)", WRAP_OBJ_LAST(IncRot1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", WRAP_OBJ_LAST(IncRot2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getRot() const",                         WRAP_OBJ_LAST(GetRot),    asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void setScale(CPoint & in)",                            WRAP_OBJ_LAST(SetScale1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void setScale(float x = 1, float y = 1, float z = 1)",  WRAP_OBJ_LAST(SetScale2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "void incScale(CPoint & in)",                            WRAP_OBJ_LAST(IncScale1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSprite", "void incScale(float x = 1, float y = 1, float z = 1)",  WRAP_OBJ_LAST(IncScale2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSprite", "const CPoint & getScale() const",                       WRAP_OBJ_LAST(GetScale),   asCALL_GENERIC) );
    }
}