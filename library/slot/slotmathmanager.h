
/************************************************************************
*    FILE NAME:       slotmathmanager.h
*
*    DESCRIPTION:     Singlton for managing slot math data
************************************************************************/

#ifndef __slot_math_manager_h__
#define __slot_math_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <slot/slotmath.h>
#include <slot/paylineset.h>

// Standard lib dependencies
#include <string>
#include <map>

class CSlotMathMgr : public CManagerBase
{
public:
    
    // Get the instance of the singleton class
    static CSlotMathMgr & Instance()
    {
        static CSlotMathMgr slotMathMgr;
        return slotMathMgr;
    }
    
    // Load all of the math data from a specific group
    void loadGroup( const std::string & group );
    
    // Unload all of the math data from a specific group
    void freeGroup( const std::string & group );
    
    // Get the slot math
    const CSlotMath & getSlotMath( const std::string & group, const std::string & id ) const;
    
    // Load the payline configuration from XML file
    void loadPaylineSetFromFile( const std::string & filePath );
    
    // Get the payline set
    const CPaylineSet & getPaylineSet( const std::string & id ) const;
    
    // Free the payline set
    void freePaylineSet();
    
    // Clear out all the data
    void clear();
    
private:

    // Constructor
    CSlotMathMgr();

    // Destructor
    virtual ~CSlotMathMgr();
    
    // Load all math data from an xml
    void load( const std::string & group, const std::string & filePath );
    
private:
    
    // Map of a map of all the reel group data
    std::map<const std::string, std::map<const std::string, CSlotMath> > m_slotMathMapMap;
    
    // Payline description
    std::map<const std::string, CPaylineSet > m_paylineSetMap;

};

#endif  // __math_data_manager_h__


