
/************************************************************************
*    FILE NAME:       slotstripmodel.cpp
*
*    DESCRIPTION:     Slot strip model
************************************************************************/

// Physical component dependency
#include <slot/slotstripmodel.h>

// Game lib dependencies
#include <slot/stripstop.h>

// Standard lib dependencies
#include <iostream>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSlotStripModel::CSlotStripModel(
    const std::vector<CStripStop> & rMathStripVec,
    std::mt19937 & rRng,
    const std::vector<int8_t> & evalSymbIndexVec ) :
        m_rMathStripVec(rMathStripVec),
        m_rRng(rRng),
        m_evalSymbIndexVec(evalSymbIndexVec),
        m_totalWeight(0),
        m_lastStop(0),
        m_stop(0)
{
    // Get the total weight of the slot strip
    for( auto & iter : rMathStripVec )
        m_totalWeight += iter.getWeight();
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSlotStripModel::~CSlotStripModel()
{
}


/************************************************************************
*    DESC:  Generate the strip stop
************************************************************************/
void CSlotStripModel::generateStop()
{
    m_lastStop = m_stop;
    m_stop = 0;
    int weightCount = 0;

    int awardedWeight = m_rRng() % (m_totalWeight + 1);

    for( auto & iter : m_rMathStripVec )
    {
        weightCount += iter.getWeight();

        if( awardedWeight <= weightCount )
            break;

        ++m_stop;
    }
}


/************************************************************************
*    DESC:  Get the last strip stop
************************************************************************/
int CSlotStripModel::getLastStop() const
{
    return m_lastStop;
}


/************************************************************************
*    DESC:  Get the strip stop
************************************************************************/
int CSlotStripModel::getStop() const
{
    return m_stop;
}


/************************************************************************
*    DESC:  Get the math symbol
************************************************************************/
const CMathSymbol & CSlotStripModel::getSymbol( const int stop ) const
{
    const int index = getSymbolIndex( stop );

    return m_rMathStripVec.at(index).getMathSymbol();
}


/************************************************************************
*    DESC:  Get the math symbol index
************************************************************************/
int CSlotStripModel::getSymbolIndex( const int stop ) const
{
    int index = std::abs(stop) % m_rMathStripVec.size();

    if( (stop < 0) && (index != 0) )
        index = m_rMathStripVec.size() - index;

    return index;
}


/************************************************************************
*    DESC:  Get the evaluation symbol index count
************************************************************************/
const std::vector<int8_t> & CSlotStripModel::getEvalIndexVec() const
{
    return m_evalSymbIndexVec;
}


/************************************************************************
*    DESC:  Get the math symbol strip vector
************************************************************************/
const std::vector<CStripStop> & CSlotStripModel::getStripVec() const
{
    return m_rMathStripVec;
}
