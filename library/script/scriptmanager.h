
/************************************************************************
*    FILE NAME:       scriptmanager.h
*
*    DESCRIPTION:     Class to manage AngelScripts
************************************************************************/

#pragma once

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <utilities/smartpointers.h>
#include <script/scriptparam.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
class asIScriptEngine;
class asIScriptContext;
class asIScriptModule;
class asIScriptFunction;
class asITypeInfo;
struct asSMessageInfo;

class CScriptMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CScriptMgr & Instance()
    {
        static CScriptMgr scriptManager;
        return scriptManager;
    }

    // Derived class loading of class specific data
    void loadUniqueData( const XMLNode & node, const std::string & group ) override;

    // Get the pointer to the script engine
    asIScriptEngine * getEnginePtr();

    // Load all of the scripts of a specific group
    void loadGroup( const std::string & group, const bool forceLoadFromScript = false );

    // Free all of the scripts of a specific data group
    void freeGroup( const std::string & group );
    
    // Delete all the contexts
    void clear();

    // Get the script engine contex from a managed pool
    // NOTE: The receiver of this pointer is the owner if it's still 
    //       holding on to it when the game terminates
    asIScriptContext * getContext();
    
    // Update the active scripts
    bool update();
    void update( std::vector<asIScriptContext *> & pContextVec, const bool forcedUpdate = false );

    // Add the script context back to the managed pool
    void recycleContext( asIScriptContext * pContext );

    // Get pointer to function
    asIScriptFunction * getPtrToFunc( const std::string & group, const std::string & name );
    
    // Get pointer to type declaration
    asITypeInfo * getPtrToTypeInfo( const std::string & typeDecl );
    
    // Prepare the script function to run
    void prepare(
        const std::string & group,
        const std::string & funcName,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );
    
    void prepare(
        const std::string & group,
        const std::string & funcName,
        std::vector<asIScriptContext *> & pContextVec,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );
    
    void prepare(
        const std::string & group,
        const std::string & funcName,
        asIScriptContext * pContext,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );
    
    // Prepare the spawn script function to run
    void prepareSpawn( const std::string & funcName, const std::string & group = "" );

    // Spawn by thread
    void spawnByThread( const std::string & funcName, const std::string & group = "" );

    // Set the max pool percentage
    void setMaxPoolPercentage( float poolPercentage );

private:

    // Constructor
    CScriptMgr();

    // Destructor
    virtual ~CScriptMgr();

    // Add the script to the module
    void addScript( asIScriptModule * pScriptModule, const std::string & filePath );

    // Build all the scripts added to the module
    void buildScript( asIScriptModule * pScriptModule, const std::string & group );

    // Call back to display AngelScript messages
    void messageCallback(const asSMessageInfo & msg);
    
    // Execute the script
    void executeScript( asIScriptContext * pContext );
    
    // Execute the script from thread
    void executeFromThread( asIScriptContext * pContext );

private:

    const bool FORCE_LOAD_FROM_SCRIPT = true;

    // Smart com pointer to AngelScript script engine
    NSmart::scoped_com_ptr<asIScriptEngine> scpEngine;

    // Get a pointer to type declaration
    std::map< const std::string, std::map< const std::string, asIScriptFunction * > > m_scriptFunctMapMap;
    
    // Holds the pointer to type declaration
    std::map<const std::string, asITypeInfo *> m_pTypeDeclMap;

    // Holds the pool of script contexts
    std::vector<asIScriptContext *> m_pContextPoolVec;
    
    // Holds active contexts that are executing scripts
    std::vector<asIScriptContext *> m_pActiveContextVec;
    
    // Error string messages
    std::string m_errorTitle;
    std::string m_errorMsg;

    // Active context counter
    int m_activeContextCounter = 0;

    // Maximum pool percentage
    float m_maxPoolPercentage = 0.25f;

    // Holds the list table map
    std::map<const std::string, std::string> m_byteCodeFileMap;
};
