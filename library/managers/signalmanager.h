
/************************************************************************
*    FILE NAME:       signalmanager.h
*
*    DESCRIPTION:     Class for handling messaging
************************************************************************/

#ifndef __message_manager_h__
#define __message_manager_h__

// Boost lib dependencies
#include <boost/signals2.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
class iControl;
class CMenu;
class CSprite;

class CSignalMgr
{
public:

    // Define the boost signals
    typedef boost::signals2::signal<void (iControl *)> SmartGuiControlSignal;
    typedef boost::signals2::signal<void (CMenu *)> SmartMenuSignal;
    typedef boost::signals2::signal<void (const std::string &, CSprite *)> AICreateSignal;
    typedef boost::signals2::signal<void ()> BasicFunction;

    // Get the instance of the singleton class
    static CSignalMgr & Instance()
    {
        static CSignalMgr signalMgr;
        return signalMgr;
    }

    ////////////////////////////////////////////////////////
    // Note: Boost::Bind does not produce unique types
    //       so function overloading does not work which
    //       which is why each connect has it's own function
    ////////////////////////////////////////////////////////

    // Connect/Disconnect to the smart gui signal
    void connect_smartGui( const SmartGuiControlSignal::slot_type & slot );
    void disconnect_smartGui();

    // Connect to the smart menu signal
    void connect_smartMenu( const SmartMenuSignal::slot_type & slot );
    void disconnect_smartMenu();
    
    // Connect to the Ai Sprite create signal
    void connect_aICreate( const AICreateSignal::slot_type & slot );
    void disconnect_aICreate();
    
    // Connect/Disconnect to the load signal
    //void connect_load( const BasicFunction::slot_type & slot );
    //void disconnect_load();
    
    // Connect/Disconnect to the resolution change signal
    void connect_resolutionChange( const BasicFunction::slot_type & slot );
    void disconnect_resolutionChange();

    // Broadcast smart gui control signal
    void broadcast( iControl * pControl );

    // Broadcast smart menu signal
    void broadcast( CMenu * pMenu );
    
    // Broadcast AI Sprite create signal
    void broadcast( const std::string & aiName, CSprite * pSprite );
    
    // Broadcast the load signal
    void broadcast_LoadSignal();
    
    // Broadcast the resolution change
    void broadcast_ResolutionChange();

private:

    // Constructor
    CSignalMgr();

    // Destructor
    ~CSignalMgr();

private:

    // Boost signals
    SmartGuiControlSignal m_smartGuiControlSignal;
    SmartMenuSignal m_smartMenuSignal;
    AICreateSignal m_aiCreateSignal;
    BasicFunction m_loadSignal;
    BasicFunction m_resolutionChange;
};

#endif
