
/************************************************************************
*    FILE NAME:       scriptcolor.cpp
*
*    DESCRIPTION:     CColor script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptcolor.h>

// Game lib dependencies
#include <common/color.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptColor
{
    /************************************************************************
    *    DESC:  Constructor
    ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CColor();
    }

    /************************************************************************
    *    DESC:  Copy Constructor
    ************************************************************************/
    void CopyConstructor(const CColor & other, void * pThisPointer)
    {
        new(pThisPointer) CColor(other);
    }

    void ConstructorFromFloats(float r, float g, float b, float a, void * pThisPointer)
    {
        new(pThisPointer) CColor(r, g, b, a);
    }

    /************************************************************************
    *    DESC:  Destructor
    ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CColor*)pThisPointer)->~CColor();
    }

    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CColor", sizeof(CColor), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<CColor>() | asOBJ_APP_CLASS_ALLFLOATS ) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f()",                           WRAP_OBJ_LAST(Constructor),           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f(const CColor & in)",          WRAP_OBJ_LAST(CopyConstructor),       asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", WRAP_OBJ_LAST(ConstructorFromFloats), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CColor", asBEHAVE_DESTRUCT,  "void f()",                           WRAP_OBJ_LAST(Destructor),            asCALL_GENERIC) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor & opAssign(const CColor & in)", WRAP_MFN_PR(CColor, operator =, (const CColor &), CColor &), asCALL_GENERIC) );

        // binary operators
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opAdd ( const CColor & in )", WRAP_MFN_PR(CColor, operator +, (const CColor &) const, CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opSub ( const CColor & in )", WRAP_MFN_PR(CColor, operator -, (const CColor &) const, CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opMul ( const CColor & in )", WRAP_MFN_PR(CColor, operator *, (const CColor &) const, CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opDiv ( const CColor & in )", WRAP_MFN_PR(CColor, operator /, (const CColor &) const, CColor), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opAdd ( float )", WRAP_MFN_PR(CColor, operator +, (float) const, CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opSub ( float )", WRAP_MFN_PR(CColor, operator -, (float) const, CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opMul ( float )", WRAP_MFN_PR(CColor, operator *, (float) const, CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opDiv ( float )", WRAP_MFN_PR(CColor, operator /, (float) const, CColor), asCALL_GENERIC) );

        // compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opAddAssign ( const CColor & in )", WRAP_MFN_PR(CColor, operator +=, (const CColor &), CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opSubAssign ( const CColor & in )", WRAP_MFN_PR(CColor, operator -=, (const CColor &), CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opMulAssign ( const CColor & in )", WRAP_MFN_PR(CColor, operator *=, (const CColor &), CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opDivAssign ( const CColor & in )", WRAP_MFN_PR(CColor, operator /=, (const CColor &), CColor), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opAddAssign ( float )", WRAP_MFN_PR(CColor, operator +=, (float), CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opSubAssign ( float )", WRAP_MFN_PR(CColor, operator -=, (float), CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opMulAssign ( float )", WRAP_MFN_PR(CColor, operator *=, (float), CColor), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opDivAssign ( float )", WRAP_MFN_PR(CColor, operator /=, (float), CColor), asCALL_GENERIC) );

	    // Register property
        Throw( pEngine->RegisterObjectProperty("CColor", "float r", asOFFSET(CColor, r)) );
        Throw( pEngine->RegisterObjectProperty("CColor", "float g", asOFFSET(CColor, g)) );
        Throw( pEngine->RegisterObjectProperty("CColor", "float b", asOFFSET(CColor, b)) );
        Throw( pEngine->RegisterObjectProperty("CColor", "float a", asOFFSET(CColor, a)) );

        // Class members
        Throw( pEngine->RegisterObjectMethod("CColor", "void set( float r, float g, float b, float a )",       WRAP_MFN(CColor, set),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CColor", "void transformHSV( float hue, float sat, float val )", WRAP_MFN(CColor, transformHSV), asCALL_GENERIC) );
	    Throw( pEngine->RegisterObjectMethod("CColor", "void convert()",                                       WRAP_MFN(CColor, convert),      asCALL_GENERIC ) );
    }
}
