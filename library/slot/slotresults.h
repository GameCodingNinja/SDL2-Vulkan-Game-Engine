
/************************************************************************
*    FILE NAME:       slotresults.h
*
*    DESCRIPTION:     Class of slot result's
************************************************************************/

#ifndef __slot_results_h__
#define __slot_results_h__

// Game lib dependencies
#include <slot/playresult.h>

// Standard lib dependencies
#include <vector>

class CSlotResults
{
public:

    // Constructor
    CSlotResults();
    
    // Create a new play results entry into the vector
    CPlayResult & create();
    
    // Clear the pays
    void clear();
    
    // Sort the pays
    void sortPays();
    
    // Add up the win
    void addUpWin();
    
    // Get the total win
    uint getTotalWin();
    
    // Do we have a win
    bool isWin();
    
private:
    
    // Play result vector
    std::vector<CPlayResult> m_playResultVec;
    
    // Total win amount
    uint m_totalWinAmount;

};

#endif  // __slot_results_h__


