
/************************************************************************
*    FILE NAME:       spritedatacontainer.h
*
*    DESCRIPTION:     Sprite Data Container Class
************************************************************************/

#ifndef __sprite_data_container_h__
#define __sprite_data_container_h__

// Game lib dependencies
#include <common/defs.h>

// Forward Declarations
class CSpriteData;
class CActorData;

class CSpriteDataContainer
{
public:

    // constructor
    CSpriteDataContainer( CSpriteData * pData, NDefs::EObjectType type );
    CSpriteDataContainer( CActorData * pData, NDefs::EObjectType type );

    // destructor
    ~CSpriteDataContainer();
    
    // Get the data type
    int getType() const;
    
    // Get the data
    template<typename T>
    T & get() const;
    
private:

    // Sprite Type
    NDefs::EObjectType m_type = NDefs::EOT_NONE;

    // Pointer to sprite data class. We own the pointer
    union USpriteDataPtr
    {
        USpriteDataPtr( CSpriteData * pData ) : m_pSpriteData(pData)
        {}
        USpriteDataPtr( CActorData * pData ) : m_pActorData(pData)
        {}

        CSpriteData * m_pSpriteData;
        CActorData * m_pActorData;

    } m_data;
};

template<> inline CSpriteData & CSpriteDataContainer::get() const { return *m_data.m_pSpriteData; }
template<> inline CActorData  & CSpriteDataContainer::get() const { return *m_data.m_pActorData; }

#endif  // __sprite_data_h__


