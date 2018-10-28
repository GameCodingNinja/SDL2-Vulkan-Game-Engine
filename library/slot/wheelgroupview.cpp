
/************************************************************************
*    FILE NAME:       wheelgroupview.cpp
*
*    DESCRIPTION:     Class for wheel group view
************************************************************************/

// Physical component dependency
#include <slot/wheelgroupview.h>

// Game lib dependencies
#include <slot/symbolsetview.h>
#include <slot/slotgroupmodel.h>
#include <slot/slotstripmodel.h>
#include <slot/icycleresults.h>
#include <sprite/sprite.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CWheelGroupView::CWheelGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel ) :
    CSlotGroupView( rSlotGroupModel )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CWheelGroupView::~CWheelGroupView()
{
}


/************************************************************************
*    DESC:  Create the view slot strips
************************************************************************/
void CWheelGroupView::create(
    const XMLNode & node,
    CSymbolSetView & rSymbolSetView,
    std::unique_ptr<iCycleResults> upCycleResults )
{
    CSlotGroupView::create( node, rSymbolSetView, std::move(upCycleResults) );

    // Get the group name
    const std::string group = node.getAttribute( "group" );

    // Sanity check
    const XMLNode wheelLstNode = node.getChildNode( "wheelList" );
    if( wheelLstNode.isEmpty() )
        throw NExcept::CCriticalException("Wheel View Creation Error!",
            boost::str( boost::format("Wheel view node is empty!\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    // Create the view wheel
    for( int wheel = 0; wheel < wheelLstNode.nChildNode(); ++wheel )
    {
        const XMLNode wheelNode = wheelLstNode.getChildNode(wheel);

        // Add the model reel strip to the view reel strip
        m_wheelViewDeq.emplace_back( m_spSlotGroupModel->getStrip(wheel), rSymbolSetView, wheel );
        m_wheelViewDeq.back().create( wheelNode, group );
    }

    // Setup the cycle result symbol vectors
    m_cycleResultSymbVec.resize( m_wheelViewDeq.size() );
    for( size_t i = 0; i < m_wheelViewDeq.size(); ++i )
        m_cycleResultSymbVec.at(i).resize( m_wheelViewDeq.at(i).getVisibleSymbolCount() );

    // One time transform to set the non-spinning wheel sprites
    transform();
    for( auto & iter : m_wheelViewDeq )
        iter.preTransform();
}


/************************************************************************
*    DESC:  Generate the cycle results symbols
************************************************************************/
void CWheelGroupView::generateCycleResultSymbs()
{
    for( size_t wheel = 0; wheel < m_cycleResultSymbVec.size(); ++wheel )
    {
        auto & evalSymbIndexVec = m_spSlotGroupModel->getStrip(wheel).getEvalIndexVec();

        // Get the first symbol index array as the starting point
        // NOTE: It is assumed these values as listed in the math file are in order from lowest to highest
        int evalStartPoint = evalSymbIndexVec.at(0);

        // These pointers don't need to be freed
        for( size_t symb = 0; symb < m_wheelViewDeq.at(wheel).getVisibleSymbolCount(); ++symb )
            m_cycleResultSymbVec.at(wheel).at(symb) = &m_wheelViewDeq.at(wheel).getSymbol( evalStartPoint + symb );
    }
}


/************************************************************************
*    DESC:  Get the cycle results symbols
************************************************************************/
std::vector<std::vector<CSymbol2d *>> & CWheelGroupView::getCycleResultSymbs()
{
    return m_cycleResultSymbVec;
}


/************************************************************************
*    DESC:  Update the reel group
************************************************************************/
void CWheelGroupView::update()
{
    CSlotGroupView::update();

    for( auto & iter : m_wheelViewDeq )
        iter.update();
}


/************************************************************************
*    DESC:  Transform the reel group
************************************************************************/
void CWheelGroupView::transform()
{
    CSlotGroupView::transform();

    for( auto & iter : m_wheelViewDeq )
        iter.transform( getMatrix(), wasWorldPosTranformed() );

    m_upCycleResultsTxtSprite->getObject()->transform( getMatrix(), wasWorldPosTranformed() );
}


/************************************************************************
*    DESC:  Do the render
************************************************************************/
/*void CWheelGroupView::render( const CMatrix & matrix )
{
    for( auto & iter : m_wheelViewDeq )
        iter.render( matrix );

    m_upCycleResultsTxtSprite->render( matrix );

    CSlotGroupView::render( matrix );
}*/


/************************************************************************
*    DESC:  Do the deferred render
************************************************************************/
/*void CWheelGroupView::deferredRender( const CMatrix & matrix )
{
    for( auto & iter : m_wheelViewDeq )
        iter.render( matrix );
}*/


/************************************************************************
*    DESC:  Start the reels spinning
************************************************************************/
void CWheelGroupView::startSpin()
{
    if( isSpinState( NSlotDefs::ESS_STOPPED ) )
    {
        auto iter = m_spinProfileMapVec.find( m_defaultSpinProfile );
        if( iter == m_spinProfileMapVec.end() )
            throw NExcept::CCriticalException("Spin Profile Set Error!",
                boost::str( boost::format("Spin profile does not exist! (%s).\n\n%s\nLine: %s")
                    % m_defaultSpinProfile % __FUNCTION__ % __LINE__ ));

        // Init the wheels with the spin profile
        for( size_t i = 0; i < m_wheelViewDeq.size(); ++i )
            m_wheelViewDeq.at(i).setSpinProfile( iter->second.at(i) );

        // Start the spin
        for( auto & iter : m_wheelViewDeq )
            iter.startSpin();
    }
}


/************************************************************************
*    DESC:  Stop the reels spinning
************************************************************************/
void CWheelGroupView::stopSpin()
{
    for( auto & iter : m_wheelViewDeq )
        iter.stopSpin();
}


/************************************************************************
*    DESC:  Is the spin state
************************************************************************/
bool CWheelGroupView::isSpinState( NSlotDefs::ESpinState state ) const
{
    bool result(true);

    for( auto & iter : m_wheelViewDeq )
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
void CWheelGroupView::allowStopSounds( bool allow )
{
    for( auto & iter : m_wheelViewDeq )
        iter.allowStopSounds( allow );
}
