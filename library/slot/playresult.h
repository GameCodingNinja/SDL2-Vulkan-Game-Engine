
/************************************************************************
*    FILE NAME:       playresult.h
*
*    DESCRIPTION:     Class for holding pays
************************************************************************/

#ifndef __play_result_h__
#define __play_result_h__

// Game lib dependencies
#include <slot/pay.h>

// Standard lib dependencies
#include <vector>

// Forward declaration(s)
class CPayCombo;

class CPlayResult
{
public:

    // Constructor
    CPlayResult();
    
    // Add a slot pay
    void addPay(
        const NSlotDefs::EPayType payType,
        const CPayCombo & rCombo,
        const int multiplier,
        const int winLine,
        const std::vector<CSymbPos> & symbPos );
    
    // Sort the pays
    void sortPays();
    
    // Add up the win
    uint addUpWin();
    
    // Get the pay
    const CPay & getPay( int index ) const;
    
    // Get the number of pays
    uint getPayCount() const;
    
    // Clear the pays
    void clear();
    
private:
    
    // Vector of pays
    std::vector<CPay> m_payVec;

    // Total win amount
    uint m_totalWinAmount;
};

#endif  // __play_result_h__


