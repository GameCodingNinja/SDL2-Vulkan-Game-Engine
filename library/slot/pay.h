
/************************************************************************
*    FILE NAME:       pay.h
*
*    DESCRIPTION:     Slot pay
************************************************************************/

#ifndef __pay_h__
#define __pay_h__

// Game lib dependencies
#include <common/defs.h>
#include <slot/slotdefs.h>
#include <slot/symbolposition.h>

// Standard lib dependencies
#include <vector>

class CPay
{
public:

    // Constructor
    CPay(
        const NSlotDefs::EPayType payType,
        const uint award,
        const int bonusCode,
        const uint multiplier,
        const int payLine,
        const std::vector<CSymbPos> & symbPos );
    
    // Get the pay type
    NSlotDefs::EPayType getPayType() const;
    
    // Get the base award
    uint getBaseAward() const;
    
    // Get the final award
    uint getFinalAward() const;
    
    // Get the multiplier
    uint getMultiplier() const;
    
    // Get the pay line
    int getPayLine() const;
    
    // Get the bonus code
    int getBonusCode() const;
    
    // Get the symbol positions
    const std::vector<CSymbPos> & getSymbPos() const;
    
    // Debug output
    void debug() const;
    
private:
    
    // Type of pay that's being awarded
    NSlotDefs::EPayType m_payType;
    
    // The amount of the win
    uint m_award;
    
    // bonus code
    int m_bonusCode;
    
    // multiplier
    uint m_multiplier;
    
    // Pay line the award was on
    int m_payLine;
    
    // Vector of symbol positions per reel that contribute to the win
    std::vector<CSymbPos> m_symbPosVec;

};

#endif  // __pay_h__


