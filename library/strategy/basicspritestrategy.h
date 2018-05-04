
/************************************************************************
*    FILE NAME:       basicspritestrategy.h
*
*    DESCRIPTION:     Basic sprite strategy class
************************************************************************/

#ifndef __basic_sprite_strategy_h__
#define __basic_sprite_strategy_h__

// Physical component dependency
#include <strategy/basestrategy.h>

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/worldvalue.h>
#include <common/spritedatacontainer.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward Declarations
class iSprite;
class CSpriteDataContainer;
class CMatrix;

class CBasicSpriteStrategy : public CBaseStrategy, boost::noncopyable
{
public:

    // Constructor
    CBasicSpriteStrategy();

    // Destructor
    virtual ~CBasicSpriteStrategy();
    
    // Load the sprite data from file
    void loadFromFile( const std::string & file ) override;

    // Create the sprite
    virtual iSprite * create(
        const std::string & dataName,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ) override;
    
    virtual iSprite * create(
        const std::string & group,
        const std::string & name,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ) override;
    
    virtual iSprite * create(
        const std::string & dataName ) override;
    
    virtual iSprite * create(
        const std::string & group,
        const std::string & name ) override;
    
    void cleanUp() override;

    // Update the sprites
    void update() override;

    // Transform the sprite
    void transform() override;

    // Render the sprites
    void render() override;
    void render( const CMatrix & matrix ) override;
    void render( const CMatrix & matrix, const CMatrix & rotMatrix ) override;
    
    // Get the reference to the sprite
    template<typename target>
    target & get( const int id )
    {
        return *dynamic_cast<target *>(getSprite( id ));
    }
    
    // Find if the sprite exists
    bool find( iSprite * piSprite );
    
    // Get the sprite data by name
    CSpriteDataContainer & getData( const std::string & name );

protected:
    
    // Handle the deleting of any sprites
    void deleteObj( int index ) override;
    
    // Handle the creating of any object by name
    void createObj( const std::string & name ) override;
    
    // Get the pointer to the sprite
    iSprite * getSprite( const int id );

protected:
    
    // Map of the sprite data
    std::map<const std::string, CSpriteDataContainer> m_dataMap;

    // Map of all the sprites
    std::map<const int, iSprite *> m_spriteMap;
    
    // Vector of iSprite pointers
    std::vector<iSprite *> m_pSpriteVec;
};

#endif  // __basic_sprite_strategy_h__


