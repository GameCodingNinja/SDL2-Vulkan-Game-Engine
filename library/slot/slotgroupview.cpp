
/************************************************************************
*    FILE NAME:       slotgroupview.cpp
*
*    DESCRIPTION:     View base class for the slot group
************************************************************************/

// Physical component dependency
#include <slot/slotgroupview.h>

// Game lib dependencies
#include <slot/slotgroupmodel.h>
#include <slot/pay.h>
#include <slot/icycleresults.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <objectdata/objectdatamanager.h>
#include <sprite/sprite.h>
#include <common/ivisualcomponent.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSlotGroupView::CSlotGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel ) :
    m_spSlotGroupModel( rSlotGroupModel )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSlotGroupView::~CSlotGroupView()
{
}


/************************************************************************
*    DESC:  Create the view slot strips
************************************************************************/
void CSlotGroupView::create(
    const XMLNode & node,
    CSymbolSetView & rSymbolSetView,
    std::unique_ptr<iCycleResults> upCycleResults )
{
    // Set the cycle results unique pointer
    m_upCycleResults = std::move(upCycleResults);

    // Get the group name
    const std::string group = node.getAttribute( "group" );

    // Load the transform data from node
    loadTransFromNode( node.getChildNode( "translation" ) );

    // Get the cycle results text node and object data
    const XMLNode cycleResultsTxtNode = node.getChildNode( "cycleResultsText" );
    const std::string objectName = cycleResultsTxtNode.getAttribute( "objectName" );

    // Allocate the cycle results text
    m_upCycleResultsTxtSprite.reset( new CSprite( CObjectDataMgr::Instance().getData2D( group, objectName ) ) );
    m_upCycleResultsTxtSprite->getObject()->setVisible( false );
    m_upCycleResultsTxtSprite->getObject()->loadTransFromNode( cycleResultsTxtNode );

    // Load the font properties from XML node
    m_upCycleResultsTxtSprite->getVisualComponent()->loadFontPropFromNode( cycleResultsTxtNode );
}


/************************************************************************
*    DESC:  Set the cycle results text
************************************************************************/
void CSlotGroupView::setCycleResultText( bool visible, const CPay * pPay )
{
    m_upCycleResultsTxtSprite->getObject()->setVisible( visible );

    if( visible && (pPay != nullptr) )
    {
        std::string text;

        if( pPay->getPayType() == NSlotDefs::EP_PAYLINE )
        {
            text = boost::str( boost::format("Line %d Pays %d") % (pPay->getPayLine()+1) % pPay->getFinalAward() );
        }
        else
        {
            if( pPay->getBonusCode() > 0 )
                text = boost::str( boost::format("Bonus Pays %d") % pPay->getFinalAward() );
            else
                text = boost::str( boost::format("Scatter Pays %d") % pPay->getFinalAward() );
        }

        m_upCycleResultsTxtSprite->getVisualComponent()->createFontString(text);
    }
}


/************************************************************************
*    DESC:  Load the spin profile from XML node
************************************************************************/
void CSlotGroupView::loadSpinProfileFromNode( const XMLNode & node )
{
    // Sanity check
    if( !node.isAttributeSet("default") )
        throw NExcept::CCriticalException("Spin profile Creation Error!",
            boost::str( boost::format("Spin profile default attribute not set!\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    m_defaultSpinProfile = node.getAttribute( "default" );

    for( int i = 0; i < node.nChildNode(); ++i )
    {
        const XMLNode profileLstNode = node.getChildNode(i);

        // Get the spin profile id
        const std::string profileId = profileLstNode.getAttribute( "id" );

        // Create a new spin profile
        auto iter = m_spinProfileMapVec.emplace( profileId, std::vector<CSpinProfile>() );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Spin Profile Load Error!",
                boost::str( boost::format("Duplicate id (%s)!\n\n%s\nLine: %s")
                    % profileId % __FUNCTION__ % __LINE__ ));
        }

        // Pre-reserve the amount
        iter.first->second.reserve( profileLstNode.nChildNode() );

        for( int j = 0; j < profileLstNode.nChildNode(); ++j )
        {
            const XMLNode profileNode = profileLstNode.getChildNode(j);

            iter.first->second.emplace_back();
            iter.first->second.back().loadFromNode( profileNode );
        }
    }
}


/************************************************************************
*    DESC:  Update the reel group
************************************************************************/
void CSlotGroupView::update()
{
    if( m_upCycleResults )
        m_upCycleResults->update();
}


/************************************************************************
*    DESC:  Transform the reel group
************************************************************************/
void CSlotGroupView::transform()
{
    CObject2D::transform();

    if( m_upCycleResults )
        m_upCycleResults->transform( getMatrix(), wasWorldPosTranformed() );
}


/************************************************************************
*    DESC:  Do the render
************************************************************************/
/*void CSlotGroupView::render( const CMatrix & matrix )
{
    if( m_upCycleResults )
        m_upCycleResults->render( matrix );
}*/


/************************************************************************
*    DESC:  Activate the cycle results
************************************************************************/
void CSlotGroupView::activateCycleResults()
{
    if( m_upCycleResults )
        m_upCycleResults->activate();
}


/************************************************************************
*    DESC:  Deactivate the cycle results
************************************************************************/
void CSlotGroupView::deactivateCycleResults()
{
    if( m_upCycleResults )
        m_upCycleResults->deactivate();
}


/************************************************************************
*    DESC:  Stop the cycle results animation
************************************************************************/
void CSlotGroupView::startCycleResultsAnimation()
{
    if( m_upCycleResults )
        m_upCycleResults->startAnimation();
}


/************************************************************************
*    DESC:  Stop the cycle results animation
************************************************************************/
void CSlotGroupView::stopCycleResultsAnimation()
{
    if( m_upCycleResults )
        m_upCycleResults->stopAnimation();
}


/************************************************************************
*    DESC:  Is the cycle results active
************************************************************************/
bool CSlotGroupView::isCycleResultsActive()
{
    if( m_upCycleResults )
        return m_upCycleResults->isCycleResultsActive();

    return false;
}


/************************************************************************
*    DESC:  Is the cycle results animating
************************************************************************/
bool CSlotGroupView::isCycleResultsAnimating()
{
    if( m_upCycleResults )
        return m_upCycleResults->isAnimating();

    return false;
}


/************************************************************************
*    DESC:  Clear the cycle results symbols
************************************************************************/
void CSlotGroupView::clearCycleResultSymbs()
{
    // Empty by design. Clearing may not be required
}
