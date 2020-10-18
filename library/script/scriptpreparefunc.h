
/************************************************************************
*    FILE NAME:       scriptpreparefunc.h
*
*    DESCRIPTION:     Script prepare function falues
************************************************************************/

#pragma once

// Standard lib dependencies
#include <string>

class CScriptPrepareFunc
{
public:
    
    CScriptPrepareFunc(const std::string & grp, const std::string & fId, bool prep, bool force) :
        group(grp),
        funcId(fId),
        prepareOnInit(prep),
        forceUpdate(force)
    {}

    // Group
    std::string group;

    // Function Id
    std::string funcId;

    // Prepare on Init flag
    bool prepareOnInit = false;

    // Force Update flag
    bool forceUpdate = false;
};
