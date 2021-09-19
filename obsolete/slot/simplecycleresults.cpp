
/************************************************************************
*    FILE NAME:       simplecycleresults.cpp
*
*    DESCRIPTION:     Simple cycle results implementation
************************************************************************/

// Physical component dependency
#include <slot/simplecycleresults.h>

// Game lib dependencies
#include <slot/slotgroupview.h>
#include <slot/playresult.h>
#include <slot/symbol2d.h>
#include <sprite/sprite.h>
#include <common/ivisualcomponent.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSimpleCycleresults::CSimpleCycleresults( CPlayResult * pPlayResult ) :
    iCycleResults( pPlayResult )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSimpleCycleresults::~CSimpleCycleresults()
{
}


/***************************************************************************
*    DESC:  Do some inits
****************************************************************************/
void CSimpleCycleresults::init( std::shared_ptr<CSlotGroupView> & spSlotGroupView )
{
    m_spSlotGroupView = spSlotGroupView;
}


/***************************************************************************
*    DESC:  Start the cycle results
****************************************************************************/
void CSimpleCycleresults::activate()
{
    if( m_pPlayResult->getPayCount() > 0 )
    {
        iCycleResults::activate();

        m_spSlotGroupView->generateCycleResultSymbs();
    }
}


/***************************************************************************
*    DESC:  Deactivate the cycle results
****************************************************************************/
void CSimpleCycleresults::deactivate()
{
    if( m_cycleResultsActive )
    {
        iCycleResults::deactivate();

        m_spSlotGroupView->clearCycleResultSymbs();
    }
}


/***************************************************************************
*    DESC:  Start the cycle results animation
****************************************************************************/
void CSimpleCycleresults::startAnimation()
{
    if( m_cycleResultsActive )
    {
        m_cycleResultsTimer.set( 1000 );

        auto & rCycleResultSymb = m_spSlotGroupView->getCycleResultSymbs();

        auto & rPay = m_pPlayResult->getPay( m_cyclePayCounter );
        auto & rSymbPos = rPay.getSymbPos();
        m_cyclePayCounter = (m_cyclePayCounter + 1) % m_pPlayResult->getPayCount();

        // Set them all to a low alphs
        for( auto & iter : rCycleResultSymb )
            for( auto sympIter : iter )
                sympIter->getSprite().getVisualComponent()->setAlpha( 0.20f );

        // Set only the winners to default color
        for( auto & iter : rSymbPos )
            rCycleResultSymb.at(iter.getReel()).at(iter.getPos())->getSprite().getVisualComponent()->setDefaultColor();

        m_spSlotGroupView->setCycleResultText( true, &rPay );
    }
}


/***************************************************************************
*    DESC:  Stop the cycle results animation
****************************************************************************/
void CSimpleCycleresults::stopAnimation()
{
    if( m_cycleResultsActive )
    {
        m_cycleResultsTimer.setExpired();

        auto & rCycleResultSymb = m_spSlotGroupView->getCycleResultSymbs();

        // Set it all back to normal
        for( auto & iter : rCycleResultSymb )
            for( auto sympIter : iter )
                sympIter->getSprite().getVisualComponent()->setDefaultColor();

        m_spSlotGroupView->setCycleResultText( false );
    }
}


/***************************************************************************
*    DESC:  Are we still animating
****************************************************************************/
bool CSimpleCycleresults::isAnimating()
{
    return !m_cycleResultsTimer.expired();
}
