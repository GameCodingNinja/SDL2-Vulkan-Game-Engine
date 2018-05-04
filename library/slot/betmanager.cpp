
/************************************************************************
*    FILE NAME:       betmanager.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include <slot/betmanager.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CBetMgr::CBetMgr() :
    m_lineBet(0),
    m_totalBet(0),
    m_totalLines(0),
    m_credits(0)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CBetMgr::~CBetMgr()
{
}


/************************************************************************
*    DESC:  Set/Get the line bet
************************************************************************/
void CBetMgr::setLineBet( uint lineBet )
{
    m_lineBet = lineBet;
}

uint CBetMgr::getLineBet() const
{
    return m_lineBet;
}


/************************************************************************
*    DESC:  Set the total lines being bet
************************************************************************/
void CBetMgr::setTotalLines( uint totalLines )
{
    m_totalLines = totalLines;
}


/************************************************************************
*    DESC:  Get the total bet
************************************************************************/
uint CBetMgr::getTotalBet() const
{
    return m_lineBet * m_totalLines;
}


/************************************************************************
*    DESC:  Set/Get the credits
************************************************************************/
void CBetMgr::setCredits( uint credits )
{
    m_credits = credits;
}

uint CBetMgr::getCredits() const
{
    return m_credits;
}


/************************************************************************
*    DESC:  Is there anough credits to play?
************************************************************************/
bool CBetMgr::allowPlay() const
{
    return ((m_credits > 0) && (m_credits >= getTotalBet()));
}


/************************************************************************
*    DESC:  Deduct the bet amount from the credits
************************************************************************/
void CBetMgr::deductBet()
{
    if( allowPlay() )
        m_credits -= getTotalBet();
}


/************************************************************************
*    DESC:  Add in the award
************************************************************************/
void CBetMgr::addAward( uint award )
{
    m_credits += award;
}
