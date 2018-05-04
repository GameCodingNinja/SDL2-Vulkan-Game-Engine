
/************************************************************************
*    FILE NAME:       paytableset.h
*
*    DESCRIPTION:     Paytable set math class
************************************************************************/

#ifndef __paytable_set_h__
#define __paytable_set_h__

// Game lib dependencies
#include <slot/slotdefs.h>

// Standard lib dependencies
#include <string>

class CPaytableSet
{
public:

    // Constructor
    CPaytableSet( const NSlotDefs::EPayType type, const std::string & id );
    
    // Get the paytable ID
    const std::string & getId() const;
    
    // Get the paytable type
    NSlotDefs::EPayType getType() const;
    
private:
    
    // Paytable type
    const NSlotDefs::EPayType m_type;
    
    // Paytable string id
    const std::string m_id;

};

#endif  // __paytable_set_h__


