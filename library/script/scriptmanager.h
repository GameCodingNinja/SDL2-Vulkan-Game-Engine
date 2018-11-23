
/************************************************************************
*    FILE NAME:       scriptmanager.h
*
*    DESCRIPTION:     Class to manage AngelScripts
************************************************************************/

#ifndef __script_manager_h__
#define __script_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <utilities/smartpointers.h>
#include <script/scriptdefs.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward declaration(s)
class asIScriptEngine;
class asIScriptContext;
class asIScriptModule;
class asIScriptContext;
class asIScriptFunction;
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

    // Get the pointer to the script engine
    asIScriptEngine * getEnginePtr();

    // Load all of the scripts of a specific group
    void loadGroup( const std::string & group );

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
    void update( std::vector<asIScriptContext *> & pContextVec );

    // Add the script context back to the managed pool
    void recycleContext( asIScriptContext * pContext );

    // Get pointer to function
    asIScriptFunction * getPtrToFunc( const std::string & group, const std::string & name );
    
    // Prepare the script function to run
    void prepare(
        const std::string & group,
        const std::string & funcName,
        std::vector<asIScriptContext *> & pContextVec,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );
    
    void prepare(
        const std::string & group,
        const std::string & funcName,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );
    
    // Prepare the spawn script function to run
    void prepareSpawn( const std::string & funcName, const std::string & group = "" );
    void prepareSpawnVoid( const std::string & funcName, void * pVoid );
    void prepareLocalSpawnVoid( const std::string & funcName, void * pVoid );

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

private:

    // Smart com pointer to AngelScript script engine
    NSmart::scoped_com_ptr<asIScriptEngine> scpEngine;

    // Map containing a group of function pointers
    std::map< const std::string, std::map< const std::string, asIScriptFunction * > > m_scriptFunctMapMap;

    // Holds the pool of script contexts
    std::vector<asIScriptContext *> m_pContextPoolVec;
    
    // Holds active contexts that are executing scripts
    std::vector<asIScriptContext *> m_pActiveContextVec;
    
    // Holds the spawn contexts to be added to the component pool
    std::vector<asIScriptContext *> m_pLocalSpawnContextVec;

};

#endif
