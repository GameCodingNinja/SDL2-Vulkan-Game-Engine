
/************************************************************************
*    FILE NAME:       slotresults.cpp
*
*    DESCRIPTION:     Class of slot result's
************************************************************************/

// Physical component dependency
#include <slot/slotresults.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSlotResults::CSlotResults() :
    m_totalWinAmount(0)
{
}


/************************************************************************
*    DESC:  Create a new play results entry into the vector
************************************************************************/
CPlayResult & CSlotResults::create()
{
    m_playResultVec.emplace_back();

    return m_playResultVec.back();
}


/************************************************************************
*    DESC:  Clear the pays
************************************************************************/
void CSlotResults::clear()
{
    m_totalWinAmount = 0;

    for( auto & iter : m_playResultVec )
        iter.clear();
}


/************************************************************************
*    DESC:  Sort the pays
************************************************************************/
void CSlotResults::sortPays()
{
    for( auto & iter : m_playResultVec )
        iter.sortPays();
}


/************************************************************************
*    DESC:  Add up the win
************************************************************************/
void CSlotResults::addUpWin()
{
    m_totalWinAmount = 0;

    for( auto & iter : m_playResultVec )
        m_totalWinAmount += iter.addUpWin();
}


/************************************************************************
*    DESC:  Get the total win
************************************************************************/
uint CSlotResults::getTotalWin()
{
    return m_totalWinAmount;
}


/************************************************************************
*    DESC:  Do we have a win
************************************************************************/
bool CSlotResults::isWin()
{
    return (m_totalWinAmount > 0);
}
