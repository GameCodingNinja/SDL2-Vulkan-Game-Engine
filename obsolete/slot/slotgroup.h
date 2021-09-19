
/************************************************************************
*    FILE NAME:       slotgroup.h
*
*    DESCRIPTION:     Controller class for the slot group
************************************************************************/

#ifndef __slot_group_h__
#define __slot_group_h__

// Game lib dependencies
#include <slot/slotdefs.h>

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
class CSlotGroupModel;
class CSlotGroupView;
class CSymbolSetView;
class CMatrix;

class CSlotGroup
{
public:

    // Constructor
    CSlotGroup(
        std::shared_ptr<CSlotGroupModel> spSlotGroupModel,
        std::shared_ptr<CSlotGroupView> spSlotGroupView );

    // Destructor
    virtual ~CSlotGroup();
    
    // Get the slot group model
    CSlotGroupModel * getModel();
    
    // Get the slot group view
    CSlotGroupView * getView();
    
private:
    
    // Slot group model
    std::shared_ptr<CSlotGroupModel> m_spSlotGroupModel;
    
    // Slot group view
    std::shared_ptr<CSlotGroupView> m_spSlotGroupView;
};

#endif
