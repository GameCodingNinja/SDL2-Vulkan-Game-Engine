
/************************************************************************
*    FILE NAME:       playresult.cpp
*
*    DESCRIPTION:     Class for holding pays
************************************************************************/

// Physical component dependency
#include <slot/playresult.h>
#include <slot/paycombo.h>

// Standard lib dependencies
#include <algorithm>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CPlayResult::CPlayResult() :
    m_totalWinAmount(0)
{
}


/************************************************************************
*    DESC:  Add a slot pay
************************************************************************/
void CPlayResult::addPay(
    const NSlotDefs::EPayType payType,
    const CPayCombo & rCombo,
    const int multiplier,
    const int winLine,
    const std::vector<CSymbPos> & symbPos )
{
    m_payVec.emplace_back( payType, rCombo.getAward(), rCombo.getBonusCode(), multiplier, winLine, symbPos );
}


/************************************************************************
*    DESC:  Sort the pays
************************************************************************/
void CPlayResult::sortPays()
{
    // Sort via lamda expression
    std::sort(m_payVec.begin(), m_payVec.end(),
        [](const CPay & a, const CPay & b){ return (a.getBaseAward() >= b.getBaseAward()); });
}


/************************************************************************
*    DESC:  Add up the win
************************************************************************/
uint CPlayResult::addUpWin()
{
    m_totalWinAmount = 0;

    for( auto & iter : m_payVec )
        m_totalWinAmount += iter.getFinalAward();

    return m_totalWinAmount;
}


/************************************************************************
*    DESC:  Clear the pays
************************************************************************/
void CPlayResult::clear()
{
    m_totalWinAmount = 0;
    m_payVec.clear();
}


/************************************************************************
*    DESC:  Get the pay
************************************************************************/
const CPay & CPlayResult::getPay( int index ) const
{
    return m_payVec.at( index );
}


/************************************************************************
*    DESC:  Get the number of pays
************************************************************************/
uint CPlayResult::getPayCount() const
{
    return m_payVec.size();
}
