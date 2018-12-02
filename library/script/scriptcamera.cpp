
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
    
    
    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CCamera", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setPos(CPoint &in)",                             asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incPos(CPoint & in)",                            asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "CPoint getPos() const",                                asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CCamera", "void setRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "const CPoint & getRot() const",                         asFUNCTION(GetRot),               asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CCamera", "const CPoint & getScale() const",     asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void transform()",  asFUNCTION(Transform), asCALL_CDECL_OBJLAST) );
    }
}