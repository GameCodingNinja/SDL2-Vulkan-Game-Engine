
/************************************************************************
 *    FILE NAME:       scriptpoint.cpp
 *
 *    DESCRIPTION:     CPoint script object registration
 ************************************************************************/

// Physical component dependency
#include <script/scriptpoint.h>

// Game lib dependencies
#include <common/point.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptPoint
{
    /************************************************************************
     *    DESC:  Constructor
     ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CPoint<float>();
    }

    /************************************************************************
     *    DESC:  Copy Constructor
     ************************************************************************/
    void CopyConstructor(const CPoint<float> & other, void * pThisPointer)
    {
        new(pThisPointer) CPoint<float>(other);
    }

    void ConstructorFromThreeFloats(float x, float y, float z, void * pThisPointer)
    {
        new(pThisPointer) CPoint<float>(x, y, z);
    }

    void ConstructorFromTwoFloats(float x, float y, void * pThisPointer)
    {
        new(pThisPointer) CPoint<float>(x, y);
    }

    /************************************************************************
     *    DESC:  Destructor
     ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CPoint<float>*)pThisPointer)->~CPoint();
    }
    
    /************************************************************************
    *    DESC:  Wrapper function due to type being a template
    ************************************************************************/
    float GetLengthSquared2D1(CPoint<float> & point)
    {
        return point.getLengthSquared2D();
    }
    
    float GetLengthSquared2D2(CPoint<float> & in, CPoint<float> & point)
    {
        return point.getLengthSquared2D( in );
    }
    
    float GetLength2D1(CPoint<float> & point)
    {
        return point.getLength2D();
    }
    
    float GetLength2D2(CPoint<float> & in, CPoint<float> & point)
    {
        return point.getLength2D( in );
    }
    
    float GetLengthSquared1(CPoint<float> & point)
    {
        return point.getLengthSquared();
    }
    
    float GetLengthSquared2(CPoint<float> & in, CPoint<float> & point)
    {
        return point.getLengthSquared( in );
    }
    
    float GetLength1(CPoint<float> & point)
    {
        return point.getLength();
    }
    
    float GetLength2(CPoint<float> & in, CPoint<float> & point)
    {
        return point.getLength( in );
    }
    
    bool IsEquilEnough(CPoint<float> & in, float val, CPoint<float> & point)
    {
        return point.isEquilEnough( in, val );
    }

    float GetDotProduct(CPoint<float> & in, CPoint<float> & point)
    {
        return point.getDotProduct( in );
    }
    
    float GetDotProduct2D(CPoint<float> & in, CPoint<float> & point)
    {
        return point.getDotProduct2D( in );
    }
    
    CPoint<float> GetCrossProduct(CPoint<float> & in, CPoint<float> & point)
    {
        return point.getCrossProduct( in );
    }
    
    void Cap(float value, CPoint<float> & point)
    {
        point.cap(value);
    }


    /************************************************************************
     *    DESC:  Register the class with AngelScript
     ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CPoint", sizeof(CPoint<float>), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object operator overloads
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f()",                    asFUNCTION(Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(const CPoint & in)",   asFUNCTION(CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(ConstructorFromThreeFloats), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(float, float)",        asFUNCTION(ConstructorFromTwoFloats), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_DESTRUCT, "void f()",                     asFUNCTION(Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint & opAssign(CPoint & in)", asMETHODPR(CPoint<float>, operator =, (const CPoint<float> &), CPoint<float> &), asCALL_THISCALL) );

        // binary operators
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAdd ( CPoint & in )", asMETHODPR(CPoint<float>, operator +, (const CPoint<float> &) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSub ( CPoint & in )", asMETHODPR(CPoint<float>, operator -, (const CPoint<float> &) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMul ( CPoint & in )", asMETHODPR(CPoint<float>, operator *, (const CPoint<float> &) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDiv ( CPoint & in )", asMETHODPR(CPoint<float>, operator /, (const CPoint<float> &) const, CPoint<float>), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAdd ( float )", asMETHODPR(CPoint<float>, operator +, (float) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSub ( float )", asMETHODPR(CPoint<float>, operator -, (float) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMul ( float )", asMETHODPR(CPoint<float>, operator *, (float) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDiv ( float )", asMETHODPR(CPoint<float>, operator /, (float) const, CPoint<float>), asCALL_THISCALL) );

        // compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAddAssign ( CPoint & in )", asMETHODPR(CPoint<float>, operator +=, (const CPoint<float> &), CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSubAssign ( CPoint & in )", asMETHODPR(CPoint<float>, operator -=, (const CPoint<float> &), CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMulAssign ( CPoint & in )", asMETHODPR(CPoint<float>, operator *=, (const CPoint<float> &), CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDivAssign ( CPoint & in )", asMETHODPR(CPoint<float>, operator /=, (const CPoint<float> &), CPoint<float>), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAddAssign ( float )", asMETHODPR(CPoint<float>, operator +=, (float) , CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSubAssign ( float )", asMETHODPR(CPoint<float>, operator -=, (float) , CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMulAssign ( float )", asMETHODPR(CPoint<float>, operator *=, (float) , CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDivAssign ( float )", asMETHODPR(CPoint<float>, operator /=, (float) , CPoint<float>), asCALL_THISCALL) );

        // Register property
        Throw( pEngine->RegisterObjectProperty("CPoint", "float x", asOFFSET(CPoint<float>, x)) );
        Throw( pEngine->RegisterObjectProperty("CPoint", "float y", asOFFSET(CPoint<float>, y)) );
        Throw( pEngine->RegisterObjectProperty("CPoint", "float z", asOFFSET(CPoint<float>, z)) );

        // Class members
        Throw( pEngine->RegisterObjectMethod("CPoint", "void clearX()",       asMETHOD(CPoint<float>, clearX), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void clearY()",       asMETHOD(CPoint<float>, clearY), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void clearZ()",       asMETHOD(CPoint<float>, clearZ), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void clear()",        asMETHOD(CPoint<float>, clear), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isEmpty()",      asMETHOD(CPoint<float>, isEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isXEmpty()",     asMETHOD(CPoint<float>, isXEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isYEmpty()",     asMETHOD(CPoint<float>, isYEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isZEmpty()",     asMETHOD(CPoint<float>, isZEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void invert()",       asMETHOD(CPoint<float>, invert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void invertX()",      asMETHOD(CPoint<float>, invert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void invertY()",      asMETHOD(CPoint<float>, invert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void invertZ()",      asMETHOD(CPoint<float>, invert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint getInvert() const",  asMETHOD(CPoint<float>, getInvert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void normalize()",    asMETHOD(CPoint<float>, normalize), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void normalize2D()",  asMETHOD(CPoint<float>, normalize2D), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLengthSquared2D() const",            asFUNCTION(GetLengthSquared2D1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLengthSquared2D(CPoint & in) const", asFUNCTION(GetLengthSquared2D2), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLengthSquared() const",              asFUNCTION(GetLengthSquared1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLengthSquared(CPoint & in) const",   asFUNCTION(GetLengthSquared2), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLength2D() const",                   asFUNCTION(GetLength2D1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLength2D(CPoint & in) const",        asFUNCTION(GetLength2D2), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLength() const",                     asFUNCTION(GetLength1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLength(CPoint & in) const",          asFUNCTION(GetLength2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isEquilEnough(CPoint & in, float val)", asFUNCTION(IsEquilEnough), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getDotProduct(CPoint & in) const",           asFUNCTION(GetDotProduct), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getDotProduct2D(CPoint & in) const",         asFUNCTION(GetDotProduct2D), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint getCrossProduct(CPoint & in) const",        asFUNCTION(GetCrossProduct), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void cap(float)",                            asFUNCTION(Cap), asCALL_CDECL_OBJLAST) );
    }
}
