
/************************************************************************
*    FILE NAME:       scriptcamera.cpp
*
*    DESCRIPTION:     Sprite2D script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptcamera.h>

// Game lib dependencies
#include <common/camera.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptCamera
{
    CPoint<float> point;
    
    /************************************************************************
    *    DESC:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetPos1(const CPoint<float> & pos, CCamera & camera)
    {
        camera.setPos(pos);
    }
    
    void SetPos2(float x, float y, float z, CCamera & camera)
    {
        camera.setPos(x,y,z);
    }
    
    void IncPos1(const CPoint<float> & pos, CCamera & camera)
    {
        camera.incPos(pos);
    }
    
    void IncPos2(float x, float y, float z, CCamera & camera)
    {
        camera.incPos(x,y,z);
    }
    
    CPoint<float> GetPos(CCamera & camera)
    {
        point = camera.getPos();
        return point;
    }
    
    void SetRot1(const CPoint<float> & rot, bool convertToRadians, CCamera & camera)
    {
        camera.setRot(rot, convertToRadians);
    }
    
    void SetRot2(float x, float y, float z, bool convertToRadians, CCamera & camera)
    {
        camera.setRot(x,y,z, convertToRadians);
    }
    
    void IncRot1(const CPoint<float> & rot, bool convertToRadians, CCamera & camera)
    {
        camera.incRot(rot, convertToRadians);
    }
    
    void IncRot2(float x, float y, float z, bool convertToRadians, CCamera & camera)
    {
        camera.incRot(x,y,z, convertToRadians);
    }
    
    const CPoint<float> & GetRot(CCamera & camera)
    {
        return camera.getRot();
    }
    
    void SetScale1(const CPoint<float> & rot, CCamera & camera)
    {
        camera.setScale(rot);
    }
    
    void SetScale2(float x, float y, float z, CCamera & camera)
    {
        camera.setScale(x,y,z);
    }
    
    void IncScale1(const CPoint<float> & rot, CCamera & camera)
    {
        camera.incScale(rot);
    }
    
    void IncScale2(float x, float y, float z, CCamera & camera)
    {
        camera.incScale(x,y,z);
    }

    const CPoint<float> & GetScale(CCamera & camera)
    {
        return camera.getScale();
    }
    
    void Transform(CCamera & camera)
    {
        camera.transform();
    }

    CPoint<float> ToOrthoCoord1(const CPoint<float> & pos, CCamera & camera)
    {
        return camera.toOrthoCoord(pos);
    }

    CPoint<float> ToOrthoCoord2(const int x, const int y, CCamera & camera)
    {
        return camera.toOrthoCoord(CPoint<float>(x, y));
    }
    
    
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CCamera", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setPos(CPoint &in)",                             WRAP_OBJ_LAST(SetPos1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setPos(float x = 0, float y = 0, float z = 0)",  WRAP_OBJ_LAST(SetPos2), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incPos(CPoint & in)",                            WRAP_OBJ_LAST(IncPos1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incPos(float x = 0, float y = 0, float z = 0)",  WRAP_OBJ_LAST(IncPos2), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "CPoint getPos() const",                                WRAP_OBJ_LAST(GetPos),  asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CCamera", "void setRot(CPoint &in, bool convertToRadians = true)", WRAP_OBJ_LAST(SetRot1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", WRAP_OBJ_LAST(SetRot2), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incRot(CPoint &in, bool convertToRadians = true)", WRAP_OBJ_LAST(IncRot1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", WRAP_OBJ_LAST(IncRot2), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "const CPoint & getRot() const",                         WRAP_OBJ_LAST(GetRot),    asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setScale(CPoint & in)",                            WRAP_OBJ_LAST(SetScale1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setScale(float x = 1, float y = 1, float z = 1)",  WRAP_OBJ_LAST(SetScale2), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incScale(CPoint & in)",                            WRAP_OBJ_LAST(IncScale1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incScale(float x = 1, float y = 1, float z = 1)",  WRAP_OBJ_LAST(IncScale2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CCamera", "const CPoint & getScale() const",                       WRAP_OBJ_LAST(GetScale),   asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CCamera", "CPoint toOrthoCoord(const CPoint & in)",                WRAP_OBJ_LAST(ToOrthoCoord1),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "CPoint toOrthoCoord(const int, const int)",             WRAP_OBJ_LAST(ToOrthoCoord2),   asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void transform()",                                      WRAP_OBJ_LAST(Transform), asCALL_GENERIC) );
    }
}