
/************************************************************************
*    FILE NAME:       spritedatacontainer.cpp
*
*    DESCRIPTION:     Sprite Data Container Class
************************************************************************/

// Physical component dependency
#include <common/spritedatacontainer.h>

// Game lib dependencies
#include <sprite/spritedata.h>
#include <common/actordata.h>
#include <utilities/deletefuncs.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteDataContainer::CSpriteDataContainer( CSpriteData * pData, NDefs::EObjectType type ) :
    m_type(type),
    m_data(pData)
{
}

CSpriteDataContainer::CSpriteDataContainer( CActorData * pData, NDefs::EObjectType type ) :
    m_type(type),
    m_data(pData)
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CSpriteDataContainer::~CSpriteDataContainer()
{
    if( m_type == NDefs::EOT_SPRITE2D )
        NDelFunc::Delete( m_data.m_pSpriteData );
    
    else if( m_type == NDefs::EOT_OBJECT_NODE )
        NDelFunc::Delete( m_data.m_pActorData );
}


/************************************************************************
*    DESC:  Get the data type                                                             
************************************************************************/
int CSpriteDataContainer::getType() const
{
    return m_type;
}
