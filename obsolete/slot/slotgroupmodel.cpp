
/************************************************************************
*    FILE NAME:       slotgroupmodel.cpp
*
*    DESCRIPTION:     Model class for the slot group
************************************************************************/

// Physical component dependency
#include <slot/slotgroupmodel.h>

// Game lib dependencies
#include <slot/slotmath.h>
#include <slot/playresult.h>
#include <slot/symbolposition.h>
#include <slot/slotmathmanager.h>
#include <slot/betmanager.h>
#include <slot/paylineset.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <chrono>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSlotGroupModel::CSlotGroupModel( const CSlotMath & rSlotMath, CPlayResult & rPlayResult ) :
    m_rSlotMath( rSlotMath ),
    m_rPlayResult( rPlayResult ),
    m_rPaylineSet( CSlotMathMgr::Instance().getPaylineSet( rSlotMath.getPaylineSetID() ) )
{
    // Seed the random number generator
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto seed = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
    m_rng.seed( seed );

}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSlotGroupModel::~CSlotGroupModel()
{
}


/************************************************************************
*    DESC:  Create the model strips
************************************************************************/
void CSlotGroupModel::create(
    const std::string & rStripSetId, const std::string & paytableSetId )
{
    m_paytableSetId = paytableSetId;

    // Get the reel strip set
    auto & rStripSetVec = m_rSlotMath.getStripSet( rStripSetId );

    // Create the model strips
    for( auto & iter : rStripSetVec )
        m_slotStripModelDeq.emplace_back( m_rSlotMath.getStrip( iter.getId() ), m_rng, iter.getEvalIndexVec() );

    // Setup the evaluation vectors
    m_evalMathSymbs.resize( rStripSetVec.size() );
    for( size_t i = 0; i < rStripSetVec.size(); ++i )
        m_evalMathSymbs.at(i).resize( rStripSetVec.at(i).getEvalIndexVec().size() );
}


/************************************************************************
*    DESC:  Generate the strip stops
************************************************************************/
void CSlotGroupModel::generateStops()
{
    for( auto & iter : m_slotStripModelDeq )
        iter.generateStop();
}


/************************************************************************
*    DESC:  Evaluate the strips
************************************************************************/
void CSlotGroupModel::evaluate()
{
    generateEvalSymbs();

    // Get the paytable set and evaluate as line pay or scatter
    auto & rPaytableSetVec = m_rSlotMath.getPaytableSet( m_paytableSetId );
    for( auto & iter: rPaytableSetVec )
    {
        if( iter.getType() == NSlotDefs::EP_PAYLINE )
            evaluateLinePays( iter.getId() );

        else if( iter.getType() == NSlotDefs::EP_SCATTER )
            evaluateScatters( iter.getId() );
    }
}


/************************************************************************
*    DESC:  Generate the evaluation symbols
************************************************************************/
void CSlotGroupModel::generateEvalSymbs()
{
    for( size_t strip = 0; strip < m_slotStripModelDeq.size(); ++strip )
    {
        const int stop = m_slotStripModelDeq.at(strip).getStop();
        const auto & rEvalSymbIndexVec = m_slotStripModelDeq.at(strip).getEvalIndexVec();

        for( size_t symb = 0; symb < rEvalSymbIndexVec.size(); ++symb )
            m_evalMathSymbs.at(strip).at(symb) = &m_slotStripModelDeq.at(strip).getSymbol( stop + rEvalSymbIndexVec.at(symb) );
    }
}


/************************************************************************
*    DESC:  Evaluate the line pays
************************************************************************/
void CSlotGroupModel::evaluateLinePays( const std::string & paytable )
{
    auto & rPayComboVec = m_rSlotMath.getPayComboSet( paytable );

    const auto & rPaylineSetVecVec = m_rPaylineSet.getLineData();

    // An vector of flags to indicate a payline has been awarded and is no longer checked
    std::vector<bool> awarded(rPaylineSetVecVec.size());

    for( auto & cboIter : rPayComboVec )
    {
        for( size_t payline = 0; payline < rPaylineSetVecVec.size(); ++payline )
        {
            // Continue if this payline has already been awarded
            if( awarded.at(payline) )
                continue;

            for( size_t strip = 0; strip < rPaylineSetVecVec.at(payline).size(); ++strip )
            {
                const int pos = rPaylineSetVecVec.at(payline).at(strip);

                auto pMathSymb = m_evalMathSymbs.at(strip).at(pos);

                // Break here if not a match to start checking the next payline
                if( !pMathSymb->isMatch( cboIter.getSymbol() ) )
                    break;

                // If we made it this far and the below condition is true, then it's a match
                if( strip == static_cast<size_t>(cboIter.getCount()-1) )
                {
                    awarded.at(payline) = true;

                    addLinePay( cboIter, payline, rPaylineSetVecVec );

                    break;
                }
            }
        }
    }
}


/************************************************************************
*    DESC:  Add line pay to slot result
************************************************************************/
void CSlotGroupModel::addLinePay(
    const CPayCombo & rPayCombo,
    const int payline,
    const std::vector<std::vector<int8_t>> & rPaylineSetVecVec )
{
    std::vector<CSymbPos> symbPos;
    symbPos.reserve(rPayCombo.getCount());

    // Copy over the symbol offsets for the number of strips effected by the win
    for( int i = 0; i < rPayCombo.getCount(); ++i )
        symbPos.emplace_back( i, rPaylineSetVecVec.at(payline).at(i) );

    // Add the win to the play result
    m_rPlayResult.addPay( NSlotDefs::EP_PAYLINE, rPayCombo, CBetMgr::Instance().getLineBet(), payline, symbPos );
}


/************************************************************************
*    DESC:  Evaluate the scatter pays
************************************************************************/
void CSlotGroupModel::evaluateScatters( const std::string & paytable )
{
    const std::vector<CPayCombo> & rPayComboVec = m_rSlotMath.getPayComboSet( paytable );

    // An array of each unique scatter symbol name in the paytable
    std::vector<std::string> symbVec;
    // A multi-dementional array to record the position of scatter symbols
    std::vector<std::vector<CSymbPos>> posVecVec;

	 // Record each unique scatter symbol and setup a multi-dementional array
    // to hold the list of positions of each scatter symbol found on the strips.
    for( auto & cboIter : rPayComboVec )
    {
        if( std::find( symbVec.begin(), symbVec.end(), cboIter.getSymbol() ) == symbVec.end() )
        {
            symbVec.push_back( cboIter.getSymbol() );
            posVecVec.emplace_back();
        }
    }

    // Go through the eval symbols to find these symbols
    for( size_t strip = 0; strip < m_evalMathSymbs.size(); ++strip )
    {
        for( size_t pos = 0; pos < m_evalMathSymbs.at(strip).size(); ++pos )
        {
            // Check if these scatter positions are allowed
            if( m_rPaylineSet.indexOfScaterData( strip, pos ) )
            {
                auto pMathSymb = m_evalMathSymbs.at(strip).at(pos);

                for( size_t symb = 0; symb < symbVec.size(); ++symb )
                {
                    // If the symbol is a match, record it's position
                    if( pMathSymb->isMatch( symbVec.at(symb) )  )
                    {
                        posVecVec.at(symb).emplace_back( strip, pos );
                    }
                }
            }
        }
    }

    // Go throught the combo, check for the symbol and see if any of the counts match
    for( auto & cboIter : rPayComboVec )
    {
        for( size_t symb = 0; symb < symbVec.size(); ++symb )
        {
            if( (cboIter.getSymbol() == symbVec.at(symb)) &&
                (posVecVec.at(symb).size() == static_cast<size_t>(cboIter.getCount())) )
            {
                // Add the win to the play result
                m_rPlayResult.addPay( NSlotDefs::EP_SCATTER, cboIter, CBetMgr::Instance().getTotalBet(), -1, posVecVec.at(symb) );
            }
        }
    }
}


/************************************************************************
*    DESC:  Get the math strip
************************************************************************/
const CSlotStripModel & CSlotGroupModel::getStrip( int index ) const
{
    return m_slotStripModelDeq.at(index);
}


/************************************************************************
*    DESC:  Get the number of math strips
************************************************************************/
size_t CSlotGroupModel::getCount() const
{
    return m_slotStripModelDeq.size();
}
