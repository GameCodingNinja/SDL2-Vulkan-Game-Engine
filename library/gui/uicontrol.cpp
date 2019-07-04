
/************************************************************************
*    FILE NAME:       uicontrol.cpp
*
*    DESCRIPTION:     class for user interface controls
************************************************************************/

// Physical component dependency
#include <gui/uicontrol.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>
#include <utilities/exceptionhandling.h>
#include <utilities/deletefuncs.h>
#include <gui/menudefs.h>
#include <gui/ismartguibase.h>
#include <gui/messagecracker.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>
#include <system/device.h>
#include <managers/actionmanager.h>
#include <common/fontproperties.h>
#include <common/ivisualcomponent.h>
#include <sprite/sprite.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>
#include <iostream>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIControl::CUIControl( const std::string & group ) :
    iControl( group ),
    m_state(NUIControl::ECS_NULL),
    m_lastState(NUIControl::ECS_NULL),
    m_actionType(NUIControl::ECAT_NULL),
    m_mouseSelectType(NDefs::EAP_UP)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CUIControl::~CUIControl()
{
    NDelFunc::DeleteVectorPointers( m_pSpriteVec );
}


/************************************************************************
*    DESC:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CUIControl::loadFromNode( const XMLNode & node )
{
    iControl::loadFromNode( node );

    // Set the default state of the control
    if( node.isAttributeSet( "defaultState" ) )
        setDefaultState( std::string( node.getAttribute( "defaultState" ) ) );

    // Set if mouse selection is the down message
    if( node.isAttributeSet( "mouseSelectDown" ) )
        if( std::strcmp( node.getAttribute( "mouseSelectDown" ), "true") == 0 )
            m_mouseSelectType = NDefs::EAP_DOWN;

    // Setup the action
    XMLNode actionNode = node.getChildNode( "action" );
    if( !actionNode.isEmpty() )
    {
        // Set the action type
        if( actionNode.isAttributeSet( "actionType" ) )
            setActionType( actionNode.getAttribute( "actionType" ) );

        // Set the execution action.
        if( actionNode.isAttributeSet( "executionAction" ) )
            setExecutionAction( std::string(actionNode.getAttribute( "executionAction" )) );
    }

    // Setup the script functions for this control
    XMLNode scriptLstNode = node.getChildNode( "scriptLst" );
    if( !scriptLstNode.isEmpty() )
    {
        // Internal support for "init", "disabled", "inactive", "active", "select", "execute"
        for( int i = 0; i < scriptLstNode.nChildNode(); ++i )
        {
            const XMLNode scriptNode = scriptLstNode.getChildNode(i);

            // Only the first attribute is used
            const XMLAttribute attribute = scriptNode.getAttribute(0);
            const std::string attrName = attribute.lpszName;
            const std::string attrValue = attribute.lpszValue;

            // Add the attribute name and value to the map
            if( !attrValue.empty() )
                setScriptStateFunc( attrName, attrValue );
        }
    }

    // Load the scroll data from node
    m_scrollParam.loadFromNode( node.getChildNode( "scroll" ) );

    // Get the size modifier info
    m_sizeModifier = NParseHelper::LoadRect( node );

    // Init to the default state
    revertToDefaultState();
}


/************************************************************************
*    DESC:  Set the script state function
************************************************************************/
void CUIControl::setScriptStateFunc( const std::string & scriptStateStr, const std::string & scriptFuncStr )
{
    auto ctrlState = getScriptState( scriptStateStr );

    if( ctrlState != NUIControl::ECS_NULL )
        m_scriptFunction.emplace( ctrlState, scriptFuncStr );
}


/************************************************************************
*    DESC:  Get the script state
************************************************************************/
NUIControl::EControlState CUIControl::getScriptState( const std::string & scriptStateStr )
{
    if( scriptStateStr == "init" )
        return NUIControl::ECS_INIT;

    else if( scriptStateStr == "disabled" )
        return NUIControl::ECS_DISABLED;

    else if( scriptStateStr == "inactive" )
        return NUIControl::ECS_INACTIVE;

    else if( scriptStateStr == "active" )
        return NUIControl::ECS_ACTIVE;

    else if( scriptStateStr == "select" )
        return NUIControl::ECS_SELECTED;

    else if( scriptStateStr == "execute" )
        return NUIControl::ECS_EXECUTE;

    return NUIControl::ECS_NULL;
}


/************************************************************************
*    DESC:  Load the control specific info from XML node
************************************************************************/
void CUIControl::loadControlFromNode( const XMLNode & node )
{
    // Get the list of object data associated with this button
    const XMLNode spriteLstNode = node.getChildNode( "spriteList" );

    if( !spriteLstNode.isEmpty() )
    {
        size_t fontSpriteCount(0);

        for( int i = 0; i < spriteLstNode.nChildNode(); ++i )
        {
            const XMLNode spriteNode = spriteLstNode.getChildNode( i );

            // Load the sprite from node
            loadSpriteFromNode( spriteNode, fontSpriteCount );
        }
    }
}


/************************************************************************
*    DESC:  Load a sprite from an XML node
************************************************************************/
void CUIControl::loadSpriteFromNode( const XMLNode & node, size_t & fontSpriteCount )
{
    // Get the type of object
    std::string objectName = node.getAttribute( "objectName" );

    // Internally allocate the sprite in the deque
    m_pSpriteVec.push_back( new CSprite( CObjectDataMgr::Instance().getData2D( m_group, objectName ) ) );
    auto pSprite = m_pSpriteVec.back();

    // Load the sprite data
    pSprite->load( node );

    // See if this sprite is used for rendering a font string
    if( pSprite->getVisualComponent()->isFontSprite() )
    {
        // Set the font string to be created later
        if( !m_stringVec.empty() && (fontSpriteCount < m_stringVec.size()) )
            pSprite->getVisualComponent()->setFontString( m_stringVec.at(fontSpriteCount) );

        ++fontSpriteCount;
    }
    else
    {
        // Find the largest size width and height of the different sprites for the controls size
        const float width( pSprite->getVisualComponent()->getSize().w + std::fabs( pSprite->getObject()->getPos().x ) );
        const float height( pSprite->getVisualComponent()->getSize().h + std::fabs( pSprite->getObject()->getPos().y ) );

        if( width > m_size.w )
            m_size.w = width;

        if( height > m_size.h )
            m_size.h = height;
    }
}


/************************************************************************
*    DESC:  Update the control
************************************************************************/
void CUIControl::update()
{
    m_scriptComponent.update();

    for( auto iter : m_pSpriteVec )
        iter->update();
}


/************************************************************************
*    DESC:  Transform the control
************************************************************************/
void CUIControl::transform( const CObject2D & object )
{
    CObject2D::transform( object );

    for( auto iter : m_pSpriteVec )
        iter->getObject()->transform( *this );

    transformCollision();
}


/************************************************************************
*    DESC:  Transform the collision
************************************************************************/
void CUIControl::transformCollision()
{
    if( wasWorldPosTranformed() && !m_size.isEmpty() )
    {
        CMatrix finalMatrix( getMatrix() );
        finalMatrix.scale( CSettings::Instance().getOrthoAspectRatio().getH() );

        // Get half the screen size to convert to screen coordinates
        CSize<float> screenHalf = CSettings::Instance().getSizeHalf();

        // Create the rect of the control based on half it's size
        float halfwidth = m_size.getW() * 0.5f;
        float halfHeight = m_size.getH() * 0.5f;

        CQuad quad;
        quad.point[0].x = -halfwidth + -m_sizeModifier.x1;
        quad.point[0].y = -halfHeight + -m_sizeModifier.y1;
        quad.point[1].x = halfwidth + m_sizeModifier.x2;
        quad.point[1].y = -halfHeight + -m_sizeModifier.y1;
        quad.point[2].x = halfwidth + m_sizeModifier.x2;
        quad.point[2].y = halfHeight + m_sizeModifier.y2;
        quad.point[3].x = -halfwidth + -m_sizeModifier.x1;
        quad.point[3].y = halfHeight + m_sizeModifier.y2;

        finalMatrix.transform( m_collisionQuad, quad );

        // Convert the translated rect to screen coordinates
        m_collisionQuad.point[0].x += screenHalf.getW();
        m_collisionQuad.point[0].y += screenHalf.getH();
        m_collisionQuad.point[1].x += screenHalf.getW();
        m_collisionQuad.point[1].y += screenHalf.getH();
        m_collisionQuad.point[2].x += screenHalf.getW();
        m_collisionQuad.point[2].y += screenHalf.getH();
        m_collisionQuad.point[3].x += screenHalf.getW();
        m_collisionQuad.point[3].y += screenHalf.getH();

        finalMatrix.transform( m_collisionCenter, CPoint<float>() );

        // Convert to screen coordinates
        m_collisionCenter.x += screenHalf.getW();
        m_collisionCenter.y += screenHalf.getH();
    }
}


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CUIControl::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera )
{
    for( auto iter : m_pSpriteVec )
        iter->recordCommandBuffer( index, cmdBuf, camera );
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CUIControl::handleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenuDefs::EME_MENU_CONTROL_STATE_CHANGE )
    {
        onStateChange( rEvent );
    }
    else if( rEvent.type == NMenuDefs::EME_MENU_SELECT_EXECUTE )
    {
        onSelectExecute( rEvent );
    }
    else if( rEvent.type == NMenuDefs::EME_MENU_SET_ACTIVE_CONTROL )
    {
        onSetActiveControl( rEvent );
    }
    else if( rEvent.type == NMenuDefs::EME_MENU_REACTIVATE )
    {
        onReactivate( rEvent );
    }
    else if( rEvent.type == NMenuDefs::EME_MENU_TRANS_IN )
    {
        onTransIn( rEvent );
    }
    else if( rEvent.type == NMenuDefs::EME_MENU_TRANS_OUT )
    {
        onTransOut( rEvent );
    }

    // Do any smart event handling
    smartHandleEvent( rEvent );
}


/************************************************************************
*    DESC:  Handle OnTransIn message
************************************************************************/
void CUIControl::onTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NMenuDefs::ETC_BEGIN )
    {
        // Set the script functions for the current displayed state
        if( m_lastState != m_state )
            setDisplayState();
    }
}

/************************************************************************
*    DESC:  Handle OnTransOut message
************************************************************************/
void CUIControl::onTransOut( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NMenuDefs::ETC_BEGIN )
    {
        // Reset the control
        reset();

        // Recycle the contexts
        recycleContext();

        // Set the script functions for the current displayed state
        if( m_lastState != m_state )
            setDisplayState();
    }
}

/************************************************************************
*    DESC:  Handle OnStateChange message
************************************************************************/
void CUIControl::onStateChange( const SDL_Event & rEvent )
{
    // This control is the focus of the state change
    // The control's "this" pointer is used as a means of identification
    if( rEvent.user.data1 == (void *)this )
        changeState( NUIControl::EControlState(rEvent.user.code) );
    else
        deactivateControl();
}

/************************************************************************
*    DESC:  Handle OnSelectExecute message
************************************************************************/
void CUIControl::onSelectExecute( const SDL_Event & rEvent )
{
    if( m_state == NUIControl::ECS_SELECTED )
    {
        if( m_actionType == NUIControl::ECAT_TO_TREE )
            NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_TO_TREE, 0, &m_executionAction );

        else if( m_actionType == NUIControl::ECAT_TO_MENU )
            NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_TO_MENU, 0, &m_executionAction, this );

        else if( m_actionType == NUIControl::ECAT_BACK )
            NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_BACK_ACTION );

        else if( m_actionType == NUIControl::ECAT_CLOSE )
            NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_TOGGLE_ACTION );

        else if( m_actionType == NUIControl::ECAT_GAME_STATE_CHANGE )
            NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_GAME_STATE_CHANGE, NMenuDefs::ETC_BEGIN, &m_executionAction );

        else if( m_actionType == NUIControl::ECAT_QUIT_GAME )
            NGenFunc::DispatchEvent( SDL_QUIT );

        // Smart gui execution
        smartExecuteAction();

        // Boost signal execute action
        m_executionActionSignal(this);

        // Prepare script functions associated with this event
        prepareControlScriptFunction( NUIControl::ECS_EXECUTE );
    }
}


/************************************************************************
*    DESC:  Handle OnSetActiveControl message
************************************************************************/
void CUIControl::onSetActiveControl( const SDL_Event & rEvent )
{
    // Set the last active control to be active again
    if( (rEvent.user.code == NMenuDefs::EAC_LAST_ACTIVE_CONTROL) &&
        (m_lastState > NUIControl::ECS_INACTIVE))
    {
        m_lastState = m_state = NUIControl::ECS_ACTIVE;

        // Don't animate the control if the mouse was used
        if( !CActionMgr::Instance().wasLastDeviceMouse() )
        {
            recycleContext();
            setDisplayState();
        }
    }
}

/************************************************************************
*    DESC:  Handle OnReactivate message
************************************************************************/
void CUIControl::onReactivate( const SDL_Event & rEvent )
{
    // Set the last active control to be active again
    if( m_state > NUIControl::ECS_INACTIVE )
    {
        m_lastState = m_state = NUIControl::ECS_ACTIVE;

        // Don't animate the control if the mouse was used
        if( !CActionMgr::Instance().wasLastDeviceMouse() ||
            isPointInControl( CActionMgr::Instance().getMouseAbsolutePos() ) )
        {
            recycleContext();
            setDisplayState();
        }
    }
}


/************************************************************************
*    DESC:  Handle the mouse move
************************************************************************/
bool CUIControl::onMouseMove( const SDL_Event & rEvent )
{
    bool result(false);

    if( !isDisabled() && isPointInControl( rEvent.motion.x, rEvent.motion.y ) )
    {
        result = true;

        // Only send the message if it's not already active
        if( !isActive() )
        {
            NGenFunc::DispatchEvent(
                NMenuDefs::EME_MENU_CONTROL_STATE_CHANGE,
                NUIControl::ECS_ACTIVE,
                (void *)this );
        }
    }

    return result;
}


/************************************************************************
*    DESC:  Change the control state
************************************************************************/
void CUIControl::changeState( NUIControl::EControlState state )
{
    if( m_state != state )
    {
        m_state = state;

        // Prepare any script functions associated with the state change
        prepareControlScriptFunction( m_state );

        recycleContext();
        setDisplayState();

        m_lastState = m_state;
    }
}


/************************************************************************
*    DESC:  Activate the control
************************************************************************/
bool CUIControl::activateControl()
{
    // The focus has switched to this control
    if( !isDisabled() )
    {
        m_lastState = m_state = NUIControl::ECS_ACTIVE;

        recycleContext();
        setDisplayState();

        return true;
    }

    return false;
}


/************************************************************************
*    DESC:  Deactivate the control
************************************************************************/
void CUIControl::deactivateControl()
{
    // The focus has switched away from this control
    if( (m_lastState == NUIControl::ECS_NULL) ||
        (m_lastState > NUIControl::ECS_INACTIVE) )
    {
        // Reset the control
        reset();

        recycleContext();
        setDisplayState();

        m_lastState = m_state;
    }
}


/************************************************************************
*    DESC:  Disable the control
************************************************************************/
void CUIControl::disableControl()
{
    if( (m_lastState == NUIControl::ECS_NULL) ||
        (m_lastState > NUIControl::ECS_DISABLED) )
    {
        m_lastState = m_state = NUIControl::ECS_DISABLED;

        recycleContext();
        setDisplayState();
    }
}


/************************************************************************
*    DESC:  Enable the control to the inactive state
************************************************************************/
void CUIControl::enableControl()
{
    if( m_lastState <= NUIControl::ECS_DISABLED )
    {
        m_lastState = m_state = NUIControl::ECS_INACTIVE;

        recycleContext();
        setDisplayState();
    }
}


/************************************************************************
*    DESC:  Set the sprite's display based on it's current state
************************************************************************/
void CUIControl::setDisplayState()
{
    // Set the script function
    prepareSpriteScriptFunction( m_state );
}


/************************************************************************
*    DESC:  Init the control
************************************************************************/
void CUIControl::init()
{
    // Create any font strings
    // This allows for delayed VBO create so that the fonts can be allocated during the load screen
    for( auto iter : m_pSpriteVec )
        iter->init();
    
    // Prepare script functions associated with this event
    prepareControlScriptFunction( NUIControl::ECS_INIT );
}


/************************************************************************
*    DESC:  Prepare the sprite script function
************************************************************************/
void CUIControl::prepareSpriteScriptFunction( NUIControl::EControlState controlState )
{
    std::string scriptFuncMapKey = "null";
    bool forceUpdate(false);

    switch( controlState )
    {
        case NUIControl::ECS_DISABLED:
            scriptFuncMapKey = "disabled";
            forceUpdate = true;
        break;

        case NUIControl::ECS_INACTIVE:
            scriptFuncMapKey = "inactive";
            forceUpdate = true;
        break;

        case NUIControl::ECS_ACTIVE:
            scriptFuncMapKey = "active";
        break;

        case NUIControl::ECS_SELECTED:
            scriptFuncMapKey = "selected";
        break;

        case NUIControl::ECS_INIT:
        case NUIControl::ECS_EXECUTE:
        case NUIControl::ECS_EVENT:
        case NUIControl::ECS_NULL:
            throw NExcept::CCriticalException("Control State NULL!",
                boost::str( boost::format("Control state can't use this state for sprites (%s)!\n\n%s\nLine: %s")
                    % scriptFuncMapKey %  __FUNCTION__ % __LINE__ ));
        break;
    };

    // Force an update for states that just change settings and don't animate
    callSpriteScriptFuncKey( scriptFuncMapKey, forceUpdate );
}


/************************************************************************
*    DESC:  Call a script function map key for sprite
************************************************************************/
void CUIControl::callSpriteScriptFuncKey( const std::string & scriptFuncMapKey, bool forceUpdate )
{
    for( auto iter : m_pSpriteVec )
        iter->prepare( scriptFuncMapKey, forceUpdate );
}


/************************************************************************
*    DESC:  Prepare the script function to run
************************************************************************/
void CUIControl::prepareControlScriptFunction( NUIControl::EControlState controlState )
{
    auto iter = m_scriptFunction.find( controlState );
    if( iter != m_scriptFunction.end() )
        m_scriptComponent.prepare(m_group, iter->second, {this});
}


/************************************************************************
*    DESC:  Reset and recycle the contexts
************************************************************************/
void CUIControl::reset( bool complete )
{
    if( m_state > NUIControl::ECS_INACTIVE )
        m_state = NUIControl::ECS_INACTIVE;

    if( complete )
        m_lastState = m_state;
}


/************************************************************************
*    DESC:  Recycle the contexts
************************************************************************/
void CUIControl::recycleContext()
{
    for( auto iter : m_pSpriteVec )
        iter->getScriptComponent().resetAndRecycle();
}


/************************************************************************
*    DESC:  Set the default state of this control
************************************************************************/
void CUIControl::setDefaultState( const std::string & value )
{
    if( value == "inactive" )
        m_defaultState = NUIControl::ECS_INACTIVE;

    else if( value == "active" )
        m_defaultState = NUIControl::ECS_ACTIVE;

    else if( value == "disabled" )
        m_defaultState = NUIControl::ECS_DISABLED;

    else if( value == "selected" )
        m_defaultState = NUIControl::ECS_SELECTED;
}

void CUIControl::setDefaultState( NUIControl::EControlState value )
{
    m_defaultState = value;
}


/************************************************************************
*    DESC:  Set/Get the smart control pointer. This class owns the pointer
************************************************************************/
void CUIControl::setSmartGui( CSmartGuiControl * pSmartGuiControl )
{
    m_upSmartGui.reset( pSmartGuiControl );
}

CSmartGuiControl * CUIControl::getSmartGuiPtr()
{
    return m_upSmartGui.get();
}


/************************************************************************
*    DESC:  Do any smart create
************************************************************************/
void CUIControl::smartCreate()
{
    if( m_upSmartGui )
        m_upSmartGui->create();
}


/************************************************************************
*    DESC:  Do any smart event handling
************************************************************************/
void CUIControl::smartHandleEvent( const SDL_Event & rEvent )
{
    if( m_upSmartGui )
        m_upSmartGui->handleEvent( rEvent );
}


/************************************************************************
*    DESC:  Smart execute the action
************************************************************************/
void CUIControl::smartExecuteAction()
{
    if( m_upSmartGui )
        m_upSmartGui->execute();
}


/************************************************************************
*    DESC:  Set the control to their default behavior
************************************************************************/
void CUIControl::revertToDefaultState()
{
    m_state = m_defaultState;
}


/************************************************************************
*    DESC:  Get/Set the state of this control
************************************************************************/
NUIControl::EControlState CUIControl::getState() const
{
    return m_state;
}

void CUIControl::setState( NUIControl::EControlState state, bool setLastState )
{
    m_state = state;

    if( setLastState )
        m_lastState = state;
}


/************************************************************************
*    DESC:  Get/Set the control's action type
************************************************************************/
NUIControl::EControlActionType CUIControl::getActionType()
{
    return m_actionType;
}

void CUIControl::setActionType( NUIControl::EControlActionType value )
{
    m_actionType = value;
}

void CUIControl::setActionType( const std::string & value )
{
    if( value == "to_idle" )
        m_actionType = NUIControl::ECAT_IDLE;

    else if( value == "to_tree" )
        m_actionType = NUIControl::ECAT_TO_TREE;

    else if( value == "to_menu" )
        m_actionType = NUIControl::ECAT_TO_MENU;

    else if( value == "back" )
        m_actionType = NUIControl::ECAT_BACK;

    else if( value == "close" )
        m_actionType = NUIControl::ECAT_CLOSE;

    else if( value == "change_focus" )
        m_actionType = NUIControl::ECAT_CHANGE_FOCUS;

    else if( value == "game_state_change" )
        m_actionType = NUIControl::ECAT_GAME_STATE_CHANGE;

    else if( value == "quit_game" )
        m_actionType = NUIControl::ECAT_QUIT_GAME;
}


/************************************************************************
*    DESC:  Get/Set the execution action
************************************************************************/
const std::string & CUIControl::getExecutionAction()
{
    return m_executionAction;
}

void CUIControl::setExecutionAction( const std::string & action )
{
    m_executionAction = action;
}


/************************************************************************
*    DESC:  Create the font string
************************************************************************/
void CUIControl::createFontString( const std::string & fontString, int spriteIndex )
{
    int fontSpriteCounter(0);

    for( auto iter : m_pSpriteVec )
    {
        if( iter->getVisualComponent()->isFontSprite() )
        {
            if( fontSpriteCounter == spriteIndex )
            {
                iter->getVisualComponent()->createFontString( fontString );
                break;
            }

            ++fontSpriteCounter;
        }
    }
}

void CUIControl::createFontString( int stringIndex, int spriteIndex )
{
    if( !m_stringVec.empty() )
        createFontString( m_stringVec[stringIndex], spriteIndex );
}


/************************************************************************
*    DESC:  Set the font string
************************************************************************/
void CUIControl::setFontString( const std::string & fontString, int spriteIndex )
{
    int fontSpriteCounter(0);

    for( auto iter : m_pSpriteVec )
    {
        if( iter->getVisualComponent()->isFontSprite() )
        {
            if( fontSpriteCounter == spriteIndex )
            {
                iter->getVisualComponent()->setFontString( fontString );
                break;
            }

            ++fontSpriteCounter;
        }
    }
}


/************************************************************************
*    DESC:  Get the string vector
************************************************************************/
const std::vector<std::string> & CUIControl::getStringVec() const
{
    return m_stringVec;
}


/************************************************************************
*    DESC:  Handle the select action
*           Only process this message if it's keyboard/gamepad down or mouse select type
************************************************************************/
bool CUIControl::handleSelectAction( const CSelectMsgCracker & msgCracker )
{
    if( (isSelectable() &&
        msgCracker.isDeviceMouse() &&
        (msgCracker.getPressType() == m_mouseSelectType) &&
        isPointInControl( msgCracker.getPos() ) ) ||

        (isActive() && !msgCracker.isDeviceMouse() && msgCracker.isPressDown()) )
    {
        NGenFunc::DispatchEvent(
            NMenuDefs::EME_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_SELECTED,
            (void *)this );

        return true;
    }

    return false;
}


/************************************************************************
*    DESC: Set the first inactive control to be active
*    NOTE: This is mainly here to be virtual for sub controls
************************************************************************/
bool CUIControl::activateFirstInactiveControl()
{
    // If a mouse was used, set the control as active but don't animate it.
    // This allows us to use the keys to scroll when pressed
    #if !(defined(__IOS__) || defined(__ANDROID__))
    if( CActionMgr::Instance().wasLastDeviceMouse() )
    {
        if( !isDisabled() )
        {
            m_lastState = m_state = NUIControl::ECS_ACTIVE;

            return true;
        }

        return false;
    }
    #endif

    return activateControl();
}


/************************************************************************
*    DESC:  Is the point in the control
************************************************************************/
bool CUIControl::isPointInControl( int x, int y )
{
    return m_collisionQuad.isPointInQuad( x, y );
}

bool CUIControl::isPointInControl( const CPoint<float> & pos )
{
    return m_collisionQuad.isPointInQuad( pos.x, pos.y );
}


/************************************************************************
*    DESC:  Get the pointer to the control if found
*
*    NOTE: These function is mainly for sub controls
************************************************************************/
iControl * CUIControl::findControl( const std::string & name )
{
    if( m_name == name )
        return this;

    return nullptr;
}

iControl * CUIControl::findControl( void * pVoid )
{
    if( pVoid == (void *)this )
        return this;

    return nullptr;
}


/************************************************************************
*    DESC:  Set the string to vector
************************************************************************/
void CUIControl::setStringToList( const std::string & str )
{
    m_stringVec.push_back( str );
}


/************************************************************************
*    DESC:  Is this control disabled/active/selected
************************************************************************/
bool CUIControl::isDisabled()
{
    return m_state == NUIControl::ECS_DISABLED;
}

bool CUIControl::isInactive()
{
    return m_state == NUIControl::ECS_INACTIVE;
}

bool CUIControl::isActive()
{
    return (m_state == NUIControl::ECS_ACTIVE);
}

bool CUIControl::isSelected()
{
    return (m_state == NUIControl::ECS_SELECTED);
}

bool CUIControl::isSelectable()
{
    return ((m_state == NUIControl::ECS_INACTIVE) || (m_state == NUIControl::ECS_ACTIVE));
}


/************************************************************************
*    DESC:  Get the collision position
************************************************************************/
const CPoint<float> & CUIControl::getCollisionPos() const
{
    return m_collisionCenter;
}


/************************************************************************
*    DESC:  Get the scroll params
************************************************************************/
CScrollParam & CUIControl::getScrollParam()
{
    return m_scrollParam;
}


/************************************************************************
*    DESC:  Check if control is a sub control
************************************************************************/
bool CUIControl::isSubControl() const
{
    return false;
}


/************************************************************************
*    DESC:  Connect to the execution action signal
************************************************************************/
void CUIControl::connect_executionAction( const ExecutionActionSignal::slot_type & slot )
{
    m_executionActionSignal.connect(slot);
}


/************************************************************************
*    DESC:  Set the alpha value of this control
************************************************************************/
void CUIControl::setAlpha( float alpha )
{
    for( auto iter : m_pSpriteVec )
        iter->getVisualComponent()->setAlpha( alpha );

    m_alpha = alpha;
}


/************************************************************************
*    DESC:  Get the pointer to the active control
*           This is mostly needed for sub controls
************************************************************************/
iControl * CUIControl::getPtrToActiveControl()
{
    return this;
}


/************************************************************************
*    DESC:  Get the mouse select type
************************************************************************/
NDefs::EActionPress CUIControl::getMouseSelectType() const
{
    return m_mouseSelectType;
}


/************************************************************************
*    DESC:  Get the size of this control
************************************************************************/
const CSize<float> & CUIControl::getSize() const
{
    return m_size;
}


/************************************************************************
*    DESC:  Can this control scroll?
************************************************************************/
bool CUIControl::canScroll( int msg )
{
    if( isActive() && m_scrollParam.canScroll(msg) )
        return true;

    return false;
}
