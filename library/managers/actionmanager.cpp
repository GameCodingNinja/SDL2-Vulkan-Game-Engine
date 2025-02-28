
/************************************************************************
*    FILE NAME:       actionmanager.cpp
*
*    DESCRIPTION:     Class for handling action mapping
************************************************************************/

// Physical component dependency
#include <managers/actionmanager.h>

// Game lib dependencies
#include <utilities/genfunc.h>
#include <gui/uidefs.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CActionMgr::CActionMgr() :
    m_lastDeviceUsed(EDeviceId::_NULL_),
    m_allowAction(true),
    UNBOUND_KEYCODE_STR_ID("---"),
    UNBOUND_KEYCODE_ID(-1),
    UNDEFINED_ACTION(-1)
{
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type(UNBOUND_KEYCODE_STR_ID, UNBOUND_KEYCODE_ID) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("RETURN",  SDLK_RETURN) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ESCAPE",  SDLK_ESCAPE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("AC_BACK",  SDLK_AC_BACK) ); // Android back button

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ARROW UP",    SDLK_UP) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ARROW DOWN",  SDLK_DOWN) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ARROW LEFT",  SDLK_LEFT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ARROW RIGHT", SDLK_RIGHT) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("A",  SDLK_A) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("B",  SDLK_B) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("C",  SDLK_C) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("D",  SDLK_D) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("E",  SDLK_E) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F",  SDLK_F) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("G",  SDLK_G) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("H",  SDLK_H) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("I",  SDLK_I) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("J",  SDLK_J) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("K",  SDLK_K) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("L",  SDLK_L) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("M",  SDLK_M) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("N",  SDLK_N) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("O",  SDLK_O) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("P",  SDLK_P) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("Q",  SDLK_Q) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("R",  SDLK_R) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("S",  SDLK_S) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("T",  SDLK_T) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("U",  SDLK_U) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("V",  SDLK_V) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("W",  SDLK_W) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("X",  SDLK_X) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("Y",  SDLK_Y) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("Z",  SDLK_Z) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F1",  SDLK_F1) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F2",  SDLK_F2) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F3",  SDLK_F3) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F4",  SDLK_F4) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F5",  SDLK_F5) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F6",  SDLK_F6) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F7",  SDLK_F7) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F8",  SDLK_F8) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F9",  SDLK_F9) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F10", SDLK_F10) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F11", SDLK_F11) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F12", SDLK_F12) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("0",  SDLK_0) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("1",  SDLK_1) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("2",  SDLK_2) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("3",  SDLK_3) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("4",  SDLK_4) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("5",  SDLK_5) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("6",  SDLK_6) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("7",  SDLK_7) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("8",  SDLK_8) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("9",  SDLK_9) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 1", SDLK_KP_1) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 2", SDLK_KP_2) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 3", SDLK_KP_3) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 4", SDLK_KP_4) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 5", SDLK_KP_5) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 6", SDLK_KP_6) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 7", SDLK_KP_7) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 8", SDLK_KP_8) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 9", SDLK_KP_9) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 0", SDLK_KP_0) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUM LOCK",     SDLK_NUMLOCKCLEAR) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD /",     SDLK_KP_DIVIDE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD *",     SDLK_KP_MULTIPLY) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD -",     SDLK_KP_MINUS) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD +",     SDLK_KP_PLUS) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD ENTER", SDLK_KP_ENTER) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD .",     SDLK_KP_PERIOD) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("LEFT CTRL",   SDLK_LCTRL) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT CTRL",  SDLK_RCTRL) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("LEFT SHIFT",  SDLK_LSHIFT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT SHIFT", SDLK_RSHIFT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("LEFT ALT",    SDLK_LALT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT ALT",   SDLK_RALT) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("PRINT SCREEN", SDLK_PRINTSCREEN) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("SCROLL LOCK",  SDLK_SCROLLLOCK) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("PAUSE",        SDLK_PAUSE) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("END",       SDLK_END) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("INSERT",    SDLK_INSERT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("DELETE",    SDLK_DELETE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("HOME",      SDLK_HOME) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("PAGE UP",   SDLK_PAGEUP) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("PAGE DOWN", SDLK_PAGEDOWN) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("BACKSPACE", SDLK_BACKSPACE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("TAB",       SDLK_TAB) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("SPACE",     SDLK_SPACE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type(",",         SDLK_COMMA) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("-",         SDLK_MINUS) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type(".",         SDLK_PERIOD) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("/",         SDLK_SLASH) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("=",         SDLK_EQUALS) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type(";",  SDLK_SEMICOLON) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("[",  SDLK_LEFTBRACKET) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("\\", SDLK_BACKSLASH) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("]",  SDLK_RIGHTBRACKET) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("`",  SDLK_GRAVE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("'",  SDLK_APOSTROPHE) );

    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type(UNBOUND_KEYCODE_STR_ID, UNBOUND_KEYCODE_ID) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("LEFT MOUSE",           SDL_BUTTON_LEFT) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MIDDLE MOUSE",         SDL_BUTTON_MIDDLE) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT MOUSE",          SDL_BUTTON_RIGHT) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 1",              MOUSE_BTN_X1) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 2",              MOUSE_BTN_X2) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 3",              MOUSE_BTN_X3) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 4",              MOUSE_BTN_X4) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 5",              MOUSE_BTN_X5) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 6",              MOUSE_BTN_X6) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 7",              MOUSE_BTN_X7) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 8",              MOUSE_BTN_X8) );

    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type(UNBOUND_KEYCODE_STR_ID, UNBOUND_KEYCODE_ID) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("A",                    SDL_GAMEPAD_BUTTON_SOUTH) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("B",                    SDL_GAMEPAD_BUTTON_EAST) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("X",                    SDL_GAMEPAD_BUTTON_WEST) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("Y",                    SDL_GAMEPAD_BUTTON_NORTH) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("BACK",                 SDL_GAMEPAD_BUTTON_BACK) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("GUIDE",                SDL_GAMEPAD_BUTTON_GUIDE) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("START",                SDL_GAMEPAD_BUTTON_START) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("UP",                   SDL_GAMEPAD_BUTTON_DPAD_UP) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT",                SDL_GAMEPAD_BUTTON_DPAD_RIGHT) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("DOWN",                 SDL_GAMEPAD_BUTTON_DPAD_DOWN) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("LEFT",                 SDL_GAMEPAD_BUTTON_DPAD_LEFT) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK",              SDL_GAMEPAD_BUTTON_LEFT_STICK) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK",              SDL_GAMEPAD_BUTTON_RIGHT_STICK) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L BUMPER",             SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R BUMPER",             SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER) );

    // Key codes to use analog sticks as buttons
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK UP",    ANALOG1_UP) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK DOWN",  ANALOG1_DOWN) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK LEFT",  ANALOG1_LEFT) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK RIGHT", ANALOG1_RIGHT) );

    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK UP",    ANALOG2_UP) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK DOWN",  ANALOG2_DOWN) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK LEFT",  ANALOG2_LEFT) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK RIGHT", ANALOG2_RIGHT) );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CActionMgr::~CActionMgr()
{
}


/************************************************************************
*    DESC:  Load the action mappings from xml
*
*    param: std::string filePath - path to file
************************************************************************/
void CActionMgr::loadActionFromXML( const std::string & filePath )
{
    m_saveFilePath = filePath;
    m_mainNode = XMLNode::openFileHelper( filePath.c_str(), "controllerMapping" );

    // Load the keyboard/mouse/gamepad mapping
    loadAction();
}


/************************************************************************
*    DESC:  Load the keyboard/mouse/gamepad mapping
************************************************************************/
void CActionMgr::loadAction()
{
    // Load the keyboard/mouse/gamepad mapping
    loadKeyboardMappingFromNode( m_mainNode.getChildNode( "keyboardMapping" ) );
    loadMouseMappingFromNode( m_mainNode.getChildNode( "mouseMapping" ) );
    loadGamepadMappingFromNode( m_mainNode.getChildNode( "gamepadMapping" ) );
}


/************************************************************************
*    DESC:  Load the keyboard mapping from node
************************************************************************/
void CActionMgr::loadKeyboardMappingFromNode( const XMLNode & node )
{
    // Load the player hidden controls
    loadActionFromNode( node.getChildNode("playerHidden"), m_keyboardKeyCodeMap, m_keyboardActionMap );

    // Load the player visible controls
    loadActionFromNode( node.getChildNode("playerVisible"), m_keyboardKeyCodeMap, m_keyboardActionMap );
}


/************************************************************************
*    DESC:  Load the mouse mapping from node
************************************************************************/
void CActionMgr::loadMouseMappingFromNode( const XMLNode & node )
{
    // Load the player hidden controls
    loadActionFromNode( node.getChildNode("playerHidden"), m_mouseKeyCodeMap, m_mouseActionMap );

    // Load the player visible controls
    loadActionFromNode( node.getChildNode("playerVisible"), m_mouseKeyCodeMap, m_mouseActionMap );
}


/************************************************************************
*    DESC:  Load the gamepad mapping from node
************************************************************************/
void CActionMgr::loadGamepadMappingFromNode( const XMLNode & node )
{
    // Load the player hidden controls
    loadActionFromNode( node.getChildNode("playerHidden"), m_gamepadKeyCodeMap, m_gamepadActionMap );

    // Load the player visible controls
    loadActionFromNode( node.getChildNode("playerVisible"), m_gamepadKeyCodeMap, m_gamepadActionMap );
}


/************************************************************************
*    DESC:  Load action data from xml node
************************************************************************/
void CActionMgr::loadActionFromNode(
    const XMLNode & node,
    keyCodeMapType & keyCodeMap,
    actionMapType & actionMap )
{
    if( !node.isEmpty() )
    {
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            XMLNode actionNode = node.getChildNode( "actionMap", i );

            // See if we can find the string that represents the key code id
            std::string componentIdStr = actionNode.getAttribute( "componetId" );
            const auto keyCodeIter = keyCodeMap.left.find( componentIdStr );

            // Add it in if we found it
            if( keyCodeIter != keyCodeMap.left.end() )
            {
                std::string actionStr = actionNode.getAttribute( "action" );

                // See if the controller action string has already been added
                auto iter = actionMap.find( actionStr );

                if( iter != actionMap.end() )
                {
                    // If it's found, add another id to this map
                    iter->second.setId( keyCodeIter->second );
                }
                else
                {
                    // Add new action to the map
                    actionMap.emplace( actionStr, keyCodeIter->second );
                }
            }
        }
    }
}


/************************************************************************
*    DESC:  Poll the event
************************************************************************/
const SDL_Event & CActionMgr::pollEvent()
{
    if( m_queueIndex < m_eventQueue.size() )
        return m_eventQueue[m_queueIndex++];

    // Reset the counter if we hit the end of the queue
    m_queueIndex = 0;

    return m_nullEvent;
}


/************************************************************************
*    DESC:  Was this an action
************************************************************************/
bool CActionMgr::wasAction( const SDL_Event & rEvent, const std::string & actionStr, EActionPress actionPress )
{
    if( wasAction( rEvent, actionStr ) == actionPress )
        return true;

    return false;
}


/************************************************************************
*    DESC:  Was this an action
************************************************************************/
EActionPress CActionMgr::wasAction( const SDL_Event & rEvent, const std::string & actionStr )
{
    EActionPress result( EActionPress::IDLE);

    if( m_allowAction )
    {
        if( (rEvent.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) || (rEvent.type == SDL_EVENT_GAMEPAD_BUTTON_UP) )
        {
            m_lastDeviceUsed = EDeviceId::GAMEPAD;

            if( wasAction( rEvent.gbutton.button, actionStr, m_gamepadActionMap ) )
            {
                result = EActionPress::UP;

                if( rEvent.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN )
                {
                    result = EActionPress::DOWN;
                }
            }
        }
        else if( ((rEvent.type == SDL_EVENT_KEY_DOWN) || (rEvent.type == SDL_EVENT_KEY_UP)) && (rEvent.key.repeat == 0) )
        {
            m_lastDeviceUsed = EDeviceId::KEYBOARD;

            if( wasAction( rEvent.key.key, actionStr, m_keyboardActionMap ) )
            {
                result = EActionPress::UP;

                if( rEvent.type == SDL_EVENT_KEY_DOWN )
                {
                    result = EActionPress::DOWN;
                }
            }
        }
        else if( (rEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN) || (rEvent.type == SDL_EVENT_MOUSE_BUTTON_UP) )
        {
            m_lastDeviceUsed = EDeviceId::MOUSE;

            if( wasAction( rEvent.button.button, actionStr, m_mouseActionMap ) )
            {
                result = EActionPress::UP;

                m_mouseAbsolutePos.x = rEvent.button.x;
                m_mouseAbsolutePos.y = rEvent.button.y;

                if( rEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN )
                {
                    result = EActionPress::DOWN;
                }
            }
        }
        else if( rEvent.type == SDL_EVENT_GAMEPAD_AXIS_MOTION )
        {
            if( rEvent.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX )
            {
                if( m_analogLXButtonStateAry[rEvent.gaxis.which] == EActionPress::IDLE )
                {
                    //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left X: %d") % ((int)rEvent.caxis.value) ) );

                    if( (rEvent.gaxis.value < -ANALOG_STICK_MSG_MAX) && wasAction( ANALOG1_LEFT, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::DOWN;

                    else if( (rEvent.gaxis.value > ANALOG_STICK_MSG_MAX) && wasAction( ANALOG1_RIGHT, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::DOWN;
                }
                else if( m_analogLXButtonStateAry[rEvent.gaxis.which] == EActionPress::DOWN )
                {
                    //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left X: %d") % ((int)rEvent.caxis.value) ) );

                    if( (rEvent.gaxis.value > -ANALOG_STICK_MSG_MAX) && (rEvent.gaxis.value < ANALOG_STICK_MSG_MAX) && wasAction( ANALOG1_LEFT, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::UP;

                    else if( (rEvent.gaxis.value < ANALOG_STICK_MSG_MAX) && (rEvent.gaxis.value > -ANALOG_STICK_MSG_MAX) && wasAction( ANALOG1_RIGHT, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::UP;
                }

                if( result != EActionPress::IDLE )
                {
                    if( m_analogLXButtonStateAry[rEvent.gaxis.which] == EActionPress::IDLE && result == EActionPress::DOWN )
                    {
                        //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left Y: %d") % ((int)rEvent.caxis.value) ) );
                        m_analogLXButtonStateAry[rEvent.gaxis.which] = EActionPress::DOWN;
                    }
                    else if( m_analogLXButtonStateAry[rEvent.gaxis.which] == EActionPress::DOWN && result == EActionPress::UP )
                    {
                        //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left Y: %d") % ((int)rEvent.caxis.value) ) );
                        m_analogLXButtonStateAry[rEvent.gaxis.which] = EActionPress::IDLE;
                    }
                    else
                        result = EActionPress::IDLE;

                    m_lastDeviceUsed = EDeviceId::GAMEPAD;
                }
            }
            else if( rEvent.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY )
            {
                if( m_analogLYButtonStateAry[rEvent.gaxis.which] == EActionPress::IDLE )
                {
                    if( (rEvent.gaxis.value < -ANALOG_STICK_MSG_MAX) && wasAction( ANALOG1_UP, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::DOWN;

                    else if( (rEvent.gaxis.value > ANALOG_STICK_MSG_MAX) && wasAction( ANALOG1_DOWN, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::DOWN;
                }
                else if( m_analogLYButtonStateAry[rEvent.gaxis.which] == EActionPress::DOWN )
                {
                    if( (rEvent.gaxis.value > -ANALOG_STICK_MSG_MAX) && (rEvent.gaxis.value < ANALOG_STICK_MSG_MAX) && wasAction( ANALOG1_UP, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::UP;

                    else if( (rEvent.gaxis.value < ANALOG_STICK_MSG_MAX) && (rEvent.gaxis.value > -ANALOG_STICK_MSG_MAX) && wasAction( ANALOG1_DOWN, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::UP;
                }

                if( result != EActionPress::IDLE )
                {
                    if( m_analogLYButtonStateAry[rEvent.gaxis.which] == EActionPress::IDLE && result == EActionPress::DOWN )
                    {
                        //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left Y: %d") % ((int)rEvent.caxis.value) ) );
                        m_analogLYButtonStateAry[rEvent.gaxis.which] = EActionPress::DOWN;
                    }
                    else if( m_analogLYButtonStateAry[rEvent.gaxis.which] == EActionPress::DOWN && result == EActionPress::UP )
                    {
                        //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left Y: %d") % ((int)rEvent.caxis.value) ) );
                        m_analogLYButtonStateAry[rEvent.gaxis.which] = EActionPress::IDLE;
                    }
                    else
                        result = EActionPress::IDLE;

                    m_lastDeviceUsed = EDeviceId::GAMEPAD;
                }
            }
            else if( rEvent.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX )
            {
                if( m_analogRXButtonStateAry[rEvent.gaxis.which] == EActionPress::IDLE )
                {
                    if( (rEvent.gaxis.value < -ANALOG_STICK_MSG_MAX) && wasAction( ANALOG2_LEFT, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::DOWN;

                    else if( (rEvent.gaxis.value > ANALOG_STICK_MSG_MAX) && wasAction( ANALOG2_RIGHT, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::DOWN;
                }
                else if( m_analogRXButtonStateAry[rEvent.gaxis.which] == EActionPress::DOWN )
                {
                    if( (rEvent.gaxis.value > -ANALOG_STICK_MSG_MAX) && (rEvent.gaxis.value < ANALOG_STICK_MSG_MAX) && wasAction( ANALOG2_LEFT, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::UP;

                    else if( (rEvent.gaxis.value < ANALOG_STICK_MSG_MAX) && (rEvent.gaxis.value > -ANALOG_STICK_MSG_MAX) && wasAction( ANALOG2_RIGHT, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::UP;
                }

                if( result != EActionPress::IDLE )
                {
                    if( m_analogRXButtonStateAry[rEvent.gaxis.which] == EActionPress::IDLE && result == EActionPress::DOWN )
                    {
                        //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left Y: %d") % ((int)rEvent.caxis.value) ) );
                        m_analogRXButtonStateAry[rEvent.gaxis.which] = EActionPress::DOWN;
                    }
                    else if( m_analogRXButtonStateAry[rEvent.gaxis.which] == EActionPress::DOWN && result == EActionPress::UP )
                    {
                        //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left Y: %d") % ((int)rEvent.caxis.value) ) );
                        m_analogRXButtonStateAry[rEvent.gaxis.which] = EActionPress::IDLE;
                    }
                    else
                        result = EActionPress::IDLE;

                    m_lastDeviceUsed = EDeviceId::GAMEPAD;
                }
            }
            else if( rEvent.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY )
            {
                if( m_analogRYButtonStateAry[rEvent.gaxis.which] == EActionPress::IDLE )
                {
                    if( (rEvent.gaxis.value < -ANALOG_STICK_MSG_MAX) && wasAction( ANALOG2_UP, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::DOWN;

                    else if( (rEvent.gaxis.value > ANALOG_STICK_MSG_MAX) && wasAction( ANALOG2_DOWN, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::DOWN;
                }
                else if( m_analogRYButtonStateAry[rEvent.gaxis.which] == EActionPress::DOWN )
                {
                    if( (rEvent.gaxis.value > -ANALOG_STICK_MSG_MAX) && (rEvent.gaxis.value < ANALOG_STICK_MSG_MAX) && wasAction( ANALOG2_UP, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::UP;

                    else if( (rEvent.gaxis.value < ANALOG_STICK_MSG_MAX) && (rEvent.gaxis.value > -ANALOG_STICK_MSG_MAX) && wasAction( ANALOG2_DOWN, actionStr, m_gamepadActionMap ) )
                        result = EActionPress::UP;
                }

                if( result != EActionPress::IDLE )
                {
                    if( m_analogRYButtonStateAry[rEvent.gaxis.which] == EActionPress::IDLE && result == EActionPress::DOWN )
                    {
                        //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left Y: %d") % ((int)rEvent.caxis.value) ) );
                        m_analogRYButtonStateAry[rEvent.gaxis.which] = EActionPress::DOWN;
                    }
                    else if( m_analogRYButtonStateAry[rEvent.gaxis.which] == EActionPress::DOWN && result == EActionPress::UP )
                    {
                        //NGenFunc::PostDebugMsg( boost::str( boost::format("Axis Value Left Y: %d") % ((int)rEvent.caxis.value) ) );
                        m_analogRYButtonStateAry[rEvent.gaxis.which] = EActionPress::IDLE;
                    }
                    else
                        result = EActionPress::IDLE;

                    m_lastDeviceUsed = EDeviceId::GAMEPAD;
                }
            }
        }
    }

    return result;
}


/************************************************************************
*    DESC:  Was this an action
************************************************************************/
bool CActionMgr::wasAction(
    const int Id,
    const std::string & actionStr,
    const actionMapType & actionMap )
{
    bool result(false);

    // See if the action has already been added
    auto iter = actionMap.find( actionStr );

    // If it's found, see if this is the correct action
    if( iter != actionMap.end() )
    {
        result = iter->second.wasAction( Id );
    }

    return result;
}


/************************************************************************
*    DESC:  What was the last device
************************************************************************/
bool CActionMgr::wasLastDeviceGamepad()
{
    return (m_lastDeviceUsed == EDeviceId::GAMEPAD);
}

bool CActionMgr::wasLastDeviceKeyboard()
{
    return (m_lastDeviceUsed == EDeviceId::KEYBOARD);
}

bool CActionMgr::wasLastDeviceMouse()
{
    return (m_lastDeviceUsed == EDeviceId::MOUSE);
}

EDeviceId CActionMgr::getLastDeviceUsed()
{
    return m_lastDeviceUsed;
}


/************************************************************************
*    DESC:  Reset the last used device
************************************************************************/
void CActionMgr::resetLastUsedDevice()
{
    m_lastDeviceUsed = EDeviceId::_NULL_;
}


/************************************************************************
*    DESC:  Get the mouse position
************************************************************************/
const CPoint<float> & CActionMgr::getMouseAbsolutePos() const
{
    return m_mouseAbsolutePos;
}

const CPoint<float> & CActionMgr::getMouseRelativePos() const
{
    return m_mouseRelativePos;
}


/************************************************************************
*    DESC:  Get the last controller position
************************************************************************/
const CPoint<float> & CActionMgr::getControllerPosLeft() const
{
    return m_lastAnalogLeft;
}

const CPoint<float> & CActionMgr::getControllerPosRight() const
{
    return m_lastAnalogRight;
}


/************************************************************************
*    DESC:  Get the action/component strings for the give device id
************************************************************************/
std::string CActionMgr::getDeviceActionStr(
    EDeviceId deviceId,
    const std::string & actionNameStr,
    bool & configurable )
{
    std::string mappingName("keyboardMapping");

    if( deviceId == EDeviceId::MOUSE )
    {
        mappingName = "mouseMapping";
    }
    else if( deviceId == EDeviceId::GAMEPAD )
    {
        mappingName = "gamepadMapping";
    }

    std::string componetIdStr;
    XMLNode playerVisibleNode = m_mainNode.getChildNode( mappingName.c_str() ).getChildNode( "playerVisible" );
    getComponentStr( playerVisibleNode, actionNameStr, componetIdStr, configurable );

    return componetIdStr;
}


/************************************************************************
*    DESC:  Get the component string for the device id
************************************************************************/
int CActionMgr::getComponentStr(
    const XMLNode & playerVisibleNode,
    const std::string & actionNameStr,
    std::string & componetIdStr,
    bool & configurable )
{
    for( int i = 0; i < playerVisibleNode.nChildNode(); ++i )
    {
        XMLNode actionMapNode = playerVisibleNode.getChildNode( "actionMap", i );

        if( actionNameStr == actionMapNode.getAttribute( "action" ) )
        {
            componetIdStr = actionMapNode.getAttribute( "componetId" );

            configurable = false;

            if( actionMapNode.isAttributeSet( "configurable" ) )
                configurable = ( std::strcmp( actionMapNode.getAttribute("configurable"), "true" ) == 0 );

            return i;
        }
    }

    return UNDEFINED_ACTION;
}


/************************************************************************
*    DESC:  Enable/Disable action handling
************************************************************************/
void CActionMgr::enableAction( bool value )
{
    m_allowAction = value;
}


/************************************************************************
*    DESC:  Is action handling allowed
************************************************************************/
bool CActionMgr::isAction()
{
    return m_allowAction;
}


/************************************************************************
*    DESC:  Reset the action
************************************************************************/
std::string CActionMgr::resetAction(
    EDeviceId deviceId,
    const std::string & actionNameStr,
    int keyCode,
    bool & configurable )
{
    std::string componetIdStr;
    std::string mappingName("keyboardMapping");
    keyCodeMapType * pKeyCodeMap = &m_keyboardKeyCodeMap;
    actionMapType * pActionMap = &m_keyboardActionMap;

    if( deviceId == EDeviceId::MOUSE )
    {
        mappingName = "mouseMapping";
        pKeyCodeMap = &m_mouseKeyCodeMap;
        pActionMap = &m_mouseActionMap;
    }
    else if( deviceId == EDeviceId::GAMEPAD )
    {
        mappingName = "gamepadMapping";
        pKeyCodeMap = &m_gamepadKeyCodeMap;
        pActionMap = &m_gamepadActionMap;
    }

    std::string oldIdStr;

    // If this action ID can be found and is configurable, reset it
    XMLNode playerVisibleNode = m_mainNode.getChildNode( mappingName.c_str() ).getChildNode( "playerVisible" );
    int xmlNodeIndex = getComponentStr( playerVisibleNode, actionNameStr, oldIdStr, configurable );

    if( (xmlNodeIndex > UNDEFINED_ACTION) && configurable )
    {
        std::string newKeyCodeStr;
        if( getKeyCodeStr( *pKeyCodeMap, keyCode, newKeyCodeStr ) )
        {
            componetIdStr = UNBOUND_KEYCODE_STR_ID;
            if( newKeyCodeStr != oldIdStr )
                componetIdStr = newKeyCodeStr;
            else
                keyCode = UNBOUND_KEYCODE_ID;
                
            int oldKeyCodeId = getKeyCode( *pKeyCodeMap, oldIdStr );

            // Check for the action to remove the old key code
            auto iter = pActionMap->find( actionNameStr );
            if( iter != pActionMap->end() )
            {
                // Remove the old key code Id
                iter->second.removeId( oldKeyCodeId );

                // Add the new key code Id
                iter->second.setId( keyCode );

                // Update the XML node with the change
                XMLNode node = playerVisibleNode.getChildNode( "actionMap", xmlNodeIndex );
                node.updateAttribute(componetIdStr.c_str(), "componetId", "componetId");
            }
        }
    }

    return componetIdStr;
}


/************************************************************************
*    DESC:  Get the string associated with the key code
************************************************************************/
bool CActionMgr::getKeyCodeStr( keyCodeMapType & keyCodeMap, const int keyCode, std::string & componetIdStr )
{
    // See if we can find the string that represents the key code id
    auto keyCodeIter = keyCodeMap.right.find( keyCode );

    // Add it in if we found it
    if( keyCodeIter != keyCodeMap.right.end() )
    {
        componetIdStr = keyCodeIter->second;
        return true;
    }

    return false;
}


/************************************************************************
*    DESC:  Get the key code associated with the string
************************************************************************/
int CActionMgr::getKeyCode( keyCodeMapType & keyCodeMap, const std::string & componetIdStr )
{
    // See if we can find the string that represents the key code id
    auto keyCodeIter = keyCodeMap.left.find( componetIdStr );

    // Add it in if we found it
    if( keyCodeIter != keyCodeMap.left.end() )
        return keyCodeIter->second;

    return UNBOUND_KEYCODE_ID;
}


/************************************************************************
*    DESC:  Save the xml to file
************************************************************************/
void CActionMgr::saveToFile()
{
    // Save it to file
    m_mainNode.writeToFile(m_saveFilePath.c_str(), "utf-8");
}


/************************************************************************
*    DESC:  Reset all the key bindings to their default settings
************************************************************************/
void CActionMgr::resetKeyBindingsToDefault()
{
    std::vector<std::string> stringPathsVec = {
        "keyboardMapping/playerVisible",
        "mouseMapping/playerVisible",
        "gamepadMapping/playerVisible" };

    for( auto & iter : stringPathsVec )
    {
        XMLNode playerVisibleNode = m_mainNode.getChildNodeByPath( iter.c_str() );

        for( int i = 0; i < playerVisibleNode.nChildNode("actionMap"); ++i )
        {
            XMLNode node = playerVisibleNode.getChildNode( "actionMap", i );

            if( node.isAttributeSet( "configurable" ) &&
                ( std::strcmp( node.getAttribute("configurable"), "true" ) == 0 ) &&
                node.isAttributeSet( "defaultId" ) )
            {
                node.updateAttribute( node.getAttribute( "defaultId"), "componetId", "componetId");
            }
        }
    }

    // Clear out all the actions
    m_keyboardActionMap.clear();
    m_mouseActionMap.clear();
    m_gamepadActionMap.clear();

    // Load the keyboard/mouse/gamepad mapping from node
    loadAction();

    // Save the xml to file
    m_mainNode.writeToFile(m_saveFilePath.c_str(), "utf-8");
}


/************************************************************************
*    DESC:  Queue the event
************************************************************************/
void CActionMgr::queueEvent( const SDL_Event & rEvent )
{
    m_eventQueue.emplace_back( rEvent );

    if( rEvent.type == SDL_EVENT_MOUSE_MOTION )
    {
        m_mouseAbsolutePos.x = rEvent.motion.x;
        m_mouseAbsolutePos.y = rEvent.motion.y;

        m_mouseRelativePos.x = rEvent.motion.xrel;
        m_mouseRelativePos.y = rEvent.motion.yrel;
        
        m_lastDeviceUsed = EDeviceId::MOUSE;
    }
    else if( rEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN || rEvent.type == SDL_EVENT_MOUSE_BUTTON_UP )
    {
        m_mouseAbsolutePos.x = rEvent.button.x;
        m_mouseAbsolutePos.y = rEvent.button.y;

        m_lastDeviceUsed = EDeviceId::MOUSE;
    }
    else if( rEvent.type == SDL_EVENT_GAMEPAD_AXIS_MOTION )
    {
        if( rEvent.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX )
            m_lastAnalogLeft.x = rEvent.gaxis.value;

        else if( rEvent.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY )
            m_lastAnalogLeft.y = rEvent.gaxis.value;

        if( rEvent.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX )
            m_lastAnalogRight.x = rEvent.gaxis.value;

        else if( rEvent.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY )
            m_lastAnalogRight.y = rEvent.gaxis.value;
        
        m_lastDeviceUsed = EDeviceId::GAMEPAD;
    }
    else if( (rEvent.type == NMenuEvent::ROOT_TRANS_IN ||
        rEvent.type == NMenuEvent::TRANS_IN) &&
        rEvent.user.code == NTransCode::END )
    {
        resetLastUsedDevice();
    }
    /*else if( rEvent.type == SDL_SENSORUPDATE )
    {
        auto iter = m_sensorMap.find( rEvent.sensor.which );
        if( iter == m_sensorMap.end() )
            iter = m_sensorMap.emplace( rEvent.sensor.which, CSensor() ).first;
        
        iter->second.v1 = rEvent.sensor.data[0];
        iter->second.v2 = rEvent.sensor.data[1];
        iter->second.v3 = rEvent.sensor.data[2];
        iter->second.v4 = rEvent.sensor.data[3];
        iter->second.v5 = rEvent.sensor.data[4];
        iter->second.v6 = rEvent.sensor.data[5];
    }*/
}


/************************************************************************
*    DESC:  Is the queue empty
************************************************************************/
bool CActionMgr::isQueueEmpty()
{
    return m_eventQueue.empty();
}


/************************************************************************
*    DESC:  Clear the queue
************************************************************************/
void CActionMgr::clearQueue()
{
    m_queueIndex = 0;
    m_eventQueue.clear();
}


/************************************************************************
*    DESC:  Was this an action event
************************************************************************/
bool CActionMgr::wasActionEvent( const std::string & actionStr, EActionPress actionPress )
{
    if( m_allowAction )
    {
        for( auto & iter : m_eventQueue )
            if( wasAction( iter, actionStr ) == actionPress )
                return true;
    }

    return false;
}

/************************************************************************
*    DESC:  Device specific key checks
************************************************************************/
bool CActionMgr::wasKeyboardEvent( const std::string & componentIdStr, EActionPress actionPress )
{
    const auto keyCodeIter = m_keyboardKeyCodeMap.left.find( componentIdStr );
    if( keyCodeIter != m_keyboardKeyCodeMap.left.end() )
    {
        for( auto & iter : m_eventQueue )
        {
            if( (int)iter.key.key == keyCodeIter->second )
            {
                if( (iter.type == SDL_EVENT_KEY_DOWN) &&
                    (actionPress == EActionPress::DOWN) &&
                    (iter.key.repeat == 0) )
                    return true;

                else if( (iter.type == SDL_EVENT_KEY_UP) &&
                    (actionPress == EActionPress::UP) &&
                    (iter.key.repeat == 0) )
                    return true;
            }
        }
    }

    return false;
}

bool CActionMgr::wasMouseBtnEvent( const std::string & componentIdStr, EActionPress actionPress )
{
    const auto keyCodeIter = m_mouseKeyCodeMap.left.find( componentIdStr );
    if( keyCodeIter != m_mouseKeyCodeMap.left.end() )
    {
        for( auto & iter : m_eventQueue )
        {
            if( iter.button.button == keyCodeIter->second )
            {
                if( (actionPress == EActionPress::DOWN) &&
                    (iter.type == SDL_EVENT_MOUSE_BUTTON_DOWN) )
                    return true;

                else if( (actionPress == EActionPress::UP) &&
                    (iter.type == SDL_EVENT_MOUSE_BUTTON_UP) )
                    return true;
            }
        }
    }

    return false;
}

bool CActionMgr::wasGamepadBtnEvent( const std::string & componentIdStr, EActionPress actionPress )
{
    const auto keyCodeIter = m_gamepadKeyCodeMap.left.find( componentIdStr );
    if( keyCodeIter != m_gamepadKeyCodeMap.left.end() )
    {
        for( auto & iter : m_eventQueue )
        {
            if( iter.gbutton.button == keyCodeIter->second )
            {
                if( (actionPress == EActionPress::DOWN) &&
                    (iter.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) )
                    return true;

                else if( (actionPress == EActionPress::UP) &&
                    (iter.type == SDL_EVENT_GAMEPAD_BUTTON_UP) )
                    return true;
            }
        }
    }

    return false;
}


/************************************************************************
*    DESC:  Generic call for any event
************************************************************************/
bool CActionMgr::wasEvent( uint event )
{
    for( auto & iter : m_eventQueue )
        if( iter.type == event )
            return true;

    return false;
}


/************************************************************************
*    DESC:  Was this a game specific event
************************************************************************/
bool CActionMgr::wasGameEvent( uint type, int code )
{
    for( auto & iter : m_eventQueue )
        if( (iter.type == type) && (iter.user.code == code) )
            return true;

    return false;
}


/************************************************************************
*    DESC:  Enumerate button events on all devices
************************************************************************/
uint CActionMgr::enumerateButtonEvents( uint & type, int & code, int & data, uint startIndex )
{
    for( uint i = startIndex; i < m_eventQueue.size(); i++ )
    {
        SDL_Event & rEvent = m_eventQueue[i];

        if( rEvent.type == SDL_EVENT_KEY_DOWN || rEvent.type == SDL_EVENT_KEY_UP )
        {
            type = rEvent.type;
            code = rEvent.key.key;
            data = rEvent.key.repeat;

            return i + 1;
        }
        else if( rEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN || rEvent.type == SDL_EVENT_MOUSE_BUTTON_UP )
        {
            type = rEvent.type;
            code = rEvent.button.button;
            data = rEvent.button.which;

            return i + 1;
        }
        else if( rEvent.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN || rEvent.type == SDL_EVENT_GAMEPAD_BUTTON_UP )
        {
            type = rEvent.type;
            code = rEvent.gbutton.button;
            data = rEvent.gbutton.which;

            return i + 1;
        }
    }

    return 0;
}


/************************************************************************
*    DESC:  Enumerate Mouse wheel events
************************************************************************/
uint CActionMgr::enumerateMouseWheelEvents( uint & windowID, int & x, int & y, uint & direction, uint startIndex )
{
    for( uint i = startIndex; startIndex < m_eventQueue.size(); i++ )
    {
        SDL_Event & rEvent = m_eventQueue[i];

        if( rEvent.type == SDL_EVENT_MOUSE_WHEEL )
        {
            windowID = rEvent.wheel.windowID;
            x = rEvent.wheel.x;
            y = rEvent.wheel.y;
            direction = rEvent.wheel.direction;

            return i + 1;
        }
    }

    return 0;
}


/************************************************************************
*    DESC:  Enumerate touch finger events
************************************************************************/
uint CActionMgr::enumerateTouchFingerEvents(
    uint & eventId, int64_t & touchId, int64_t & fingerId, float & x, float & y, float & dx, float & dy, float & pressure, uint startIndex )
{
    for( uint i = startIndex; startIndex < m_eventQueue.size(); i++ )
    {
        SDL_Event & rEvent = m_eventQueue[i];

        if( (rEvent.type >= SDL_EVENT_FINGER_DOWN) && (rEvent.type <= SDL_EVENT_FINGER_MOTION) )
        {
            eventId = rEvent.type;
            touchId = rEvent.tfinger.touchID;
            fingerId = rEvent.tfinger.fingerID;
            x = rEvent.tfinger.x;
            y = rEvent.tfinger.y;
            dx = rEvent.tfinger.dx;
            dy = rEvent.tfinger.dy;
            pressure = rEvent.tfinger.pressure;

            return i + 1;
        }
    }

    return 0;
}
