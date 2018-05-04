
/************************************************************************
*    FILE NAME:       paycombo.h
*
*    DESCRIPTION:     Class for holding the pay combo
************************************************************************/

#ifndef __pay_combo_h__
#define __pay_combo_h__

// Game lib dependencies
#include <common/defs.h>

// Boost lib dependencies

// Standard lib dependencies
#include <string>

class CPayCombo
{
public:

    // Constructor
    CPayCombo( const std::string & symb, const int count, const uint award, const int bonusCode );
    
    // Get the symbol ID
    const std::string & getSymbol() const;
    
    // Get the number of symbols involved in this pay
    int getCount() const;
    
    // Get the award
    uint getAward() const;
    
    // Get the bonus code
    int getBonusCode() const;

private:
    
    // Symbol for this pay
    const std::string m_symbol;
    
    // Number of symbols in this pay
    const int m_count;
    
    // Credit award
    const uint m_award;
    
    // Bonus code
    const int m_bonusCode;
    
};

#endif  // __pay_combo_h__
