
/************************************************************************
*    FILE NAME:       slotgroup.cpp
*
*    DESCRIPTION:     Controller class for the slot group
************************************************************************/

// Physical component dependency
#include <slot/slotgroup.h>

// Game lib dependencies
#include <slot/slotmathmanager.h>
#include <slot/playresult.h>
#include <slot/symbol2d.h>
#include <slot/playresult.h>
#include <slot/slotgroupmodel.h>
#include <slot/reelgroupview.h>
#include <slot/wheelgroupview.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <iostream>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSlotGroup::CSlotGroup(
    std::shared_ptr<CSlotGroupModel> spSlotGroupModel,
    std::shared_ptr<CSlotGroupView> spSlotGroupView ) :
        m_spSlotGroupModel( spSlotGroupModel ),
        m_spSlotGroupView( spSlotGroupView )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSlotGroup::~CSlotGroup()
{
}


/************************************************************************
*    DESC:  Get the slot group model
************************************************************************/
CSlotGroupModel * CSlotGroup::getModel()
{
    return m_spSlotGroupModel.get();
}


/************************************************************************
*    DESC:  Get the slot group view
************************************************************************/
CSlotGroupView * CSlotGroup::getView()
{
    return m_spSlotGroupView.get();
}
