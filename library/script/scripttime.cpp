
/************************************************************************
*    FILE NAME:       scripttime.h
*
*    DESCRIPTION:     Time script object registration
************************************************************************/

// Physical component dependency
#include <script/scripttime.h>

// Game lib dependencies
#include <utilities/timepoint.h>
#include <utilities/timeduration.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// Standard lib dependencies
#include <sstream>

// AngelScript lib dependencies
#include <angelscript.h>
#include <autowrapper/aswrappedcall.h>

namespace NScriptTime
{
    /************************************************************************
     *    DESC:  Constructor
     ************************************************************************/
    void ConstructorTP(void * thisPointer)
    {
        new(thisPointer) CTimePoint();
    }

    void ConstructorTD(void * thisPointer)
    {
        new(thisPointer) CTimeDuration();
    }

    /************************************************************************
     *    DESC:  Copy Constructor
     ************************************************************************/
    void CopyConstructorTP(const CTimePoint & other, void * pThisPointer)
    {
        new(pThisPointer) CTimePoint(other);
    }

    void CopyConstructorTDtoTP(const CTimeDuration & other, void * pThisPointer)
    {
        new(pThisPointer) CTimePoint(other);
    }

    void CopyConstructorTPInt64(int64_t other, void * pThisPointer)
    {
        new(pThisPointer) CTimePoint(other);
    }

    void CopyConstructorTD(const CTimeDuration & other, void * pThisPointer)
    {
        new(pThisPointer) CTimeDuration(other);
    }

    void CopyConstructorTDInt64(int64_t other, void * pThisPointer)
    {
        new(pThisPointer) CTimeDuration(other);
    }

    void CopyConstructorTDSet(int64_t h, int64_t m, int64_t s, int64_t mil, void * pThisPointer)
    {
        new(pThisPointer) CTimeDuration(h,m,s,mil);
    }

    /************************************************************************
     *    DESC:  Destructor
     ************************************************************************/
    void DestructorTP(void * pThisPointer)
    {
        ((CTimePoint*)pThisPointer)->~CTimePoint();
    }

    void DestructorTD(void * pThisPointer)
    {
        ((CTimeDuration*)pThisPointer)->~CTimeDuration();
    }

    /************************************************************************
     *    DESC:  Compair operators
     ************************************************************************/
    int opCmpTP(const CTimePoint &a, const CTimePoint &b)
    {
        if (a < b) return -1;
        if (a > b) return 1;
        if (a == b) return 0;
        return 1;
    }

    int opCmpTD(const CTimeDuration &a, const CTimeDuration &b)
    {
        if (a < b) return -1;
        if (a > b) return 1;
        if (a == b) return 0;
        return 1;
    }

    /************************************************************************
    *    DESC:  Wrapper functions
    ************************************************************************/
    void Now1(CTimePoint & tp)
    {
        tp.now();
    }

    void Now2(const CTimeDuration & td, CTimePoint & tp)
    {
        tp.now(td);
    }

    void SetTD(asIScriptGeneric * pScriptGen)
    {
        CTimeDuration * pTD = static_cast<CTimeDuration *>(pScriptGen->GetObject());

        pTD->set(
            pScriptGen->GetArgQWord(0),
            pScriptGen->GetArgQWord(1),
            pScriptGen->GetArgQWord(2),
            pScriptGen->GetArgQWord(3) );
    }

    /************************************************************************
    *    DESC:  Global functions
    ************************************************************************/
    void FormatTimeDuration(asIScriptGeneric * pScriptGen)
    {
        const auto duration = reinterpret_cast<CTimeDuration*>(pScriptGen->GetArgAddress(0))->getDuration();
        const int format = pScriptGen->GetArgDWord(1);

        std::stringstream ss;

        // Is hour set
        if( format & 0b1000 )
        {
            auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
            ss << std::setfill('0') << std::setw(2) << hours;
        }

        // Is minute set
        if( format & 0b0100 )
        {
            if( !ss.str().empty() )
                ss << ":";

            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
            ss << std::setfill('0') << std::setw(2) << (minutes % 60);
        }

        // Is second set
        if( format & 0b0010 )
        {
            if( !ss.str().empty() )
                ss << ":";

            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
            ss << std::setfill('0') << std::setw(2) << (seconds % 60);
        }

        // Is millisecond set
        if( format & 0b0001 )
        {
            if( !ss.str().empty() )
                ss << ":";
                
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            ss << std::setfill('0') << std::setw(2) << ((milliseconds/10) % 100);
        }

        std::string ret_val = ss.str();
	    pScriptGen->SetReturnObject(&ret_val);
    }

    void GetDurationHours(asIScriptGeneric * pScriptGen)
    {
        int64_t value = pScriptGen->GetArgQWord(0);
        auto duration = std::chrono::hours(value);
        int64_t result = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        pScriptGen->SetReturnQWord(result);
    }

    void GetDurationMinutes(asIScriptGeneric * pScriptGen)
    {
        int64_t value = pScriptGen->GetArgQWord(0);
        auto duration = std::chrono::minutes(value);
        int64_t result = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        pScriptGen->SetReturnQWord(result);
    }

    void GetDurationSeconds(asIScriptGeneric * pScriptGen)
    {
        int64_t value = pScriptGen->GetArgQWord(0);
        auto duration = std::chrono::seconds(value);
        int64_t result = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        pScriptGen->SetReturnQWord(result);
    }

    void GetDurationMilliseconds(asIScriptGeneric * pScriptGen)
    {
        int64_t value = pScriptGen->GetArgQWord(0);
        auto duration = std::chrono::milliseconds(value);
        int64_t result = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        pScriptGen->SetReturnQWord(result);
    }

    void GetDurationMicroseconds(asIScriptGeneric * pScriptGen)
    {
        int64_t value = pScriptGen->GetArgQWord(0);
        auto duration = std::chrono::microseconds(value);
        int64_t result = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        pScriptGen->SetReturnQWord(result);
    }

    /************************************************************************
     *    DESC:  Register the class with AngelScript
     ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CTimePoint",    sizeof(CTimePoint),    asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<CTimePoint>() ) );
        Throw( pEngine->RegisterObjectType("CTimeDuration", sizeof(CTimeDuration), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<CTimePoint>() ) );

        // Register the object operator overloads
        Throw( pEngine->RegisterObjectBehaviour("CTimePoint", asBEHAVE_CONSTRUCT, "void f()",                      WRAP_OBJ_LAST(ConstructorTP),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CTimePoint", asBEHAVE_CONSTRUCT, "void f(const CTimePoint & in)", WRAP_OBJ_LAST(CopyConstructorTP), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CTimePoint", asBEHAVE_CONSTRUCT, "void f(const CTimeDuration & in)", WRAP_OBJ_LAST(CopyConstructorTDtoTP), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectBehaviour("CTimePoint", asBEHAVE_CONSTRUCT, "void f(int64)", WRAP_OBJ_LAST(CopyConstructorTPInt64), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectBehaviour("CTimeDuration", asBEHAVE_CONSTRUCT, "void f()",                         WRAP_OBJ_LAST(ConstructorTD),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CTimeDuration", asBEHAVE_CONSTRUCT, "void f(const CTimeDuration & in)", WRAP_OBJ_LAST(CopyConstructorTD), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CTimeDuration", asBEHAVE_CONSTRUCT, "void f(int64)",                 WRAP_OBJ_LAST(CopyConstructorTDInt64), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectBehaviour("CTimeDuration", asBEHAVE_CONSTRUCT, "void f(int64,int64,int64,int64)", WRAP_OBJ_LAST(CopyConstructorTDSet), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectBehaviour("CTimePoint", asBEHAVE_DESTRUCT,  "void f()",                         WRAP_OBJ_LAST(DestructorTP),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectBehaviour("CTimeDuration", asBEHAVE_DESTRUCT,  "void f()",                      WRAP_OBJ_LAST(DestructorTP),  asCALL_GENERIC) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "CTimePoint & opAssign(const CTimePoint & in)", WRAP_MFN_PR(CTimePoint, operator =, (const CTimePoint &), CTimePoint &), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "CTimeDuration & opAssign(const CTimeDuration & in)", WRAP_MFN_PR(CTimeDuration, operator =, (const CTimeDuration &), CTimeDuration &), asCALL_GENERIC) );

        // CTimePoint binary operators
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "CTimePoint opAdd ( const CTimeDuration & in )", WRAP_MFN_PR(CTimePoint, operator +, (const CTimeDuration &) const, CTimePoint), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "CTimePoint opSub ( const CTimeDuration & in )", WRAP_MFN_PR(CTimePoint, operator -, (const CTimeDuration &) const, CTimePoint), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "CTimeDuration opSub ( const CTimePoint & in )", WRAP_MFN_PR(CTimePoint, operator -, (const CTimePoint &) const, CTimeDuration), asCALL_GENERIC) );

        // CTimeDuration binary operators
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "CTimeDuration opAdd ( const CTimeDuration & in )", WRAP_MFN_PR(CTimeDuration, operator +, (const CTimeDuration &) const, CTimeDuration), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "CTimeDuration opSub ( const CTimeDuration & in )", WRAP_MFN_PR(CTimeDuration, operator -, (const CTimeDuration &) const, CTimeDuration), asCALL_GENERIC) );

        // CTimePoint compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "CTimePoint opAddAssign ( const CTimeDuration & in )", WRAP_MFN_PR(CTimePoint, operator +=, (const CTimeDuration &), CTimePoint), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "CTimePoint opSubAssign ( const CTimeDuration & in )", WRAP_MFN_PR(CTimePoint, operator -=, (const CTimeDuration &), CTimePoint), asCALL_GENERIC) );

        // CTimeDuration compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "CTimeDuration opAddAssign ( const CTimeDuration & in )", WRAP_MFN_PR(CTimeDuration, operator +=, (const CTimeDuration &), CTimeDuration), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "CTimeDuration opSubAssign ( const CTimeDuration & in )", WRAP_MFN_PR(CTimeDuration, operator -=, (const CTimeDuration &), CTimeDuration), asCALL_GENERIC) );

        // CTimePoint compair operators
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "bool opEquals ( const CTimePoint & in )", WRAP_MFN_PR(CTimePoint, operator ==, (const CTimePoint &) const, bool), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "int opCmp ( const CTimePoint & in )",    asFUNCTION(opCmpTP), asCALL_CDECL_OBJFIRST ) );

        // CTimeDuration compair operators
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "bool opEquals ( const CTimeDuration & in )", WRAP_MFN_PR(CTimeDuration, operator ==, (const CTimeDuration &) const, bool), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "int opCmp ( const CTimeDuration & in )",    asFUNCTION(opCmpTD), asCALL_CDECL_OBJFIRST ) );

        // CTimePoint Class members
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "void now()",                         WRAP_OBJ_LAST(Now1), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimePoint", "void now(const CTimeDuration & in)", WRAP_OBJ_LAST(Now2), asCALL_GENERIC) );

        // CTimeDuration Class members
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void set(int64,int64,int64,int64,int64 m = 0,int64 n = 0)",  asFUNCTION(SetTD),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void setNanoseconds(int64)",  WRAP_MFN(CTimeDuration, setNanoseconds),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void setMicroseconds(int64)", WRAP_MFN(CTimeDuration, setMicroseconds), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void setMilliseconds(int64)", WRAP_MFN(CTimeDuration, setMilliseconds), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void setSeconds(int64)",      WRAP_MFN(CTimeDuration, setSeconds),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void setMinutes(int64)",      WRAP_MFN(CTimeDuration, setMinutes),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void setHours(int64)",        WRAP_MFN(CTimeDuration, setHours),        asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void addNanoseconds(int64)",  WRAP_MFN(CTimeDuration, addNanoseconds),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void addMicroseconds(int64)", WRAP_MFN(CTimeDuration, addMicroseconds), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void addMilliseconds(int64)", WRAP_MFN(CTimeDuration, addMilliseconds), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void addSeconds(int64)",      WRAP_MFN(CTimeDuration, addSeconds),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void addMinutes(int64)",      WRAP_MFN(CTimeDuration, addMinutes),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "void addHours(int64)",        WRAP_MFN(CTimeDuration, addHours),        asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "int64 getNanoseconds()",      WRAP_MFN(CTimeDuration, getNanoseconds),  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "int64 getMicroseconds()",     WRAP_MFN(CTimeDuration, getMicroseconds), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "int64 getSeconds()",          WRAP_MFN(CTimeDuration, getSeconds),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "int64 getMinutes()",          WRAP_MFN(CTimeDuration, getMinutes),      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("CTimeDuration", "int64 getHours()",            WRAP_MFN(CTimeDuration, getHours),        asCALL_GENERIC) );

        // Global functions
        Throw( pEngine->RegisterGlobalFunction("string FormatTimeDuration(const CTimeDuration &in, int)", asFUNCTION(FormatTimeDuration), asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("int64 GetDurationHours(int64)",                    asFUNCTION(GetDurationHours),        asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("int64 GetDurationMinutes(int64)",                  asFUNCTION(GetDurationMinutes),      asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("int64 GetDurationSeconds(int64)",                  asFUNCTION(GetDurationSeconds),      asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("int64 GetDurationMilliseconds(int64)",             asFUNCTION(GetDurationMilliseconds), asCALL_GENERIC) );
        Throw( pEngine->RegisterGlobalFunction("int64 GetDurationMicroseconds(int64)",             asFUNCTION(GetDurationMicroseconds), asCALL_GENERIC) );
    }
}
