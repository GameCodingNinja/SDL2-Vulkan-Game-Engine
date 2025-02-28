
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
#include <gui/messagecracker.h>
#include <gui/menumanager.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>
#include <system/device.h>
#include <managers/actionmanager.h>
#include <common/fontproperties.h>
#include <common/ivisualcomponent.h>
#include <sprite/sprite.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIControl::CUIControl( const std::string & group ) :
    iControl( group ),
    m_state(EControlState::_NULL),
    m_lastState(EControlState::_NULL),
    m_actionType(EControlActionType::_NULL),
    m_mouseSelectType(EActionPress::UP)
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
            m_mouseSelectType = EActionPress::DOWN;

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
    loadScriptFromNode( node, m_group );

    // Load the scroll data from node
    m_scrollParam.loadFromNode( node.getChildNode( "scroll" ) );

    // Get the size modifier info
    m_sizeModifier = NParseHelper::LoadRect( node );

    // Init to the default state
    revertToDefaultState();
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
    m_pSpriteVec.push_back( new CSprite( CObjectDataMgr::Instance().getData( m_group, objectName ) ) );
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
        const float width( pSprite->getVisualComponent()->getSize().w + std::fabs( pSprite->getPos().x ) );
        const float height( pSprite->getVisualComponent()->getSize().h + std::fabs( pSprite->getPos().y ) );

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
void CUIControl::transform()
{
    CObject::transform();

    for( auto iter : m_pSpriteVec )
        iter->transform( *this );

    transformCollision();
}

void CUIControl::transform( const CObject & object )
{
    CObject::transform( object );

    for( auto iter : m_pSpriteVec )
        iter->transform( *this );

    transformCollision();
}

/************************************************************************
*    DESC:  Transform the collision
************************************************************************/
void CUIControl::transformCollision()
{
    if( wasTranformed() && !m_size.isEmpty() )
    {
        CMatrix finalMatrix( getMatrix() );
        finalMatrix.mergeMatrix( CMenuMgr::Instance().getCamera().getMatrix() );
        finalMatrix.scale( CSettings::Instance().getOrthoAspectRatioOrientation() );

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
    if( rEvent.type == NMenuEvent::CONTROL_STATE_CHANGE )
    {
        onStateChange( rEvent );
    }
    else if( rEvent.type == NMenuEvent::SELECT_EXECUTE )
    {
        onSelectExecute( rEvent );
    }
    // Menu after trans in, makes a control active
    else if( rEvent.type == NMenuEvent::SET_ACTIVE_CONTROL )
    {
        onSetActiveControl( rEvent );
    }
    // Called by script to reactive a control after the select animation completes
    // if this control and it's menu is designed to be selected again.
    else if( rEvent.type == NMenuEvent::REACTIVATE )
    {
        onReactivate( rEvent );
    }
    else if( rEvent.type == NMenuEvent::ROOT_TRANS_IN )
    {
        onRootTransIn( rEvent );
    }
    else if( rEvent.type == NMenuEvent::TRANS_IN )
    {
        onTransIn( rEvent );
    }
    else if( rEvent.type == NMenuEvent::TRANS_OUT )
    {
        onTransOut( rEvent );
    }

    // Prepare script function associated with handling this game event
    prepareControlScriptFunction( EControlState::EVENT, rEvent.type, rEvent.user.code );
}

/************************************************************************
*    DESC:  Handle OnRootTransIn message
************************************************************************/
void CUIControl::onRootTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NTransCode::BEGIN )
    {
        // Set the script functions for the current displayed state
        if( m_lastState != m_state )
            setDisplayState();

        // Prepare script function associated with handling this game event
        prepareControlScriptFunction( EControlState::ROOT_TRANS_IN );
    }
}

/************************************************************************
*    DESC:  Handle OnTransIn message
************************************************************************/
void CUIControl::onTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NTransCode::BEGIN )
    {
        // Set the script functions for the current displayed state
        if( m_lastState != m_state )
            setDisplayState();

        // Prepare script function associated with handling this game event
        prepareControlScriptFunction( EControlState::TRANS_IN );
    }
}

/************************************************************************
*    DESC:  Handle OnTransOut message
************************************************************************/
void CUIControl::onTransOut( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NTransCode::BEGIN )
    {
        // Reset the control
        reset();

        // Recycle the contexts
        recycleContext();

        // Set the script functions for the current displayed state
        if( m_lastState != m_state )
            setDisplayState();

        // Prepare script function associated with handling this game event
        prepareControlScriptFunction( EControlState::TRANS_OUT );
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
        changeState( EControlState(rEvent.user.code) );
    else
        deactivateControl();
}

/************************************************************************
*    DESC:  Handle OnSelectExecute message
************************************************************************/
void CUIControl::onSelectExecute( const SDL_Event & rEvent )
{
    if( m_state == EControlState::SELECT )
    {
        if( m_actionType == EControlActionType::TO_TREE )
            NGenFunc::DispatchEvent( NMenuEvent::TO_TREE, 0, &m_executionAction );

        else if( m_actionType == EControlActionType::TO_MENU )
            NGenFunc::DispatchEvent( NMenuEvent::TO_MENU, 0, &m_executionAction, this );

        else if( m_actionType == EControlActionType::BACK )
            NGenFunc::DispatchEvent( NMenuEvent::BACK_ACTION );

        else if( m_actionType == EControlActionType::CLOSE )
            NGenFunc::DispatchEvent( NMenuEvent::TOGGLE_ACTION );

        else if( m_actionType == EControlActionType::GAME_STATE_CHANGE )
            NGenFunc::DispatchEvent( NMenuEvent::GAME_STATE_CHANGE, NTransCode::BEGIN, &m_executionAction );

        else if( m_actionType == EControlActionType::QUIT_GAME )
            NGenFunc::DispatchEvent( SDL_EVENT_QUIT );

        // Prepare script function associated with handling this event
        prepareControlScriptFunction( EControlState::EXECUTE );
    }
}

/************************************************************************
*    DESC:  Handle OnSetActiveControl message
************************************************************************/
void CUIControl::onSetActiveControl( const SDL_Event & rEvent )
{
    // Set the last active control to be active again
    if( (rEvent.user.code == NActiveControl::LAST) &&
        (m_lastState > EControlState::INACTIVE))
    {
        m_lastState = m_state = EControlState::ACTIVE;

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
*    DESC:  Handle OnReactivate message
************************************************************************/
void CUIControl::onReactivate( const SDL_Event & rEvent )
{
    // Set the last active control to be active again
    if( m_state > EControlState::INACTIVE )
    {
        m_lastState = m_state = EControlState::ACTIVE;

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
                NMenuEvent::CONTROL_STATE_CHANGE,
                static_cast<int>(EControlState::ACTIVE),
                (void *)this );
        }
    }

    return result;
}

/************************************************************************
*    DESC:  Change the control state
************************************************************************/
void CUIControl::changeState( EControlState state )
{
    if( m_state != state )
    {
        m_state = state;

        // Prepare script function associated with handling this game event
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
        m_lastState = m_state = EControlState::ACTIVE;

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
    if( (m_lastState == EControlState::_NULL) ||
        (m_lastState > EControlState::INACTIVE) )
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
    if( (m_lastState == EControlState::_NULL) ||
        (m_lastState > EControlState::DISABLE) )
    {
        m_lastState = m_state = EControlState::DISABLE;

        recycleContext();
        setDisplayState();
    }
}

/************************************************************************
*    DESC:  Enable the control to the inactive state
************************************************************************/
void CUIControl::enableControl()
{
    if( m_lastState <= EControlState::DISABLE )
    {
        m_lastState = m_state = EControlState::INACTIVE;

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
    
    // Prepare any script functions that are flagged to prepareOnInit
    prepareOnInit();
}

/************************************************************************
*    DESC:  Prepare the sprite script function
************************************************************************/
void CUIControl::prepareSpriteScriptFunction( EControlState controlState )
{
    std::string scriptFuncMapKey = "null";

    switch( controlState )
    {
        case EControlState::ROOT_TRANS_IN:
            scriptFuncMapKey = "rootTransIn";
        break;

        case EControlState::TRANS_IN:
            scriptFuncMapKey = "transIn";
        break;

        case EControlState::TRANS_OUT:
            scriptFuncMapKey = "transOut";
        break;

        case EControlState::DISABLE:
            scriptFuncMapKey = "disable";
        break;

        case EControlState::INACTIVE:
            scriptFuncMapKey = "inactive";
        break;

        case EControlState::ACTIVE:
            scriptFuncMapKey = "active";
        break;

        case EControlState::SELECT:
            scriptFuncMapKey = "select";
        break;

        case EControlState::EXECUTE:
        case EControlState::EVENT:
        case EControlState::_NULL:
        case EControlState::CHANGE:
            throw NExcept::CCriticalException("Control State NULL!",
                boost::str( boost::format("Control state can't use this state for sprites (%s)!\n\n%s\nLine: %s")
                    % scriptFuncMapKey %  __FUNCTION__ % __LINE__ ));
        break;
    };

    for( auto iter : m_pSpriteVec )
        iter->prepare( scriptFuncMapKey );
}

/************************************************************************
*    DESC:  Prepare the script function to run
************************************************************************/
void CUIControl::prepareControlScriptFunction( EControlState controlState, uint type, int code )
{
    std::string scriptFuncMapKey = "null";

    switch( controlState )
    {
        case EControlState::_NULL:
            scriptFuncMapKey = "null";
        break;

        case EControlState::ROOT_TRANS_IN:
            scriptFuncMapKey = "rootTransIn";
        break;

        case EControlState::TRANS_IN:
            scriptFuncMapKey = "transIn";
        break;

        case EControlState::TRANS_OUT:
            scriptFuncMapKey = "transOut";
        break;

        case EControlState::DISABLE:
            scriptFuncMapKey = "disable";
        break;

        case EControlState::INACTIVE:
            scriptFuncMapKey = "inactive";
        break;

        case EControlState::ACTIVE:
            scriptFuncMapKey = "active";
        break;

        case EControlState::SELECT:
            scriptFuncMapKey = "select";
        break;

        case EControlState::CHANGE:
            scriptFuncMapKey = "change";
        break;

        case EControlState::EXECUTE:
            scriptFuncMapKey = "execute";
        break;

        case EControlState::EVENT:
            scriptFuncMapKey = "event";
        break;
    };

    auto iter = m_scriptFunctionMap.find( scriptFuncMapKey );
    if( iter != m_scriptFunctionMap.end() )
    {
        if( controlState == EControlState::EVENT )
            m_scriptComponent.prepare( iter->second.group, iter->second.funcName, {this, type, code} );
        else
            m_scriptComponent.prepare( iter->second.group, iter->second.funcName, {this} );
        
        // Force an update
        if( iter->second.forceUpdate )
            m_scriptComponent.update();
    }
}

/************************************************************************
*    DESC:  Prepare the script function Id to run
************************************************************************/
bool CUIControl::prepare( const std::string & scriptFuncId )
{
    bool result = CObject::prepare( scriptFuncId);

    // Have the individual sprites respond to the prepare
    for( auto iter : m_pSpriteVec )
        result |= iter->prepare( scriptFuncId );

    return result;
}

/************************************************************************
*    DESC:  Reset and recycle the contexts
************************************************************************/
void CUIControl::reset( bool complete )
{
    if( m_state > EControlState::INACTIVE )
        m_state = EControlState::INACTIVE;

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
        m_defaultState = EControlState::INACTIVE;

    else if( value == "active" )
        m_defaultState = EControlState::ACTIVE;

    else if( value == "disabled" )
        m_defaultState = EControlState::DISABLE;

    else if( value == "selected" )
        m_defaultState = EControlState::SELECT;
}

void CUIControl::setDefaultState( EControlState value )
{
    m_defaultState = value;
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
EControlState CUIControl::getState() const
{
    return m_state;
}

void CUIControl::setState( EControlState state, bool setLastState )
{
    m_state = state;

    if( setLastState )
        m_lastState = state;
}

/************************************************************************
*    DESC:  Get/Set the control's action type
************************************************************************/
EControlActionType CUIControl::getActionType()
{
    return m_actionType;
}

void CUIControl::setActionType( EControlActionType value )
{
    m_actionType = value;
}

void CUIControl::setActionType( const std::string & value )
{
    if( value == "to_idle" )
        m_actionType = EControlActionType::IDLE;

    else if( value == "to_tree" )
        m_actionType = EControlActionType::TO_TREE;

    else if( value == "to_menu" )
        m_actionType = EControlActionType::TO_MENU;

    else if( value == "back" )
        m_actionType = EControlActionType::BACK;

    else if( value == "close" )
        m_actionType = EControlActionType::CLOSE;

    else if( value == "change_focus" )
        m_actionType = EControlActionType::CHANGE_FOCUS;

    else if( value == "game_state_change" )
        m_actionType = EControlActionType::GAME_STATE_CHANGE;

    else if( value == "quit_game" )
        m_actionType = EControlActionType::QUIT_GAME;
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
            NMenuEvent::CONTROL_STATE_CHANGE,
            static_cast<int>(EControlState::SELECT),
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
    if ( !CSettings::Instance().isMobileDevice() )
    {
        if( CActionMgr::Instance().wasLastDeviceMouse() )
        {
            if( !isDisabled() )
            {
                m_lastState = m_state = EControlState::ACTIVE;

                // Animate the control if the mouse just happens to be in it
                if( isPointInControl( CActionMgr::Instance().getMouseAbsolutePos() ) )
                    return activateControl();
                
                return true;
            }

            return false;
        }
    }

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
    return m_state == EControlState::DISABLE;
}

bool CUIControl::isInactive()
{
    return m_state == EControlState::INACTIVE;
}

bool CUIControl::isActive()
{
    return (m_state == EControlState::ACTIVE);
}

bool CUIControl::isSelected()
{
    return (m_state == EControlState::SELECT);
}

bool CUIControl::isSelectable()
{
    return ((m_state == EControlState::INACTIVE) || (m_state == EControlState::ACTIVE));
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
EActionPress CUIControl::getMouseSelectType() const
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
