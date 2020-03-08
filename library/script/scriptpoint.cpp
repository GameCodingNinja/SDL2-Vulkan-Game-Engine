
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
#include <autowrapper/aswrappedcall.h>

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
    void OpAssignCPoint(asIScriptGeneric *gen)
    {
        CPoint<float> * a = static_cast<CPoint<float> *>(gen->GetArgObject(0));
        CPoint<float> * self = static_cast<CPoint<float> *>(gen->GetObject());
        *self = *a;
        gen->SetReturnAddress(self);
    }

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
        Throw( pEngine->RegisterObjectType("CPoint", sizeof(CPoint<float>), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<CPoint<float>>() | asOBJ_APP_CLASS_ALLFLOATS ) );

        // Register the object operator overloads
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f()",                    WRAP_OBJ_LAST(Constructor),                asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(const CPoint & in)",   WRAP_OBJ_LAST(CopyConstructor),            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(float, float, float)", WRAP_OBJ_LAST(ConstructorFromThreeFloats), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(float, float)",        WRAP_OBJ_LAST(ConstructorFromTwoFloats),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_DESTRUCT,  "void f()",                    WRAP_OBJ_LAST(Destructor),                 asCALL_GENERIC) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint & opAssign(const CPoint & in)", asFUNCTION(OpAssignCPoint), asCALL_GENERIC) );

        // binary operators
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAdd ( CPoint & in )", WRAP_MFN_PR(CPoint<float>, operator +, (const CPoint<float> &) const, CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSub ( CPoint & in )", WRAP_MFN_PR(CPoint<float>, operator -, (const CPoint<float> &) const, CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMul ( CPoint & in )", WRAP_MFN_PR(CPoint<float>, operator *, (const CPoint<float> &) const, CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDiv ( CPoint & in )", WRAP_MFN_PR(CPoint<float>, operator /, (const CPoint<float> &) const, CPoint<float>), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAdd ( float )", WRAP_MFN_PR(CPoint<float>, operator +, (float) const, CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSub ( float )", WRAP_MFN_PR(CPoint<float>, operator -, (float) const, CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMul ( float )", WRAP_MFN_PR(CPoint<float>, operator *, (float) const, CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDiv ( float )", WRAP_MFN_PR(CPoint<float>, operator /, (float) const, CPoint<float>), asCALL_GENERIC) );

        // compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAddAssign ( CPoint & in )", WRAP_MFN_PR(CPoint<float>, operator +=, (const CPoint<float> &), CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSubAssign ( CPoint & in )", WRAP_MFN_PR(CPoint<float>, operator -=, (const CPoint<float> &), CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMulAssign ( CPoint & in )", WRAP_MFN_PR(CPoint<float>, operator *=, (const CPoint<float> &), CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDivAssign ( CPoint & in )", WRAP_MFN_PR(CPoint<float>, operator /=, (const CPoint<float> &), CPoint<float>), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAddAssign ( float )", WRAP_MFN_PR(CPoint<float>, operator +=, (float) , CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSubAssign ( float )", WRAP_MFN_PR(CPoint<float>, operator -=, (float) , CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMulAssign ( float )", WRAP_MFN_PR(CPoint<float>, operator *=, (float) , CPoint<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDivAssign ( float )", WRAP_MFN_PR(CPoint<float>, operator /=, (float) , CPoint<float>), asCALL_GENERIC) );

        // Register property
        Throw( pEngine->RegisterObjectProperty("CPoint", "float x", asOFFSET(CPoint<float>, x)) );
        Throw( pEngine->RegisterObjectProperty("CPoint", "float y", asOFFSET(CPoint<float>, y)) );
        Throw( pEngine->RegisterObjectProperty("CPoint", "float z", asOFFSET(CPoint<float>, z)) );

        // Class members
        Throw( pEngine->RegisterObjectMethod("CPoint", "void clearX()",            WRAP_MFN(CPoint<float>, clearX),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void clearY()",            WRAP_MFN(CPoint<float>, clearY),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void clearZ()",            WRAP_MFN(CPoint<float>, clearZ),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void clear()",             WRAP_MFN(CPoint<float>, clear),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isEmpty()",           WRAP_MFN(CPoint<float>, isEmpty),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isXEmpty()",          WRAP_MFN(CPoint<float>, isXEmpty),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isYEmpty()",          WRAP_MFN(CPoint<float>, isYEmpty),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isZEmpty()",          WRAP_MFN(CPoint<float>, isZEmpty),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void invert()",            WRAP_MFN(CPoint<float>, invert),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void invertX()",           WRAP_MFN(CPoint<float>, invert),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void invertY()",           WRAP_MFN(CPoint<float>, invert),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void invertZ()",           WRAP_MFN(CPoint<float>, invert),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint getInvert() const", WRAP_MFN(CPoint<float>, getInvert),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void normalize()",         WRAP_MFN(CPoint<float>, normalize),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void normalize2D()",       WRAP_MFN(CPoint<float>, normalize2D), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLengthSquared2D() const",            WRAP_OBJ_LAST(GetLengthSquared2D1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLengthSquared2D(CPoint & in) const", WRAP_OBJ_LAST(GetLengthSquared2D2), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLengthSquared() const",              WRAP_OBJ_LAST(GetLengthSquared1),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLengthSquared(CPoint & in) const",   WRAP_OBJ_LAST(GetLengthSquared2),   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLength2D() const",                   WRAP_OBJ_LAST(GetLength2D1),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLength2D(CPoint & in) const",        WRAP_OBJ_LAST(GetLength2D2),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLength() const",                     WRAP_OBJ_LAST(GetLength1),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getLength(CPoint & in) const",          WRAP_OBJ_LAST(GetLength2),          asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool isEquilEnough(CPoint & in, float val)", WRAP_OBJ_LAST(IsEquilEnough),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getDotProduct(CPoint & in) const",     WRAP_OBJ_LAST(GetDotProduct),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float getDotProduct2D(CPoint & in) const",   WRAP_OBJ_LAST(GetDotProduct2D),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint getCrossProduct(CPoint & in) const",  WRAP_OBJ_LAST(GetCrossProduct),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void cap(float)",                            WRAP_OBJ_LAST(Cap),                  asCALL_GENERIC) );
    }
}
