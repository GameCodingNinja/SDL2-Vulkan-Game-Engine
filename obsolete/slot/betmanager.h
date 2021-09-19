
/************************************************************************
*    FILE NAME:       betmanager.h
*
*    DESCRIPTION:     Singleton to manage the bet
************************************************************************/

#ifndef __bet_manager_h__
#define __bet_manager_h__

// Game lib dependencies
#include <common/defs.h>

class CBetMgr
{
public:

    // Get the instance of the singleton class
    static CBetMgr & Instance()
    {
        static CBetMgr betMgr;
        return betMgr;
    }
    
    // Set/Get the line bet
    void setLineBet( uint lineBet );
    uint getLineBet() const;
    
    // Set the total lines being bet
    void setTotalLines( uint totalLinesBet );
    
    // Get the total bet
    uint getTotalBet() const;
    
    // Set/Get the credits
    void setCredits( uint credits );
    uint getCredits() const;
    
    // Is there anough credits to play?
    bool allowPlay() const;
    
    // Deduct the bet amount from the credits
    void deductBet();
    
    // Add in the award
    void addAward( uint award );
    
private:
    
    // Constructor
    CBetMgr();

    // Destructor
    ~CBetMgr();
    
private:
    
    // line bet
    uint m_lineBet;
    
    // Total bet
    uint m_totalBet;
    
    // Total number of lines being bet
    uint m_totalLines;
    
    // Total credits
    uint m_credits;
};

#endif
