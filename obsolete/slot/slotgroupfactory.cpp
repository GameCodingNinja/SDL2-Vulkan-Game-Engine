/************************************************************************
*    FILE NAME:       slotgroupfactory.cpp
*
*    DESCRIPTION:     Factory for slot group creation
************************************************************************/

// Physical component dependency
#include <slot/slotgroupfactory.h>

// Game lib dependencies
#include <slot/slotgroup.h>
#include <slot/reelgroupview.h>
#include <slot/wheelgroupview.h>
#include <slot/slotgroupmodel.h>
#include <slot/icycleresults.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/format.hpp>

namespace NSlotGroupFactory
{
    /************************************************************************
    *    DESC:  Load the control info from XML node
    ************************************************************************/
    std::unique_ptr<CSlotGroup> Create(
        const NSlotDefs::ESlotDevice slotDevice,
        const std::string & slotStripSetId,
        const std::string & paytableSetId,
        const CSlotMath & rSlotMath,
        const XMLNode & rViewCfgNode,
        const XMLNode & rViewSpinProfileCfgNode,
        CSymbolSetView & rSymbolSetView,
        CPlayResult & rPlayResult,
        std::unique_ptr<iCycleResults> upCycleResults )
    {
        // Create the group model
        auto spSlotGroupModel = std::make_shared<CSlotGroupModel>( rSlotMath, rPlayResult );
        spSlotGroupModel->create( slotStripSetId, paytableSetId );
        
        // Create the group view
        std::shared_ptr<CSlotGroupView> spSlotGroupView;
        if( slotDevice == NSlotDefs::ED_REEL )
            spSlotGroupView = std::make_shared<CReelGroupView>( spSlotGroupModel );

        else if( slotDevice == NSlotDefs::ED_WHEEL )
            spSlotGroupView = std::make_shared<CWheelGroupView>( spSlotGroupModel );

        else
            throw NExcept::CCriticalException("Slot Group Create Error!",
                boost::str( boost::format("Undefined slot device!\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));
        
        // Init the cycle results if we have one
        if( upCycleResults )
            upCycleResults->init( spSlotGroupView );
        
        // Create group view based on xml node and symbol set view
        spSlotGroupView->create( rViewCfgNode, rSymbolSetView, std::move(upCycleResults) );

        // Load the spin profile from XML node
        spSlotGroupView->loadSpinProfileFromNode( rViewSpinProfileCfgNode );
        
        // Create the slot group
        std::unique_ptr<CSlotGroup> upSlotGroup( new CSlotGroup(spSlotGroupModel, spSlotGroupView) );

        return upSlotGroup;

    }	// Create

}   // namespace NUIControlFactory
