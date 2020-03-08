
/************************************************************************
 *    FILE NAME:       scriptpoint.cpp
 *
 *    DESCRIPTION:     CSize script object registration
 ************************************************************************/

// Physical component dependency
#include <script/scriptsize.h>

// Game lib dependencies
#include <common/size.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptSize
{
    /************************************************************************
     *    DESC:  Constructor
     ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CSize<float>();
    }

    /************************************************************************
     *    DESC:  Copy Constructor
     ************************************************************************/
    void CopyConstructor(const CSize<float> & other, void * pThisPointer)
    {
        new(pThisPointer) CSize<float>(other);
    }

    static void ConstructorFromTwoFloats(float w, float h, void * pThisPointer)
    {
        new(pThisPointer) CSize<float>(w, h);
    }

    /************************************************************************
     *    DESC:  Destructor
     ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CSize<float>*)pThisPointer)->~CSize();
    }

    void OpAssignCSize(asIScriptGeneric *gen)
    {
        CSize<float> * a = static_cast<CSize<float> *>(gen->GetArgObject(0));
        CSize<float> * self = static_cast<CSize<float> *>(gen->GetObject());
        *self = *a;
        gen->SetReturnAddress(self);
    }

    /************************************************************************
     *    DESC:  Register the class with AngelScript
     ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CSize", sizeof(CSize<float>), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<CSize<float>>() | asOBJ_APP_CLASS_ALLFLOATS ) );

        // Register the object operator overloads
        Throw( pEngine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT, "void f()",                 WRAP_OBJ_LAST(Constructor),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT, "void f(const CSize & in)", WRAP_OBJ_LAST(CopyConstructor),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT, "void f(float, float)",     WRAP_OBJ_LAST(ConstructorFromTwoFloats), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CSize", asBEHAVE_DESTRUCT,  "void f()",                 WRAP_OBJ_LAST(Destructor),               asCALL_GENERIC) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize & opAssign(const CSize & in)", asFUNCTION(OpAssignCSize), asCALL_GENERIC) );

        // binary operators
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opAdd ( const CSize & in )", WRAP_MFN_PR(CSize<float>, operator +, (const CSize<float> &) const, CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opSub ( const CSize & in )", WRAP_MFN_PR(CSize<float>, operator -, (const CSize<float> &) const, CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opMul ( const CSize & in )", WRAP_MFN_PR(CSize<float>, operator *, (const CSize<float> &) const, CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opDiv ( const CSize & in )", WRAP_MFN_PR(CSize<float>, operator /, (const CSize<float> &) const, CSize<float>), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opAdd ( float )", WRAP_MFN_PR(CSize<float>, operator +, (float) const, CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opSub ( float )", WRAP_MFN_PR(CSize<float>, operator -, (float) const, CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opMul ( float )", WRAP_MFN_PR(CSize<float>, operator *, (float) const, CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opDiv ( float )", WRAP_MFN_PR(CSize<float>, operator /, (float) const, CSize<float>), asCALL_GENERIC) );

        // compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opAddAssign ( const CSize & in )", WRAP_MFN_PR(CSize<float>, operator +=, (const CSize<float> &), CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opSubAssign ( const CSize & in )", WRAP_MFN_PR(CSize<float>, operator -=, (const CSize<float> &), CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opMulAssign ( const CSize & in )", WRAP_MFN_PR(CSize<float>, operator *=, (const CSize<float> &), CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opDivAssign ( const CSize & in )", WRAP_MFN_PR(CSize<float>, operator /=, (const CSize<float> &), CSize<float>), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opAddAssign ( float )", WRAP_MFN_PR(CSize<float>, operator +=, (float) , CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opSubAssign ( float )", WRAP_MFN_PR(CSize<float>, operator -=, (float) , CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opMulAssign ( float )", WRAP_MFN_PR(CSize<float>, operator *=, (float) , CSize<float>), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opDivAssign ( float )", WRAP_MFN_PR(CSize<float>, operator /=, (float) , CSize<float>), asCALL_GENERIC) );

        // Register property
        Throw( pEngine->RegisterObjectProperty("CSize", "float w", asOFFSET(CSize<float>, w)) );
        Throw( pEngine->RegisterObjectProperty("CSize", "float h", asOFFSET(CSize<float>, h)) );

        // Class members
        Throw( pEngine->RegisterObjectMethod("CSize", "void clear()",          WRAP_MFN(CSize<float>, clear),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "void round()",          WRAP_MFN(CSize<float>, round),    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "bool isEmpty() const",  WRAP_MFN(CSize<float>, isEmpty),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "bool isWEmpty() const", WRAP_MFN(CSize<float>, isWEmpty), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CSize", "bool isHEmpty() const", WRAP_MFN(CSize<float>, isHEmpty), asCALL_GENERIC) );
    }
}
