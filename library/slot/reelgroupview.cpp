
/************************************************************************
*    FILE NAME:       reelgroupview.cpp
*
*    DESCRIPTION:     View class for the reel group
************************************************************************/

// Physical component dependency
#include <slot/reelgroupview.h>

// Game lib dependencies
#include <slot/symbolsetview.h>
#include <slot/slotgroupmodel.h>
#include <slot/slotstripmodel.h>
#include <slot/icycleresults.h>
#include <slot/pay.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/deletefuncs.h>
#include <utilities/matrix.h>
#include <objectdata/objectdatamanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CReelGroupView::CReelGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel ) :
    CSlotGroupView( rSlotGroupModel )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CReelGroupView::~CReelGroupView()
{
    deleteCycleResultSymbs();
}


/************************************************************************
*    DESC:  Create the view slot strips
************************************************************************/
void CReelGroupView::create(
    const XMLNode & node,
    CSymbolSetView & rSymbolSetView,
    std::unique_ptr<iCycleResults> upCycleResults )
{
    CSlotGroupView::create( node, rSymbolSetView, std::move(upCycleResults) );

    // Get the group name
    const std::string group = node.getAttribute( "group" );

    // Sanity check
    const XMLNode reelstripLstNode = node.getChildNode( "reelstripList" );
    if( reelstripLstNode.isEmpty() )
        throw NExcept::CCriticalException("Reel Strip Creation Error!",
            boost::str( boost::format("Reel strip list node is empty!\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    // Check that the model and view reel strip count is the same
    if( static_cast<int>(m_spSlotGroupModel->getCount()) != reelstripLstNode.nChildNode() )
    {
        throw NExcept::CCriticalException("Reel Strip Creation Error!",
            boost::str( boost::format("Reelstrip model count does not match view count.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    }

    // Build the visible symbol set
    rSymbolSetView.buildSymbolSetView();

    // Create the view reel strips
    for( int reel = 0; reel < reelstripLstNode.nChildNode(); ++reel )
    {
        const XMLNode reelNode = reelstripLstNode.getChildNode(reel);

        // Add the model reel strip to the view reel strip
        m_reelStripViewDeq.emplace_back( m_spSlotGroupModel->getStrip(reel), rSymbolSetView, reel );
        m_reelStripViewDeq.back().create( reelNode, group );
    }

    // Setup the cycle result symbol vectors
    m_cycleResultSymbVec.resize( m_reelStripViewDeq.size() );
    for( size_t i = 0; i < m_reelStripViewDeq.size(); ++i )
        m_cycleResultSymbVec.at(i).resize( m_reelStripViewDeq.at(i).getVisibleSymbolCount() );
}


/************************************************************************
*    DESC:  Generate the cycle results symbols
************************************************************************/
void CReelGroupView::generateCycleResultSymbs()
{
    for( size_t reel = 0; reel < m_cycleResultSymbVec.size(); ++reel )
    {
        auto & evalSymbIndexVec = m_spSlotGroupModel->getStrip(reel).getEvalIndexVec();

        // This is an allocated pointer and will need to be freed
        for( size_t symb = 0; symb < evalSymbIndexVec.size(); ++symb )
            m_cycleResultSymbVec.at(reel).at(symb) = m_reelStripViewDeq.at(reel).getCycleResultSymbol( evalSymbIndexVec.at(symb) );
    }
}


/************************************************************************
*    DESC:  Get the cycle results symbols
************************************************************************/
std::vector<std::vector<CSymbol2d *>> & CReelGroupView::getCycleResultSymbs()
{
    return m_cycleResultSymbVec;
}


/************************************************************************
*    DESC:  Clear the cycle results symbols
************************************************************************/
void CReelGroupView::clearCycleResultSymbs()
{
    // This replaces the temporary symbols with the ones used for spinning
    for( auto & iter : m_reelStripViewDeq )
        iter.clearCycleResultSymbs();

    // Free the memory allocated for the cycle result symbols
    deleteCycleResultSymbs();
}


/************************************************************************
*    DESC:  Free the memory allocated for the cycle result symbols
************************************************************************/
void CReelGroupView::deleteCycleResultSymbs()
{
    for( size_t reel = 0; reel < m_cycleResultSymbVec.size(); ++reel )
        for( size_t symb = 0; symb < m_cycleResultSymbVec.at(reel).size(); ++symb )
            NDelFunc::Delete( m_cycleResultSymbVec.at(reel).at(symb) );
}


/************************************************************************
*    DESC:  Update the reel group
************************************************************************/
void CReelGroupView::update()
{
    CSlotGroupView::update();

    for( auto & iter : m_reelStripViewDeq )
        iter.update();
}


/************************************************************************
*    DESC:  Transform the reel group
************************************************************************/
void CReelGroupView::transform()
{
    CSlotGroupView::transform();

    for( auto & iter : m_reelStripViewDeq )
        iter.transform( getMatrix(), wasWorldPosTranformed() );

    m_upCycleResultsTxtSprite->transform( getMatrix(), wasWorldPosTranformed() );
}


/************************************************************************
*    DESC:  Do the render
************************************************************************/
void CReelGroupView::render( const CMatrix & matrix )
{
    for( auto & iter : m_reelStripViewDeq )
        iter.render( matrix );

    m_upCycleResultsTxtSprite->render( matrix );

    CSlotGroupView::render( matrix );
}


/************************************************************************
*    DESC:  Do the deferred render
************************************************************************/
void CReelGroupView::deferredRender( const CMatrix & matrix )
{
    for( auto & iter : m_reelStripViewDeq )
        iter.deferredRender( matrix );
}


/************************************************************************
*    DESC:  Start the reels spinning
************************************************************************/
void CReelGroupView::startSpin()
{
    if( isSpinState( NSlotDefs::ESS_STOPPED ) )
    {
        auto iter = m_spinProfileMapVec.find( m_defaultSpinProfile );
        if( iter == m_spinProfileMapVec.end() )
            throw NExcept::CCriticalException("Spin Profile Set Error!",
                boost::str( boost::format("Spin profile does not exist! (%s).\n\n%s\nLine: %s")
                    % m_defaultSpinProfile % __FUNCTION__ % __LINE__ ));

        // Init the reels with the spin profile
        for( size_t i = 0; i < m_reelStripViewDeq.size(); ++i )
            m_reelStripViewDeq.at(i).setSpinProfile( iter->second.at(i) );

        // Start the spin
        for( auto & iter : m_reelStripViewDeq )
            iter.startSpin();
    }
}


/************************************************************************
*    DESC:  Stop the reels spinning
************************************************************************/
void CReelGroupView::stopSpin()
{
    for( auto & iter : m_reelStripViewDeq )
        iter.stopSpin();
}


/************************************************************************
*    DESC:  Is the spin state
************************************************************************/
bool CReelGroupView::isSpinState( NSlotDefs::ESpinState state ) const
{
    bool result(true);

    for( auto & iter : m_reelStripViewDeq )
    {
        if( iter.getSpinState() != state )
        {
            result = false;
            break;
        }
    }

    return result;
}


/************************************************************************
*    DESC:  Do we allow the stop sounds?
************************************************************************/
void CReelGroupView::allowStopSounds( bool allow )
{
    for( auto & iter : m_reelStripViewDeq )
        iter.allowStopSounds( allow );
}
