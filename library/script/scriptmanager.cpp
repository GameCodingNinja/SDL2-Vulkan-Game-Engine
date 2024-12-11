
/************************************************************************
*    FILE NAME:       scriptmanager.cpp
*
*    DESCRIPTION:     Class to manage AngelScripts
************************************************************************/

// Physical component dependency
#include <script/scriptmanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <utilities/statcounter.h>
#include <utilities/settings.h>
#include <utilities/threadpool.h>
#include <script/bytecodestream.h>

// Boost lib dependencies
#include <boost/format.hpp>

// AngelScript lib dependencies
#include <angelscript.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CScriptMgr::CScriptMgr()
{
    // Create the script engine
    scpEngine.reset( asCreateScriptEngine(ANGELSCRIPT_VERSION) );
    if( scpEngine.isNull() )
    {
        throw NExcept::CCriticalException("Error Creating AngelScript Engine!",
            boost::str( boost::format("AngelScript engine could not be created.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    }

    // We don't use call backs for every line execution so set this property to not build with line cues
    scpEngine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true);

    // Set the message callback to print the messages that the engine gives in case of errors
    if( scpEngine->SetMessageCallback(asMETHOD(CScriptMgr, messageCallback), this, asCALL_THISCALL) < 0 )
    throw NExcept::CCriticalException("Error Creating AngelScript Engine!",
        boost::str( boost::format("AngelScript message callback could not be created.\n\n%s\nLine: %s")
            % __FUNCTION__ % __LINE__ ));
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CScriptMgr::~CScriptMgr()
{
}


/************************************************************************
*    DESC:  Derived class loading of class specific data
*           NOTE: This function is used to save script byte code
************************************************************************/
void CScriptMgr::loadUniqueData( const XMLNode & node, const std::string & group )
{
    if( node.isAttributeSet("byteCodeFile") )
    {
        // Make sure it's not an empty path
        std::string byteCodePath = node.getAttribute("byteCodeFile");
        if( !byteCodePath.empty() )
        {
            // Save the byte code file path for loading later
            m_byteCodeFileMap.emplace(group, byteCodePath);

            // Do we want to save out the byte code
            if( CSettings::Instance().getSaveByteCode() )
            {
                // Load the scripts for this group
                loadGroup( group, FORCE_LOAD_FROM_SCRIPT );

                // Get the script module. It should be built by the previous call
                asIScriptModule * pScriptModule = scpEngine->GetModule(group.c_str(), asGM_ONLY_IF_EXISTS);
                if( pScriptModule == nullptr )
                {
                    throw NExcept::CCriticalException("Script Byte Code Save Error!",
                        boost::str( boost::format("Error script module does not exist (%s).\n\n%s\nLine: %s")
                            % group % __FUNCTION__ % __LINE__ ));
                }

                // Save the byte code
                CByteCodeStream byteCode( byteCodePath, "wb" );
                if( pScriptModule->SaveByteCode( &byteCode, CSettings::Instance().getStripDebugInfo() ) < 0 )
                {
                    throw NExcept::CCriticalException("Script Byte Code Save Error!",
                        boost::str( boost::format("Error writing script byte code (%s).\n\n%s\nLine: %s")
                            % group % __FUNCTION__ % __LINE__ ));
                }

                // Discard this module and all it's contents.
                // It will be reloaded through the normal process and will error if it's not discarded
                pScriptModule->Discard();
            }
        }
    }
}


/************************************************************************
*    DESC:  Delete all the strategies
************************************************************************/
void CScriptMgr::clear()
{
    // Release the context pool
    for( auto iter : m_pContextPoolVec )
        iter->Release();

    for( auto iter : m_pActiveContextVec )
        iter->Release();
    
    m_pContextPoolVec.clear();
    m_pActiveContextVec.clear();
    m_activeContextCounter = 0;
}


/************************************************************************
*    DESC:  Load all of the scripts of a specific group
*
*    param: string & group - specified group of scripts to load
************************************************************************/
void CScriptMgr::loadGroup( const std::string & group, const bool forceLoadFromScript )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Script List Load Group Data Error!",
            boost::str( boost::format("Script list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Create the module if it doesn't already exist
    asIScriptModule * pScriptModule = scpEngine->GetModule(group.c_str(), asGM_CREATE_IF_NOT_EXISTS);
    if( pScriptModule == nullptr )
    {
        throw NExcept::CCriticalException("Script List load Error!",
            boost::str( boost::format("Error creating script group module (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

    // See if we have the file path to the byte code
    auto byteCodeFileIter = m_byteCodeFileMap.find( group );

    // Load the byte code
    if( CSettings::Instance().getLoadByteCode() && byteCodeFileIter != m_byteCodeFileMap.end() && !forceLoadFromScript )
    {
        CByteCodeStream byteCode( byteCodeFileIter->second, "rb" );
        if( pScriptModule->LoadByteCode( &byteCode ) < 0 )
        {
            throw NExcept::CCriticalException("Script Byte Code Load Error!",
                boost::str( boost::format("Error loading script byte code (%s).\n\n%s\nLine: %s")
                    % group % __FUNCTION__ % __LINE__ ));
        }
    }
    else
    {
        // Add the scripts to the module
        for( auto & iter : listTableIter->second )
            addScript( pScriptModule, iter );

        // Build all the scripts added to the module
        buildScript( pScriptModule, group );
    }
}


/************************************************************************
*    DESC:  Add the script to the module
************************************************************************/
void CScriptMgr::addScript( asIScriptModule * pScriptModule, const std::string & filePath )
{
    // Load the script file into a charater array
    std::vector<char> bufVec = NGenFunc::FileToVec( filePath, NGenFunc::TERMINATE );

    // Load script into module section - the file path is it's ID
    if( pScriptModule->AddScriptSection(filePath.c_str(), bufVec.data() ) < 0 )
    {
        throw NExcept::CCriticalException("Script List load Error!",
            boost::str( boost::format("Error loading script (%s).\n\n%s\nLine: %s")
                % filePath % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Build all the scripts added to the module
************************************************************************/
void CScriptMgr::buildScript( asIScriptModule * pScriptModule, const std::string & group )
{
    // Build the script
    int error = pScriptModule->Build();
    if( error < 0 )
    {
        throw NExcept::CCriticalException("Script List build Error!",
            boost::str( boost::format("Error building script group with error code %d. (%s).\n\n%s\nLine: %s")
                % error % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Get the script context from a managed pool
*
*    NOTE: The receiver of this pointer is the owner if it's still
*          holding on to it when the game terminates
************************************************************************/
asIScriptContext * CScriptMgr::getContext()
{
    // Set the active contex counter
    CStatCounter::Instance().setActiveContexCounter( ++m_activeContextCounter );

    if( !m_pContextPoolVec.empty() )
    {
        asIScriptContext * pContex = m_pContextPoolVec.back();
        m_pContextPoolVec.pop_back();

        // Set the pool contex counter
        CStatCounter::Instance().setPoolContexCounter( m_pContextPoolVec.size() );

        return pContex;
    }

    return scpEngine->CreateContext();
}


/************************************************************************
*    DESC:  Add the script context back to the managed pool
************************************************************************/
void CScriptMgr::recycleContext( asIScriptContext * pContext )
{
    if( m_pContextPoolVec.size() < (float)m_activeContextCounter * m_maxPoolPercentage )
        m_pContextPoolVec.push_back( pContext );
    else
        pContext->Release();
    
    CStatCounter::Instance().setPoolContexCounter( m_pContextPoolVec.size() );
    CStatCounter::Instance().setActiveContexCounter( --m_activeContextCounter );
}


/************************************************************************
*    DESC:  Get pointer to function name
************************************************************************/
asIScriptFunction * CScriptMgr::getPtrToFunc( const std::string & group, const std::string & name )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_scriptFunctMapMap.find( group );
    if( mapMapIter == m_scriptFunctMapMap.end() )
        mapMapIter = m_scriptFunctMapMap.emplace( group, std::map<const std::string, asIScriptFunction *>() ).first;

    // See if this function pointer has already been saved
    auto mapIter = mapMapIter->second.find( name );

    // If it's not found, find the function and add it to the map
    // It's faster to keep the function pointers in a map then to use AngelScript's GetFunction call.
    if( mapIter == mapMapIter->second.end() )
    {
        asIScriptModule * pScriptModule = scpEngine->GetModule(group.c_str(), asGM_ONLY_IF_EXISTS);
        if( pScriptModule == nullptr )
        {
            throw NExcept::CCriticalException("Error Getting Pointer to Function!",
                boost::str( boost::format("AngelScript module does not exist (%s, %s).\n\n%s\nLine: %s")
                    % group % name % __FUNCTION__ % __LINE__ ));
        }

        // Try to get the pointer to the function
        asIScriptFunction *pScriptFunc = pScriptModule->GetFunctionByName(name.c_str());
        if( pScriptFunc == nullptr )
        {
            throw NExcept::CCriticalException("Error Getting Pointer to Function!",
                boost::str( boost::format("AngelScript function does not exist (%s, %s).\n\n%s\nLine: %s")
                    % group % name % __FUNCTION__ % __LINE__ ));
        }

        // Insert the function pointer into the map
        mapIter = mapMapIter->second.emplace( name, pScriptFunc ).first;
    }

    return mapIter->second;
}


/************************************************************************
*    DESC:  Get a pointer to type declaration
************************************************************************/
asITypeInfo * CScriptMgr::getPtrToTypeInfo( const std::string & typeDecl )
{
    // See if this declaration pointer has already been saved
    auto mapIter = m_pTypeDeclMap.find( typeDecl );
    if( mapIter == m_pTypeDeclMap.end() )
    {
        // Generate a type info pointer and store it for quick recall
        auto pTypeInfo = scpEngine->GetTypeInfoByDecl( typeDecl.c_str() );
        mapIter = m_pTypeDeclMap.emplace( typeDecl, pTypeInfo ).first;
    }
    
    return mapIter->second;
}


/************************************************************************
*    DESC:  Call back to display AngelScript messages
************************************************************************/
void CScriptMgr::messageCallback(const asSMessageInfo & msg)
{
    std::string type = "ERROR";
    if( msg.type == asMSGTYPE_WARNING )
        type = "WARN";

    else if( msg.type == asMSGTYPE_INFORMATION )
        type = "INFO";

    std::string msgStr = boost::str( boost::format( "%s (%d, %d) : %s : %s" ) % msg.section % msg.row % msg.col % type % msg.message );

    NGenFunc::PostDebugMsg( msgStr );
}


/************************************************************************
*    DESC:  Get the pointer to the script engine
************************************************************************/
asIScriptEngine * CScriptMgr::getEnginePtr()
{
    return scpEngine.get();
}


/************************************************************************
*    DESC:  Free all of the scripts of a specific data group
************************************************************************/
void CScriptMgr::freeGroup( const std::string & group )
{
    // Make sure the group we are looking for exists
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Script List Group Error!",
            boost::str( boost::format("Script group can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Discard the module and free its memory.
    scpEngine->DiscardModule( group.c_str() );

    // Erase the group from the map
    auto mapMapIter = m_scriptFunctMapMap.find( group );
    if( mapMapIter != m_scriptFunctMapMap.end() )
            m_scriptFunctMapMap.erase( mapMapIter );
}


/************************************************************************
*    DESC:  Prepare the script function to run
************************************************************************/
void CScriptMgr::prepare(
    const std::string & group,
    const std::string & funcName,
    const std::vector<CScriptParam> & paramVec )
{
    prepare( group, funcName, m_pActiveContextVec, paramVec );
}

void CScriptMgr::prepare(
    const std::string & group,
    const std::string & funcName,
    std::vector<asIScriptContext *> & pContextVec,
    const std::vector<CScriptParam> & paramVec )
{
    // Get a context from the script manager pool
    pContextVec.push_back( getContext() );

    prepare( group, funcName, pContextVec.back(), paramVec );
}

void CScriptMgr::prepare(
    const std::string & group,
    const std::string & funcName,
    asIScriptContext * pContext,
    const std::vector<CScriptParam> & paramVec )
{
    // Get the function pointer
    asIScriptFunction * pScriptFunc = getPtrToFunc(group, funcName);

    // Prepare the function to run
    if( pContext->Prepare(pScriptFunc) < 0 )
    {
        throw NExcept::CCriticalException("Error Preparing Script!",
            boost::str( boost::format("There was an error preparing the script (%s).\n\n%s\nLine: %s")
                % funcName % __FUNCTION__ % __LINE__ ));
    }

    // Pass the parameters to the script function
    for( size_t i = 0; i < paramVec.size(); ++i )
    {
        int returnVal(0);

        if( paramVec[i].getType() == CScriptParam::EPT_BOOL )
        {
            returnVal = pContext->SetArgByte(i, paramVec[i].get<bool>());
        }
        else if( paramVec[i].getType() == CScriptParam::EPT_INT )
        {
            returnVal = pContext->SetArgDWord(i, paramVec[i].get<int>());
        }
        else if( paramVec[i].getType() == CScriptParam::EPT_UINT )
        {
            returnVal = pContext->SetArgDWord(i, paramVec[i].get<uint>());
        }
        else if( paramVec[i].getType() == CScriptParam::EPT_FLOAT )
        {
            returnVal = pContext->SetArgFloat(i, paramVec[i].get<float>());
        }
        else if( paramVec[i].getType() == CScriptParam::EPT_REG_OBJ )
        {
            returnVal = pContext->SetArgObject(i, paramVec[i].get<void *>());
        }

        if( returnVal < 0 )
        {
            throw NExcept::CCriticalException("Error Setting Script Param!",
                boost::str( boost::format("There was an error setting the script parameter (%s).\n\n%s\nLine: %s")
                    % funcName % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
*    DESC:  Prepare the spawn script function to run
************************************************************************/
void CScriptMgr::prepareSpawn( const std::string & funcName, const std::string & group )
{
    auto pContex = asGetActiveContext();
    if( pContex || !group.empty() )
    {
        // Get the module name
        std::string grp = group;
        if( group.empty() )
            grp = pContex->GetFunction()->GetModuleName();

        // Prepare the script function to run
        prepare( grp, funcName, m_pActiveContextVec );
    }
}


/************************************************************************
*    DESC:  Spawn by thread
************************************************************************/
void CScriptMgr::spawnByThread( const std::string & funcName, const std::string & group )
{
    auto pContex = asGetActiveContext();
    if( pContex || !group.empty() )
    {
        // Get the module name
        std::string grp = group;
        if( group.empty() )
            grp = pContex->GetFunction()->GetModuleName();
        
        // Prepare the script function to run
        // Don't prepare the script in the thread because that calls the script engine
        // and requires the use of a cleanup function call asThreadCleanup
        pContex = getContext();
        prepare( grp, funcName, pContex );

        // Use the thread pool if active.
        if( CThreadPool::Instance().isActive() )
        {
            CThreadPool::Instance().post( &CScriptMgr::executeFromThread, this, pContex );
        }
        else
        {
            // Execute the script from thread
            std::thread load( &CScriptMgr::executeFromThread, this, pContex );
            load.detach();
        }
    }
}


/************************************************************************
*    DESC:  Execute the script from thread
************************************************************************/
void CScriptMgr::executeFromThread( asIScriptContext * pContext )
{
    try
    {
        do
        {
            // Execute the script
            executeScript( pContext );
        }
        // Keep executing if there are any suspends in the script
        while( pContext->GetState() == asEXECUTION_SUSPENDED );
    }
    catch( NExcept::CCriticalException & ex )
    {
        m_errorTitle = ex.getErrorTitle();
        m_errorMsg = ex.getErrorMsg();
    }
    catch( std::exception const & ex )
    {
        m_errorTitle = "Standard Exception";
        m_errorMsg = ex.what();
    }
    catch(...)
    {
        m_errorTitle = "Unknown Error";
        m_errorMsg = "Something bad happened and I'm not sure what it was.";
    }
    
    // release it here
    pContext->Release();
}


/************************************************************************
*    DESC:  Update the script
************************************************************************/
bool CScriptMgr::update()
{
    // Re-throw any threaded exceptions
    if( !m_errorMsg.empty() )
        throw NExcept::CCriticalException( m_errorTitle, m_errorMsg );
    
    if( !m_pActiveContextVec.empty() )
        update( m_pActiveContextVec );
    
    return !m_pActiveContextVec.empty();
}

void CScriptMgr::update( std::vector<asIScriptContext *> & pContextVec, const bool forcedUpdate )
{
    // This is to allow for "forced Updates" when a new script has been added.
    // A new script is always the last one in the vector
    size_t i = 0;
    if( forcedUpdate )
        i = pContextVec.size() - 1;

    // Using a for loop because it allows the m_pActiveContextVec to grow
    // while the for loop is executing as spawn contexts are added.
    // DO NOT change to a C++11 ranged for loop. It won't work.
    for( ; i < pContextVec.size(); ++i )
        executeScript( pContextVec[i] );

    auto iter = pContextVec.begin();
    while( iter != pContextVec.end() )
    {
        // Return the context to the pool if it has not been suspended
        if( (*iter)->GetState() != asEXECUTION_SUSPENDED )
        {
            recycleContext( (*iter) );
            iter = pContextVec.erase( iter );
        }
        else
        {
            ++iter;
        }
    }
}


/************************************************************************
*    DESC:  Execute the script
************************************************************************/
void CScriptMgr::executeScript( asIScriptContext * pContext )
{
    // See if this context is still being used
    if( (pContext->GetState() == asEXECUTION_SUSPENDED) ||
        (pContext->GetState() == asEXECUTION_PREPARED) )
    {
        // Execute the script and check for errors
        // Since the script can be suspended, this also is used to continue execution
        const int execReturnCode = pContext->Execute();
        if( execReturnCode == asEXECUTION_ERROR || execReturnCode == asEXECUTION_EXCEPTION )
        {
            std::string errorStr = boost::str( boost::format("ERROR executing script (%s).")
                % pContext->GetExceptionString() );

            for( asUINT i = 0; i < pContext->GetCallstackSize(); i++ )
            {
                asIScriptFunction *func;
                const char *scriptSection;
                int line, column;
                func = pContext->GetFunction(i);
                line = pContext->GetLineNumber(i, &column, &scriptSection);

                errorStr += boost::str( boost::format("\n  %s; %s; %d, %d")
                    % scriptSection
                    % func->GetDeclaration()
                    % line
                    % column );
            }
            
            throw NExcept::CCriticalException( "Error Calling Script!", errorStr );
        }
    }
}


/************************************************************************
*    DESC:  Set the max pool percentage
************************************************************************/
void CScriptMgr::setMaxPoolPercentage( float poolPercentage )
{
    m_maxPoolPercentage = poolPercentage;
}
