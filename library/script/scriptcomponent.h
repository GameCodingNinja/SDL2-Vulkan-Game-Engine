
/************************************************************************
*    FILE NAME:       scriptcomponent.h
*
*    DESCRIPTION:     Class for handling the scripting
************************************************************************/

#pragma once

// Game lib dependencies
#include <script/scriptparam.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward declaration(s)
class asIScriptContext;

class CScriptComponent : boost::noncopyable
{
public:
    
    // Constructor
    CScriptComponent();

    // Destructor
    virtual ~CScriptComponent();
    
    // Prepare the script function to run
    void prepare(
        const std::string & group,
        const std::string & funcName,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );

    // Update the script
    void update( const bool forcedUpdate = false );

    // Reset the contexts and recycle
    void resetAndRecycle();
    
    // Stop a function if it is being called and recycle it
    void stopAndRecycle( const std::string & funcName );

    // Stop a function if it is being called and restart it
    void stopAndRestart(
        const std::string & group,
        const std::string & funcName,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );

    // Is this component active?
    bool isActive();

private:

    // dynamic context vector
    // NOTE: This class does not own the pointer
    std::vector<asIScriptContext *> m_pContextVec;
};
