
/************************************************************************
*    FILE NAME:       scriptcomponent.cpp
*
*    DESCRIPTION:     Class for handling the scripting
************************************************************************/

// Physical component dependency
#include <script/scriptcomponent.h>

// Game lib dependencies
#include <script/scriptmanager.h>
#include <utilities/exceptionhandling.h>
#include <utilities/statcounter.h>

// AngelScript lib dependencies
#include <angelscript.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CScriptComponent::CScriptComponent()
{
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CScriptComponent::~CScriptComponent()
{
    // Recycle the contexts we are still holding on to
    resetAndRecycle();
}

/************************************************************************
*    DESC:  Prepare the script function to run
************************************************************************/
void CScriptComponent::prepare(
    const std::string & group,
    const std::string & funcName,
    const std::vector<CScriptParam> & paramVec )
{
    CScriptMgr::Instance().prepare( group, funcName, m_pContextVec, paramVec );
}

/************************************************************************
*    DESC:  Update the script
************************************************************************/
void CScriptComponent::update( const bool forcedUpdate )
{
    if( !m_pContextVec.empty() )
        CScriptMgr::Instance().update( m_pContextVec, forcedUpdate );
}

/************************************************************************
*    DESC:  Is this component active?
************************************************************************/
bool CScriptComponent::isActive()
{
    return !m_pContextVec.empty();
}

/************************************************************************
*    DESC:  Reset the contexts and recycle
************************************************************************/
void CScriptComponent::resetAndRecycle()
{
    if( !m_pContextVec.empty() )
    {
        for( auto iter : m_pContextVec )
        {
            if( iter->GetState() == asEXECUTION_SUSPENDED )
                iter->Abort();

            CScriptMgr::Instance().recycleContext( iter );
        }

        m_pContextVec.clear();
    }
}

/************************************************************************
*    DESC:  Stop a function if it is being called and recycle it
*           This function assume you're checking for the original calling function
************************************************************************/
void CScriptComponent::stopAndRecycle( const std::string & funcName )
{
    if( !m_pContextVec.empty() )
    {
        // See if the function in question is still running
        auto iter = std::find_if( m_pContextVec.begin(), m_pContextVec.end(),
            [funcName](asIScriptContext * pContex)
            { return (funcName == pContex->GetFunction(pContex->GetCallstackSize()-1)->GetName()); } );

        if( iter != m_pContextVec.end() )
        {
            if( (*iter)->GetState() == asEXECUTION_SUSPENDED )
                (*iter)->Abort();

            CScriptMgr::Instance().recycleContext( (*iter) );

            m_pContextVec.erase( iter );
        }
    }
}

/************************************************************************
*    DESC:  Stop a function if it is being called and restart it
************************************************************************/
void CScriptComponent::stopAndRestart(
    const std::string & group, const std::string & funcName, const std::vector<CScriptParam> & paramVec )
{
    // Try to stop and recycle the function if it is active
    stopAndRecycle( funcName );

    // Prepare the script function to run
    prepare( group, funcName, paramVec );
}
