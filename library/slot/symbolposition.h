
/************************************************************************
*    FILE NAME:       symbolposition.h
*
*    DESCRIPTION:     Class for holding the symbol position on the reel strip
************************************************************************/

#ifndef __symbol_position_h__
#define __symbol_position_h__

// Standard lib dependencies
#include <cstdint>

class CSymbPos
{
public:

    // Constructor
    CSymbPos( int reel, int pos ) : m_reel(reel), m_pos(pos)
    {}
    
    int getReel() const
    { return m_reel; }
    
    int getPos() const
    { return m_pos; }

private:
    
    // Reel index
    int16_t m_reel;
    
    // Symbol position
    int16_t m_pos;
};

#endif  // __symbol_position_h__


