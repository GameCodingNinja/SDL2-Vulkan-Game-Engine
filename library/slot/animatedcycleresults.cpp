
/************************************************************************
*    FILE NAME:       animatedcycleresults.cpp
*
*    DESCRIPTION:     Animated cycle results implementation
************************************************************************/

// Physical component dependency
#include <slot/animatedcycleresults.h>

// Game lib dependencies
#include <slot/slotgroupview.h>
#include <slot/playresult.h>
#include <slot/symbol2d.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CAnimatedCycleResults::CAnimatedCycleResults( CPlayResult * pPlayResult ) :
    iCycleResults( pPlayResult )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CAnimatedCycleResults::~CAnimatedCycleResults()
{
}


/***************************************************************************
*    DESC:  Do some inits
****************************************************************************/
void CAnimatedCycleResults::init( std::shared_ptr<CSlotGroupView> & spSlotGroupView )
{
    m_spSlotGroupView = spSlotGroupView;
}


/***************************************************************************
*    DESC:  Update the cycle results
****************************************************************************/
void CAnimatedCycleResults::update()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->getCycleResultSymbs();
        auto & rPay = m_pPlayResult->getPay( m_curPayIndex );
        auto & rSymbPos = rPay.getSymbPos();

        for( auto & iter : rSymbPos )
            rCycleResultSymb.at(iter.getReel()).at(iter.getPos())->getSprite().getScriptComponent().update();
    }
}


/************************************************************************
*    DESC:  Transform
************************************************************************/
void CAnimatedCycleResults::transform( const CMatrix & matrix, bool tranformWorldPos )
{
}


/***************************************************************************
*    DESC:  Activate the cycle results
****************************************************************************/
void CAnimatedCycleResults::activate()
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
void CAnimatedCycleResults::deactivate()
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
void CAnimatedCycleResults::startAnimation()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->getCycleResultSymbs();

        m_curPayIndex = m_cyclePayCounter;
        m_cyclePayCounter = (m_cyclePayCounter + 1) % m_pPlayResult->getPayCount();

        auto & rPay = m_pPlayResult->getPay( m_curPayIndex );
        auto & rSymbPos = rPay.getSymbPos();


        // Set them all the alphs low
        for( auto & iter : rCycleResultSymb )
        {
            for( auto sympIter : iter )
            {
                sympIter->getSprite().getVisualComponent().setAlpha( 0.20f );
                sympIter->getSprite().getScriptComponent().resetAndRecycle();
                sympIter->setDeferredRender( false );
            }
        }

        // Set only the winners to default color and set the script function
        for( auto & iter : rSymbPos )
        {
            auto symbol = rCycleResultSymb.at(iter.getReel()).at(iter.getPos());
            symbol->getSprite().getVisualComponent().setDefaultColor();
            symbol->getSprite().prepareFuncId( "animate" );
            symbol->setDeferredRender( true );
        }

        m_spSlotGroupView->setCycleResultText( true, &rPay );
    }
}


/***************************************************************************
*    DESC:  Stop the cycle results animation
****************************************************************************/
void CAnimatedCycleResults::stopAnimation()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->getCycleResultSymbs();

        // Set it all back to normal
        for( auto & iter : rCycleResultSymb )
        {
            for( auto sympIter : iter )
            {
                sympIter->getSprite().getVisualComponent().setDefaultColor();
                sympIter->getSprite().getScriptComponent().resetAndRecycle();
                sympIter->setDeferredRender( false );
            }
        }

        m_spSlotGroupView->setCycleResultText( false );
    }
}


/***************************************************************************
*    DESC:  Are we still animating
****************************************************************************/
bool CAnimatedCycleResults::isAnimating()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->getCycleResultSymbs();
        auto & rPay = m_pPlayResult->getPay( m_curPayIndex );
        auto & rSymbPos = rPay.getSymbPos();

        for( auto & iter : rSymbPos )
        {
            auto symbol = rCycleResultSymb.at(iter.getReel()).at(iter.getPos());
            if( symbol->getSprite().getScriptComponent().isActive() )
                return true;
        }
    }

    return false;
}


/************************************************************************
*    DESC:  Do the render
************************************************************************/
void CAnimatedCycleResults::render( const CMatrix & matrix )
{
    if( m_cycleResultsActive )
        m_spSlotGroupView->deferredRender( matrix );
}
