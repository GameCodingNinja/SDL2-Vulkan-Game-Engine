
/************************************************************************
*    FILE NAME:       symbol2d.h
*
*    DESCRIPTION:     Class for rendering a symbol and it's animations
************************************************************************/

#ifndef __symbol_2d_h__
#define __symbol_2d_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <2d/sprite2d.h>

// Standard lib dependencies
#include <deque>

// Forward declaration(s)
class CSpriteData;

class CSymbol2d : public CObject2D
{
public:

    // Constructor
    CSymbol2d( const std::vector<CSpriteData> & rSpriteDataVec, const std::string & id );

    // Destructor
    virtual ~CSymbol2d();
    
    // Get the sprite
    CSprite2D & getSprite( int index = 0 );
    
    // Get the sprite
    size_t getSpriteCount();
    
    // Get the string id
    const std::string & getId();
    
    // Update the control
    void update();

    // Transform the control
    void transform() override;
    void transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // do the render
    void render( const CMatrix & matrix );
    
    // Get/Set if deferred render
    bool isDeferredRender();
    void setDeferredRender( bool value );
    
private:
    
    // final matrix
    CMatrix m_finalMatrix;
    
    // This symbol's visual representation
    std::deque<CSprite2D> m_spriteDeq;
    
    // string Id;
    std::string m_id;
    
    // Deferred render flag
    bool m_deferredRender;
    
};

#endif  // __symbol_2d_h__


