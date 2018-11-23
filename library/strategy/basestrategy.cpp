
/************************************************************************
*    FILE NAME:       basestrategy.cpp
*
*    DESCRIPTION:     Base strategy class
************************************************************************/

// Physical component dependency
#include <strategy/basestrategy.h>

// Game lib dependencies
#include <managers/signalmanager.h>
#include <common/defs.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CBaseStrategy::CBaseStrategy()
{
}


/************************************************************************
 *    DESC:  Set to Destroy the sprite
 ************************************************************************/
void CBaseStrategy::setToDestroy( const int spriteIndex )
{
    m_deleteSet.insert( spriteIndex );
}


/************************************************************************
 *    DESC:  Set to create the sprite
 ************************************************************************/
void CBaseStrategy::setToCreate( const std::string & name )
{
    m_createSet.insert( name );
}


/***************************************************************************
*    DESC:  Handle the deleting of any object by Id
****************************************************************************/
void CBaseStrategy::handleDelete()
{
    if( !m_deleteSet.empty() )
    {
        for( auto iter : m_deleteSet )
            deleteObj( iter );

        m_deleteSet.clear();
    }
}


/***************************************************************************
*    DESC:  Handle the deleting of any object by Id
****************************************************************************/
void CBaseStrategy::deleteObj( const int id )
{
    // Virtual function meant to be over written by inherited class
}


/***************************************************************************
*    DESC:  Handle the creating of new sprite objects by name
****************************************************************************/
void CBaseStrategy::handleCreate()
{
    if( !m_createSet.empty() )
    {
        for( auto iter : m_createSet )
            createObj( iter );

        m_createSet.clear();
    }
}


/***************************************************************************
*    DESC:  Handle the creating of any object by name
****************************************************************************/
void CBaseStrategy::createObj( const std::string & name )
{
    // Virtual function meant to be over written by inherited class
}
