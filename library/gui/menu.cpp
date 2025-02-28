
/************************************************************************
*    FILE NAME:       menu.cpp
*
*    DESCRIPTION:     Class for user interface menu
************************************************************************/

// Physical component dependency
#include <gui/menu.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/deletefuncs.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <objectdata/objectdatamanager.h>
#include <gui/uicontrolfactory.h>
#include <gui/messagecracker.h>
#include <managers/actionmanager.h>
#include <common/ivisualcomponent.h>
#include <sprite/sprite.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMenu::CMenu( const std::string & name, const std::string & group ) :
    m_name(name),
    m_group(group),
    m_pActiveNode(nullptr),
    m_state(EMenuState::INACTIVE),
    m_alpha(0.f)
{
    // The menu needs to default hidden
    setVisible(false);
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CMenu::~CMenu()
{
    NDelFunc::DeleteVectorPointers( m_pStaticControlVec );
    NDelFunc::DeleteVectorPointers( m_pMouseOnlyControlVec );

    NDelFunc::DeleteVectorPointers( m_pControlNodeVec );
    NDelFunc::DeleteVectorPointers( m_pControlVec );
    NDelFunc::DeleteVectorPointers( m_pSpriteVec );
}

/************************************************************************
*    DESC:  Load the menu info from file
************************************************************************/
void CMenu::load( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "menu" );

    // Init the script functions
    loadScriptFromNode( mainNode );

    // Load the scroll data from node
    m_scrollParam.loadFromNode( mainNode.getChildNode( "scroll" ) );

    // Get the static sprite groups
    const XMLNode staticSpriteNode = mainNode.getChildNode( "spriteList" );
    if( !staticSpriteNode.isEmpty() )
    {
        for( int i = 0; i < staticSpriteNode.nChildNode(); ++i )
        {
            XMLNode spriteNode = staticSpriteNode.getChildNode( "sprite", i );

            // Load in the static sprite
            loadStaticSpriteFromNode( spriteNode );
        }
    }

    // Get the static menu controls node
    XMLNode staticControlsNode = mainNode.getChildNode( "staticMenuControls" );
    if( !staticControlsNode.isEmpty() )
    {
        m_pStaticControlVec.reserve( staticControlsNode.nChildNode() );

        for( int i = 0; i < staticControlsNode.nChildNode(); ++i )
        {
            // Get the menu controls
            XMLNode controlNode = staticControlsNode.getChildNode( "control", i );

            // Load static controls from an XML node
            loadStaticControlFromNode( controlNode );
        }
    }

    // Get the mouse only menu controls node
    XMLNode mouseOnlyMenuControlsNode = mainNode.getChildNode( "mouseOnlyControls" );
    if( !mouseOnlyMenuControlsNode.isEmpty() )
    {
        m_pMouseOnlyControlVec.reserve( mouseOnlyMenuControlsNode.nChildNode() );

        for( int i = 0; i < mouseOnlyMenuControlsNode.nChildNode(); ++i )
        {
            // Get the menu controls
            XMLNode controlNode = mouseOnlyMenuControlsNode.getChildNode( "control", i );

            // Load static controls from an XML node
            loadMouseOnlyControlFromNode( controlNode );
        }
    }

    // Get the menu controls
    XMLNode menuControlsNode = mainNode.getChildNode( "menuControls" );
    if( !menuControlsNode.isEmpty() )
    {
        // map to help setup the node pointers
        NavHelperMap navNodeMap;

        m_pControlVec.reserve( menuControlsNode.nChildNode() );

        // Load the controls
        for( int i = 0; i < menuControlsNode.nChildNode(); ++i )
        {
            XMLNode controlNode = menuControlsNode.getChildNode( "control", i );

            // Load in the control
            loadControlFromNode( controlNode, navNodeMap );
        }

        // Map the controls to their respective nodes
        for( int i = 0; i < menuControlsNode.nChildNode(); ++i )
        {
            XMLNode controlNode = menuControlsNode.getChildNode( "control", i );

            // Find the reference nodes
            findNodes( controlNode, i, navNodeMap );
        }
    }
}

/************************************************************************
*    DESC:  Load the script functions from node and add them to the map
************************************************************************/
void CMenu::loadScriptFromNode( const XMLNode & node )
{
    CObject::loadScriptFromNode( node, m_group );
}

/************************************************************************
*    DESC:  Load a static sprite from an XML node
************************************************************************/
void CMenu::loadStaticSpriteFromNode( const XMLNode & node )
{
    // Get the type of object
    std::string objectName = node.getAttribute( "objectName" );

    // Allocate the static sprite in the container
    m_pSpriteVec.push_back( new CSprite( CObjectDataMgr::Instance().getData( m_group, objectName ) ) );

    // Load the transform data
    m_pSpriteVec.back()->loadTransFromNode( node );

    // Init the script functions
    m_pSpriteVec.back()->loadScriptFromNode( node );
}

/************************************************************************
*    DESC:  Load static controls from an XML node
************************************************************************/
void CMenu::loadStaticControlFromNode( XMLNode & node )
{
    // New up the control with its respected control type
    m_pStaticControlVec.push_back( NUIControlFactory::Create( node, m_group ) );

    // Does this control have a name then add it to the map
    if( !m_pStaticControlVec.back()->getName().empty() )
    {
        // Map of menu controls
        m_pControlMap.emplace( m_pStaticControlVec.back()->getName(), m_pStaticControlVec.back() );
    }
}

/************************************************************************
*    DESC:  Load mouse only controls from an XML node
************************************************************************/
void CMenu::loadMouseOnlyControlFromNode( const XMLNode & node )
{
    // New up the control with its respected control type
    m_pMouseOnlyControlVec.push_back( NUIControlFactory::Create( node, m_group ) );

    // Does this control have a name then add it to the map
    if( !m_pMouseOnlyControlVec.back()->getName().empty() )
        m_pControlMap.emplace( m_pMouseOnlyControlVec.back()->getName(), m_pMouseOnlyControlVec.back() );
}

/************************************************************************
*    DESC:  Load a control from an XML node
************************************************************************/
void CMenu::loadControlFromNode( XMLNode & node, NavHelperMap & navNodeMap )
{
    // Add the control to the vector
    m_pControlVec.push_back( NUIControlFactory::Create( node, m_group ) );

    // Get the control name
    const std::string controlName = m_pControlVec.back()->getName();

    // Does this control have a name then add it to the map
    if( !controlName.empty() )
    {
        // Map of menu controls
        auto iter = m_pControlMap.emplace( controlName, m_pControlVec.back() );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Control Load Error!",
                boost::str( boost::format("Duplicate control name (%s).\n\n%s\nLine: %s")
                    % controlName % __FUNCTION__ % __LINE__ ));
        }

        // Add a node to the vector with it's control
        m_pControlNodeVec.push_back( new iControlNavNode( m_pControlVec.back() ) );

        // Map of menu control nodes
        navNodeMap.emplace( controlName, m_pControlNodeVec.back() );
    }
}

/************************************************************************
*    DESC:  Load the dynamic offset data from node
************************************************************************/
void CMenu::loadDynamicOffsetFromNode( const XMLNode & node )
{
    // Load the dynamic offset
    bool dummy;
    m_dynamicOffset = NParseHelper::LoadDynamicOffset( node, dummy );

    // Set the dynamic position
    setDynamicPos();
}

/************************************************************************
*    DESC:  Set the dynamic position
************************************************************************/
void CMenu::setDynamicPos()
{
    // Position the menu based on the dynamic offset
    if( !m_dynamicOffset.isEmpty() )
        setPos( m_dynamicOffset.getPos() );
}

/************************************************************************
*    DESC:  Reset the dynamic position
************************************************************************/
void CMenu::resetDynamicPos()
{
    setDynamicPos();

    for( auto iter : m_pStaticControlVec )
        iter->setDynamicPos();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->setDynamicPos();

    for( auto iter : m_pControlVec )
        iter->setDynamicPos();
}

/************************************************************************
*    DESC:  Find the reference nodes
************************************************************************/
void CMenu::findNodes(
    const XMLNode & node,
    int nodeIndex,
    NavHelperMap & navNodeMap )
{
    const XMLNode navNode = node.getChildNode( "navigate" );
    if( !navNode.isEmpty() )
    {
        setNodes( navNode, nodeIndex, "up",    EAction::UP,    navNodeMap );
        setNodes( navNode, nodeIndex, "down",  EAction::DOWN,  navNodeMap );
        setNodes( navNode, nodeIndex, "left",  EAction::LEFT,  navNodeMap );
        setNodes( navNode, nodeIndex, "right", EAction::RIGHT, navNodeMap );
    }
}

/************************************************************************
*    DESC:  Find the reference nodes
************************************************************************/
void CMenu::setNodes(
    const XMLNode & node,
    int nodeIndex,
    std::string attr,
    EAction action,
    NavHelperMap & navNodeMap )
{
    if( node.isAttributeSet( attr.c_str() ) )
    {
        const std::string name = node.getAttribute( attr.c_str() );
        auto iter = navNodeMap.find( name );
        if( iter != navNodeMap.end() )
        {
            m_pControlNodeVec.at(nodeIndex)->setNode( action, iter->second );
        }
        else
        {
            throw NExcept::CCriticalException("Control Node Find Error!",
                boost::str( boost::format("Control node doesn't exist (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }
    }
}

/************************************************************************
*    DESC:  Init the menu controls
************************************************************************/
void CMenu::init()
{
    for( auto iter : m_pStaticControlVec )
        iter->init();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->init();

    for( auto iter : m_pControlVec )
        iter->init();
    
    // Prepare any script functions that are flagged to prepareOnInit
    prepareOnInit();
}

/************************************************************************
*    DESC:  Init the root menu
************************************************************************/
void CMenu::initRootMenu()
{
    m_state = EMenuState::IDLE;
    if( !prepare( "initRootMenu" ) )
    {
        setAlpha(1.f);
        setVisible(true);
        activateFirstInactiveControl();
    }
}

/************************************************************************
*    DESC:  Update the menu
************************************************************************/
void CMenu::update()
{
    m_scriptComponent.update();

    for( auto iter : m_pSpriteVec )
        iter->update();

    for( auto iter : m_pStaticControlVec )
        iter->update();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->update();

    for( auto iter : m_pControlVec )
        iter->update();
}

/************************************************************************
*    DESC:  Transform the menu
************************************************************************/
void CMenu::transform()
{
    CObject::transform();

    for( auto iter : m_pSpriteVec )
        iter->transform( *this );

    for( auto iter : m_pStaticControlVec )
        iter->transform( *this );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->transform( *this );

    for( auto iter : m_pControlVec )
        iter->transform( *this );
}

void CMenu::transform( const CObject & object )
{
    CObject::transform( object );

    for( auto iter : m_pSpriteVec )
        iter->transform( *this );

    for( auto iter : m_pStaticControlVec )
        iter->transform( *this );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->transform( *this );

    for( auto iter : m_pControlVec )
        iter->transform( *this );
}

/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CMenu::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera )
{
    if( isVisible() )
    {
        for( auto iter : m_pSpriteVec )
            iter->recordCommandBuffer( index, cmdBuf, camera );

        for( auto iter : m_pStaticControlVec )
            iter->recordCommandBuffer( index, cmdBuf, camera );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->recordCommandBuffer( index, cmdBuf, camera );

        for( auto iter : m_pControlVec )
            iter->recordCommandBuffer( index, cmdBuf, camera );
    }
}

/************************************************************************
*    DESC:  Get the name of the menu
************************************************************************/
const std::string & CMenu::getName() const
{
    return m_name;
}

/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CMenu::handleEvent( const SDL_Event & rEvent )
{
    // Have the controls handle events
    for( auto iter : m_pControlVec )
        iter->handleEvent( rEvent );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->handleEvent( rEvent );

    if( rEvent.type == NMenuEvent::ROOT_TRANS_IN )
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
    else if( rEvent.type == NMenuEvent::REACTIVATE )
    {
        onReactivate( rEvent );
    }
    else if( m_state == EMenuState::IDLE )
    {
        if( rEvent.type == NMenuEvent::SELECT_ACTION )
        {
            onSelectAction( rEvent );
        }
        else if( rEvent.type == NMenuEvent::SET_ACTIVE_CONTROL )
        {
            onSetActiveControl( rEvent );
        }
        else if( rEvent.type == NMenuEvent::SCROLL_UP )
        {
            onUpAction( rEvent );
        }
        else if( rEvent.type == NMenuEvent::SCROLL_DOWN )
        {
            onDownAction( rEvent );
        }
        else if( rEvent.type == NMenuEvent::SCROLL_LEFT )
        {
            onLeftAction( rEvent );
        }
        else if( rEvent.type == NMenuEvent::SCROLL_RIGHT )
        {
            onRightAction( rEvent );
        }
        else if( rEvent.type == SDL_EVENT_MOUSE_MOTION )
        {
            onMouseMove( rEvent );
        }
        else if( (rEvent.type >= NMenuEvent::UP_ACTION) &&
                 (rEvent.type <= NMenuEvent::RIGHT_ACTION) )
        {
            if( rEvent.user.code == static_cast<int>(EActionPress::DOWN) )
            {
                if( rEvent.type == NMenuEvent::UP_ACTION )
                {
                    onUpAction( rEvent );
                }
                else if( rEvent.type == NMenuEvent::DOWN_ACTION )
                {
                    onDownAction( rEvent );
                }
                if( rEvent.type == NMenuEvent::LEFT_ACTION )
                {
                    onLeftAction( rEvent );
                }
                else if( rEvent.type == NMenuEvent::RIGHT_ACTION )
                {
                    onRightAction( rEvent );
                }
            }
        }
    }

    // Prepare script functions associated with handling game events
    prepare( "event", rEvent.type, rEvent.user.code );
}

/************************************************************************
*    DESC:  Handle onUpAction message
************************************************************************/
void CMenu::onUpAction( const SDL_Event & rEvent )
{
    navigateMenu( EAction::UP );
}

/************************************************************************
*    DESC:  Handle onMenuDown message
************************************************************************/
void CMenu::onDownAction( const SDL_Event & rEvent )
{
    navigateMenu( EAction::DOWN );
}

/************************************************************************
*    DESC:  Handle onMenuLeft message
************************************************************************/
void CMenu::onLeftAction( const SDL_Event & rEvent )
{
    navigateMenu( EAction::LEFT );
}

/************************************************************************
*    DESC:  Handle onRightAction message
************************************************************************/
void CMenu::onRightAction( const SDL_Event & rEvent )
{
    navigateMenu( EAction::RIGHT );
}

/************************************************************************
*    DESC:  Navigate the menu. Find the next control node that isn't
*           disabled and make it the active control node
************************************************************************/
void CMenu::navigateMenu( EAction action )
{
    if( m_pActiveNode != nullptr )
    {
        iControlNavNode * pNavNode = m_pActiveNode;

        do
        {
            pNavNode = pNavNode->getNode( action );

            if( pNavNode == nullptr )
            {
                break;
            }
            else if( !pNavNode->getControl()->isDisabled() )
            {
                m_pActiveNode = pNavNode;

                NGenFunc::DispatchEvent(
                    NMenuEvent::CONTROL_STATE_CHANGE,
                    static_cast<int>(EControlState::ACTIVE),
                    pNavNode->getControl() );

                break;
            }
        }
        while( true );
    }
}

/************************************************************************
*    DESC:  Handle onMouseMove message
************************************************************************/
void CMenu::onMouseMove( const SDL_Event & rEvent )
{
    for( auto iter : m_pControlNodeVec )
    {
        if( iter->getControl()->onMouseMove( rEvent ) )
            m_pActiveNode = iter;
        else
            iter->getControl()->deactivateControl();
    }

    for( auto iter : m_pMouseOnlyControlVec )
        if( !iter->onMouseMove( rEvent ) )
            iter->deactivateControl();
}

/************************************************************************
*    DESC:  Handle onSelectAction message
************************************************************************/
void CMenu::onSelectAction( const SDL_Event & rEvent )
{
    bool selectionFound(false);
    CSelectMsgCracker msgCracker;
    msgCracker.setPackedUnit( rEvent.user.code );

    if( (m_pActiveNode != nullptr) &&
        (m_pActiveNode->getControl()->handleSelectAction( msgCracker )) )
    {
        selectionFound = true;

        // Set the state to active which will block all messages until the state is reset to idle
        auto pCtrl = m_pActiveNode->getControl()->getPtrToActiveControl();
        if( (pCtrl != nullptr) && (pCtrl->getActionType() > EControlActionType::IDLE) )
            m_state = EMenuState::ACTIVE;
    }
    else if( msgCracker.isDeviceMouse() )
    {
        // For mouse only controls
        for( auto iter : m_pMouseOnlyControlVec )
        {
            if( iter->handleSelectAction( msgCracker ) )
            {
                selectionFound = true;

                // Set the state to active which will block all messages until the state is reset to idle
                if( iter->getActionType() > EControlActionType::IDLE )
                    m_state = EMenuState::ACTIVE;

                break;
            }
        }
    }

    // Try to handle touch presses on a non-active control
    // The mouse just happends to be clicked over a non-active control
    if( !selectionFound && msgCracker.isDeviceMouse() )
    {
        // Deactivate the control that should be active
        if( (m_pActiveNode != nullptr) &&
            (msgCracker.getPressType() == m_pActiveNode->getControl()->getMouseSelectType()) )
        {
            m_pActiveNode->getControl()->deactivateControl();

            // Go through all the controls on this menu to try to find the one clicked on
            for( auto iter : m_pControlVec )
            {
                if( iter->handleSelectAction( msgCracker ) )
                {
                    // Set the state to active which will block all messages until the state is reset to idle
                    auto pCtrl = m_pActiveNode->getControl()->getPtrToActiveControl();
                    if( (pCtrl != nullptr) && (pCtrl->getActionType() > EControlActionType::IDLE) )
                        m_state = EMenuState::ACTIVE;

                    break;
                }
            }
        }
    }
}

/************************************************************************
*    DESC:  Handle onSetActiveControl message
************************************************************************/
void CMenu::onSetActiveControl( const SDL_Event & rEvent )
{
    // Set the first inactive control to active
    if( rEvent.user.code == NActiveControl::FIRST )
        activateFirstInactiveControl();
}

/************************************************************************
*    DESC:  Handle onReactivate message
************************************************************************/
void CMenu::onReactivate( const SDL_Event & rEvent )
{
    m_state = EMenuState::IDLE;
}

/************************************************************************
*    DESC:  Handle onRootTransIn message
************************************************************************/
void CMenu::onRootTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NTransCode::BEGIN )
    {
        if( !prepare( "rootTransIn" ) )
        {
            setAlpha(1.f);
            setVisible(true);
            NGenFunc::DispatchEvent( NMenuEvent::ROOT_TRANS_IN, NTransCode::END );
        }

        m_state = EMenuState::ACTIVE;
    }
    else if( rEvent.user.code == NTransCode::END )
    {
        m_state = EMenuState::IDLE;
    }
}

/************************************************************************
*    DESC:  Handle onTransIn message
************************************************************************/
void CMenu::onTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NTransCode::BEGIN )
    {
        if( !prepare( "transIn" ) )
        {
            setAlpha(1.f);
            setVisible(true);
            NGenFunc::DispatchEvent( NMenuEvent::TRANS_IN, NTransCode::END );
        }

        m_state = EMenuState::ACTIVE;
    }
    else if( rEvent.user.code == NTransCode::END )
    {
        m_state = EMenuState::IDLE;
    }
}

/************************************************************************
*    DESC:  Handle onTransOut message
************************************************************************/
void CMenu::onTransOut( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NTransCode::BEGIN )
    {
        if( !prepare( "transOut" ) )
        {
            setAlpha(0.f);
            setVisible(false);
            NGenFunc::DispatchEvent( NMenuEvent::TRANS_OUT, NTransCode::END );
        }

        m_state = EMenuState::ACTIVE;
    }
    else if( rEvent.user.code == NTransCode::END )
    {
        m_state = EMenuState::INACTIVE;
    }
}

/************************************************************************
*    DESC:  Prepare the script function to run
************************************************************************/
bool CMenu::prepare( const std::string & scriptFuncId, uint type, int code )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        if( scriptFuncId == "event" )
            m_scriptComponent.prepare( iter->second.group, iter->second.funcName, {this, type, code} );
        else
            m_scriptComponent.prepare( iter->second.group, iter->second.funcName, {this} );
        
        // Force an update
        if( iter->second.forceUpdate )
            m_scriptComponent.update();
        
        return true;
    }

    return false;
}

/************************************************************************
*    DESC:  Set the first inactive control to be active
************************************************************************/
void CMenu::activateFirstInactiveControl()
{
    bool found(false);

    // Activate the first control found and deactivate all the rest
    for( auto iter : m_pControlNodeVec )
    {
        if( !found && iter->getControl()->activateFirstInactiveControl() )
        {
            m_pActiveNode = iter;

            found = true;
        }
        else
        {
            iter->getControl()->deactivateControl();
        }
    }
}

/************************************************************************
*    DESC:  Deactivate all controls
************************************************************************/
void CMenu::deactivateAllControls()
{
    for( auto iter : m_pControlNodeVec )
        iter->getControl()->deactivateControl();
}

/************************************************************************
*    DESC:  Reset all controls
************************************************************************/
void CMenu::reset()
{
    for( auto iter : m_pControlVec )
        iter->reset( true );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->reset( true );
}

/************************************************************************
*    DESC:  Get the pointer to the control in question
************************************************************************/
iControl * CMenu::getPtrToControl( const std::string & name )
{
    // See if the control can be found
    auto iter = m_pControlMap.find( name );

    // Make sure control is available
    if( iter == m_pControlMap.end() )
        throw NExcept::CCriticalException("Menu Control Error!",
            boost::str( boost::format("Control being asked for is missing!(%s)\n\n%s\nLine: %s")
                % name.c_str() % __FUNCTION__ % __LINE__ ));

    // Pass back the pointer if found
    return iter->second;
}

/************************************************************************
*    DESC:  Get the pointer to the active control
************************************************************************/
iControl * CMenu::getPtrToActiveControl()
{
    iControl * pResult(nullptr);

    for( auto iter : m_pControlVec )
    {
        if( iter->getState() > EControlState::INACTIVE )
        {
            pResult = iter->getPtrToActiveControl();
            break;
        }
    }

    return pResult;
}

/************************************************************************
*    DESC:  Does this menu use dynamic offsets
************************************************************************/
bool CMenu::isDynamicOffset()
{
    return !m_dynamicOffset.isEmpty();
}

/************************************************************************
*    DESC:  Get the scroll params
************************************************************************/
CScrollParam & CMenu::getScrollParam( int msg )
{
    if( (m_pActiveNode != nullptr) &&
        m_pActiveNode->getControl()->canScroll(msg) )
    {
        return m_pActiveNode->getControl()->getScrollParam();
    }

    return m_scrollParam;
}

/************************************************************************
*    DESC:  Set/Get the alpha value of this menu
************************************************************************/
void CMenu::setAlpha( float alpha )
{
    for( auto iter : m_pSpriteVec )
        iter->getVisualComponent()->setAlpha( alpha );

    for( auto iter : m_pStaticControlVec )
        iter->setAlpha( alpha );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->setAlpha( alpha );

    for( auto iter : m_pControlVec )
        iter->setAlpha( alpha );

    m_alpha = alpha;
}

float CMenu::getAlpha()
{
    return m_alpha;
}

/************************************************************************
*    DESC:  Is the menu idle
************************************************************************/
bool CMenu::isIdle()
{
    return (m_state == EMenuState::IDLE);
}

/************************************************************************
*    DESC:  Is the menu active
************************************************************************/
bool CMenu::isActive()
{
    return (m_state == EMenuState::ACTIVE);
}
