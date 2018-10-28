/************************************************************************
*    FILE NAME:       slotgroupfactory.h
*
*    DESCRIPTION:     Factory for slot group creation
************************************************************************/

#ifndef __slot_group_factory_h__
#define __slot_group_factory_h__

// Game lib dependencies
#include <slot/slotdefs.h>

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
class CSlotMath;
class CMatrix;
class CSymbolSetView;
class CSlotGroup;
class CPlayResult;
class iCycleResults;
struct XMLNode;

namespace NSlotGroupFactory
{
    // Create the control
    std::unique_ptr<CSlotGroup> Create(
        const NSlotDefs::ESlotDevice slotDevice,
        const std::string & slotStripSetId,
        const std::string & paytableSetId,
        const CSlotMath & rSlotMath,
        const XMLNode & rViewCfgNode,
        const XMLNode & rViewSpinProfileCfgNode,
        CSymbolSetView & rSymbolSetView,
        CPlayResult & rPlayResult,
        std::unique_ptr<iCycleResults> upCycleResults );
};

#endif
