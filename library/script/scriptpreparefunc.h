
/************************************************************************
*    FILE NAME:       scriptpreparefunc.h
*
*    DESCRIPTION:     Class for holding script attributes
************************************************************************/

#pragma once

// Standard lib dependencies
#include <string>

class CScriptPrepareFunc
{
public:
    
    CScriptPrepareFunc(const std::string & grp, const std::string & fName, bool prep, bool force) :
        group(grp),
        funcName(fName),
        prepareOnInit(prep),
        forceUpdate(force)
    {}

    // Group
    std::string group;

    // Function Id
    std::string funcName;

    // Prepare on Init flag
    bool prepareOnInit = false;

    // Force Update flag
    bool forceUpdate = false;
};
