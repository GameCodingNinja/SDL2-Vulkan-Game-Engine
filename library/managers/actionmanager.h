
/************************************************************************
*    FILE NAME:       actionmanager.h
*
*    DESCRIPTION:     Class for handling action mapping
************************************************************************/

#pragma once

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <common/keycodeaction.h>
#include <common/defs.h>
#include <common/point.h>
#include <common/sensor.h>

// Boost lib dependencies
#include <boost/bimap.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// SDL lib dependencies
#include <SDL3/SDL.h>

class CActionMgr
{
public:

    // Get the instance of the singleton class
    static CActionMgr & Instance()
    {
        static CActionMgr actionMgr;
        return actionMgr;
    }

    // Load the action mappings from xml
    void loadActionFromXML( const std::string & filePath );

    // Poll the event
    const SDL_Event & pollEvent();

    // Was this an action
    bool wasAction( const SDL_Event & rEvent, const std::string & actionStr, EActionPress actionPress );
    EActionPress wasAction( const SDL_Event & rEvent, const std::string & actionStr );

    // What was the last devic
    bool wasLastDeviceGamepad();
    bool wasLastDeviceKeyboard();
    bool wasLastDeviceMouse();
    EDeviceId getLastDeviceUsed();

    // Reset the last used device
    void resetLastUsedDevice();

    // Get the mouse position
    const CPoint<float> & getMouseAbsolutePos() const;
    const CPoint<float> & getMouseRelativePos() const;
    
    // Get the last controller position
    const CPoint<float> & getControllerPosLeft() const;
    const CPoint<float> & getControllerPosRight() const;
    
    // Get the action/component strings for the give device id
    std::string getDeviceActionStr( 
        EDeviceId deviceId,
        const std::string & actionNameStr,
        bool & configurable );

    // Reset the action
    std::string resetAction(
        EDeviceId deviceId,
        const std::string & actionNameStr,
        int keyCode,
        bool & configurable );
    
    // Enable/Disable action handling
    void enableAction( bool value = true );
    
    // Is action handling allowed
    bool isAction();
    
    // Save the xml to file
    void saveToFile();
    
    // Reset all the key bindings to their default settings
    void resetKeyBindingsToDefault();
    
    // Queue the event
    void queueEvent( const SDL_Event & rEvent );
    
    // Clear the queue
    void clearQueue();

    // Is the queue empty
    bool isQueueEmpty();

    // Generic call for any event
    bool wasEvent( uint event );
    
    // Was this an action event
    bool wasActionEvent( const std::string & actionStr, EActionPress actionPress = EActionPress::DOWN );
    
    // Was this a game specific event
    bool wasGameEvent( uint type, int code );

    // Device specific key checks
    bool wasKeyboardEvent( const std::string & componentIdStr, EActionPress actionPress = EActionPress::DOWN );
    bool wasMouseBtnEvent( const std::string & componentIdStr, EActionPress actionPress = EActionPress::DOWN );
    bool wasGamepadBtnEvent( const std::string & componentIdStr, EActionPress actionPress = EActionPress::DOWN );
    
    // Enumerate events
    uint enumerateButtonEvents( uint & type, int & code, int & data, uint startIndex = 0 );
    uint enumerateMouseWheelEvents( uint & windowID, int & x, int & y, uint & direction, uint startIndex = 0 );
    uint enumerateTouchFingerEvents(
        uint & eventId, int64_t & touchId, int64_t & fingerId, float & x, float & y, float & dx, float & dy, float & pressure, uint startIndex = 0 );

private:

    // Constructor
    CActionMgr();

    // Destructor
    ~CActionMgr();
    
    // map types
    typedef boost::bimap< std::string, int > keyCodeMapType;
    typedef std::map< const std::string, CKeyCodeAction > actionMapType;

    // Load action data from xml node
    void loadActionFromNode(
        const XMLNode & node, 
        keyCodeMapType & keyCodeMap,
        actionMapType & actionMap );

    // Load the keyboard/mouse/gamepad mapping from node
    void loadAction();
    void loadKeyboardMappingFromNode( const XMLNode & node );
    void loadMouseMappingFromNode( const XMLNode & node );
    void loadGamepadMappingFromNode( const XMLNode & node );

    // Was this an action
    bool wasAction( 
        const int Id,
        const std::string & actionStr,
        const actionMapType & actionMap );
    
    // Get the component string for the device id
    int getComponentStr(
        const XMLNode & playerVisibleNode, 
        const std::string & actionNameStr, 
        std::string & componetIdStr, 
        bool & configurable );
    
    // Get the string associated with the key code
    bool getKeyCodeStr(
        keyCodeMapType & keyCodeMap,
        const int keyCode,
        std::string & componetIdStr );
    
    // Get the key code associated with the string
    int getKeyCode(
        keyCodeMapType & keyCodeMap,
        const std::string & componetIdStr );

private:

    enum
    {
        ANALOG1_UP = SDL_GAMEPAD_BUTTON_COUNT,
        ANALOG1_DOWN,
        ANALOG1_LEFT,
        ANALOG1_RIGHT,
        ANALOG2_UP,
        ANALOG2_DOWN,
        ANALOG2_LEFT,
        ANALOG2_RIGHT,
        ANALOG_STICK_MSG_MAX = 20000,
    };
    
    // SDL only defines 2 additional mouse buttons
    // Using this enum to define up to 8 additional mouse buttons
    enum
    {
        MOUSE_BTN_X1 = SDL_BUTTON_X1,
        MOUSE_BTN_X2,
        MOUSE_BTN_X3,
        MOUSE_BTN_X4,
        MOUSE_BTN_X5,
        MOUSE_BTN_X6,
        MOUSE_BTN_X7,
        MOUSE_BTN_X8
    };

    // Maps for parsing codes
    keyCodeMapType m_keyboardKeyCodeMap;
    keyCodeMapType m_mouseKeyCodeMap;
    keyCodeMapType m_gamepadKeyCodeMap;

    // Action maps
    actionMapType m_keyboardActionMap;
    actionMapType m_mouseActionMap;
    actionMapType m_gamepadActionMap;

    // xml node
    XMLNode m_mainNode;

    // Last device used
    EDeviceId m_lastDeviceUsed;

    // Mouse pos
    CPoint<float> m_mouseAbsolutePos;
    CPoint<float> m_mouseRelativePos;
    
    // Last controller pos
    CPoint<float> m_lastAnalogLeft;
    CPoint<float> m_lastAnalogRight;

    // Last Sensor values
    std::map<int, CSensor> m_sensorMap;

    // Flag to indicate analog state as button
    std::array<EActionPress,16> m_analogLXButtonStateAry;
    std::array<EActionPress,16> m_analogLYButtonStateAry;
    std::array<EActionPress,16> m_analogRXButtonStateAry;
    std::array<EActionPress,16> m_analogRYButtonStateAry;
    
    // The save file path
    std::string m_saveFilePath;
    
    // Flag to allow action handling
    bool m_allowAction;
    
    // Unbound key string
    const std::string UNBOUND_KEYCODE_STR_ID;
    const int UNBOUND_KEYCODE_ID;
    const int UNDEFINED_ACTION;
    
    // Que of event message
    std::vector<SDL_Event> m_eventQueue;

    // Queue index
    uint m_queueIndex = 0;

    // Null event
    SDL_Event m_nullEvent = {};
};

