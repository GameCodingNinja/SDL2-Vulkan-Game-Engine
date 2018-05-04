
/************************************************************************
*    FILE NAME:       spritechild2d.h
*
*    DESCRIPTION:     Class specific for child sprites where the parent 
*                     does a lot of movement to optimize matrix translations 
*                     for the child and parent.
************************************************************************/

#ifndef __sprite_child_2d_h__
#define __sprite_child_2d_h__

// Physical component dependency
#include <2d/sprite2d.h>

class CSpriteChild2D : public CSprite2D
{
public:

    // Constructor
    CSpriteChild2D( const CObjectData2D & objectData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CSpriteChild2D();
    
    // Transform
    void transform() override;
    void transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;
    
    // do the render
    virtual void render( const CMatrix & matrix ) override;
    
private:
        
    // final matrix
    CMatrix m_finalMatrix;
};

#endif  // __sprite_child_2d_h__


