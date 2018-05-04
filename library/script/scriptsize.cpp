
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

    void ConstructorFromTwoFloats(float w, float h, void * pThisPointer)
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

    /************************************************************************
     *    DESC:  Register the class with AngelScript
     ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CSize", sizeof(CSize<float>), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object operator overloads
        Throw( pEngine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT, "void f()",                    asFUNCTION(Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT, "void f(const CSize & in)",    asFUNCTION(CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT, "void f(float, float)",        asFUNCTION(ConstructorFromTwoFloats), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSize", asBEHAVE_DESTRUCT,  "void f()",                    asFUNCTION(Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize & opAssign(const CSize & in)", asMETHODPR(CSize<float>, operator =, (const CSize<float> &), CSize<float> &), asCALL_THISCALL) );

        // binary operators
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opAdd ( const CSize & in )", asMETHODPR(CSize<float>, operator +, (const CSize<float> &) const, CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opSub ( const CSize & in )", asMETHODPR(CSize<float>, operator -, (const CSize<float> &) const, CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opMul ( const CSize & in )", asMETHODPR(CSize<float>, operator *, (const CSize<float> &) const, CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opDiv ( const CSize & in )", asMETHODPR(CSize<float>, operator /, (const CSize<float> &) const, CSize<float>), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opAdd ( float )", asMETHODPR(CSize<float>, operator +, (float) const, CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opSub ( float )", asMETHODPR(CSize<float>, operator -, (float) const, CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opMul ( float )", asMETHODPR(CSize<float>, operator *, (float) const, CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opDiv ( float )", asMETHODPR(CSize<float>, operator /, (float) const, CSize<float>), asCALL_THISCALL) );

        // compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opAddAssign ( const CSize & in )", asMETHODPR(CSize<float>, operator +=, (const CSize<float> &), CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opSubAssign ( const CSize & in )", asMETHODPR(CSize<float>, operator -=, (const CSize<float> &), CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opMulAssign ( const CSize & in )", asMETHODPR(CSize<float>, operator *=, (const CSize<float> &), CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opDivAssign ( const CSize & in )", asMETHODPR(CSize<float>, operator /=, (const CSize<float> &), CSize<float>), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opAddAssign ( float )", asMETHODPR(CSize<float>, operator +=, (float) , CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opSubAssign ( float )", asMETHODPR(CSize<float>, operator -=, (float) , CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opMulAssign ( float )", asMETHODPR(CSize<float>, operator *=, (float) , CSize<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "CSize opDivAssign ( float )", asMETHODPR(CSize<float>, operator /=, (float) , CSize<float>), asCALL_THISCALL) );

        // Register property
        Throw( pEngine->RegisterObjectProperty("CSize", "float w", asOFFSET(CSize<float>, w)) );
        Throw( pEngine->RegisterObjectProperty("CSize", "float h", asOFFSET(CSize<float>, h)) );

        // Class members
        Throw( pEngine->RegisterObjectMethod("CSize", "void clear()",        asMETHOD(CSize<float>, clear), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "void round()",        asMETHOD(CSize<float>, round), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "bool isEmpty()",      asMETHOD(CSize<float>, isEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "bool isWEmpty()",     asMETHOD(CSize<float>, isWEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSize", "bool isHEmpty()",     asMETHOD(CSize<float>, isHEmpty), asCALL_THISCALL) );
    }
}
