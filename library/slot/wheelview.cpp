
/************************************************************************
*    FILE NAME:       wheelview.cpp
*
*    DESCRIPTION:     View class for the wheel implementation.
*                     NOTE: Even though this is a wheel class and wheels have wedges, we're
*                           using the Symbol2D as the wedge because it does the same thing.
************************************************************************/

// Physical component dependency
#include <slot/wheelview.h>

// Game lib dependencies
#include <slot/slotstripmodel.h>
#include <slot/symbolsetview.h>
#include <slot/mathsymbol.h>
#include <slot/stripstop.h>
#include <utilities/matrix.h>
#include <utilities/exceptionhandling.h>
#include <utilities/highresolutiontimer.h>
#include <objectdata/objectdatamanager.h>
#include <managers/soundmanager.h>
#include <common/defs.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CWheelView::CWheelView( const CSlotStripModel & rSlotStripModel, CSymbolSetView & rSymbolSetView, const int wheelId ) :
    m_rSlotStripModel(rSlotStripModel),
    m_rSymbolSetView(rSymbolSetView),
    m_wheelId(wheelId),
    m_spinState(NSlotDefs::ESS_STOPPED),
    m_spinDir(NSlotDefs::EDS_CLOCKWISE),
    m_spin(false),
    m_velocity(0.0),
    m_acceleration(0.0),
    m_spinDirVector(-1),
    m_PI_2(M_PI * 2),
    m_winPointDegree(0.0),
    m_degreePerWedge(m_PI_2 / rSlotStripModel.getStripVec().size()),
    m_saftyCheckDegree(0.0),
    m_rotation(0.0),
    m_currentRotation(0.0),
    m_disableSpinTimer(false),
    m_pSpinStopSnd(nullptr),
    m_allowStopSounds(true)
{
    // Set the value returned by Expired when the timer is disabled
    m_spinTimer.setDisableValue( true );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CWheelView::~CWheelView()
{
}


/************************************************************************
*    DESC:  Create the reel strip from data node
************************************************************************/
void CWheelView::create( const XMLNode & node, const std::string & group )
{
    // Load the transform data from node
    const XMLNode transNode = node.getChildNode( "translation" );
    if( !transNode.isEmpty() )
        loadTransFromNode( transNode );

    // Set the spin direction
    if( node.isAttributeSet("spinDirection") )
    {
        m_spinDir = NSlotDefs::ESpinDirection( std::atoi(node.getAttribute( "spinDirection" )) );

        if( m_spinDir == NSlotDefs::EDS_COUNTERCLOCKWISE )
            m_spinDirVector = 1;
    }

    // Load the wheel sprites from node
    loadWheelSprites( node, group );

    // Load the wheel wedges from node
    loadWedges( node, group );

    // Load the sprites from node. These are sprites that don't spin with the wheel
    loadSprites( node, group );
}


/************************************************************************
*    DESC:  Load the wheel wedges from node
*           NOTE: These sprites spin with the wheel
************************************************************************/
void CWheelView::loadWheelSprites( const XMLNode & node, const std::string & group )
{
    const XMLNode wheelSpriteLstNode = node.getChildNode( "wheelSpriteList" );
    if( !wheelSpriteLstNode.isEmpty() )
    {
        const XMLNode spriteLstNode = wheelSpriteLstNode.getChildNode( "spriteList" );

        for( int i = 0; i < spriteLstNode.nChildNode(); ++i )
        {
            // Sanity check for the object name
            const XMLNode spriteNode = spriteLstNode.getChildNode( i );
            if( !spriteNode.isAttributeSet( "objectName" ) )
                throw NExcept::CCriticalException("Wheel View Creation Error!",
                    boost::str( boost::format("Wheel view object name missing (%s)!\n\n%s\nLine: %s")
                        % group % __FUNCTION__ % __LINE__ ));

            m_wheelSpriteDeq.emplace_back( CObjectDataMgr::Instance().getData2D( group, spriteNode.getAttribute("objectName") ) );
            m_wheelSpriteDeq.back().loadTransFromNode( spriteNode );
        }
    }
}


/************************************************************************
*    DESC:  Load the wheel wedges from node
************************************************************************/
void CWheelView::loadWedges( const XMLNode & node, const std::string & group )
{
    const XMLNode wedgeLstNode = node.getChildNode( "wedgeList" );
    if( !wedgeLstNode.isEmpty() )
    {
        for( int i = 0; i < wedgeLstNode.nChildNode(); ++i )
        {
            const XMLNode wedgeNode = wedgeLstNode.getChildNode( i );
            if( !wedgeNode.isAttributeSet( "symb" ) )
                throw NExcept::CCriticalException("Wheel View Wedge Creation Error!",
                    boost::str( boost::format("Wheel view wedge symbol ID missing (%s)!\n\n%s\nLine: %s")
                        % group % __FUNCTION__ % __LINE__ ));

            // Symbol, wedge, it's all the same thing... a container for sprites
            const std::string symbId = wedgeNode.getAttribute( "symb" );
            m_symbolDeq.emplace_back( m_rSymbolSetView.getSpriteData(symbId), symbId );

            m_symbolDeq.back().loadTransFromNode( wedgeNode );
        }
    }
}


/************************************************************************
*    DESC:  Load the wheel wedges from node
*           NOTE: These are sprites that don't spin with the wheel
************************************************************************/
void CWheelView::loadSprites( const XMLNode & node, const std::string & group )
{
    const XMLNode nonRotateSpriteLstNode = node.getChildNode( "nonRotateSpriteList" );
    if( !nonRotateSpriteLstNode.isEmpty() )
    {
        const XMLNode spriteLstNode = nonRotateSpriteLstNode.getChildNode( "spriteList" );

        for( int i = 0; i < spriteLstNode.nChildNode(); ++i )
        {
            // Sanity check for the object name
            const XMLNode spriteNode = spriteLstNode.getChildNode( i );
            if( !spriteNode.isAttributeSet( "objectName" ) )
                throw NExcept::CCriticalException("Wheel View Creation Error!",
                    boost::str( boost::format("Wheel view object name missing (%s)!\n\n%s\nLine: %s")
                        % group % __FUNCTION__ % __LINE__ ));

            m_spriteDeq.emplace_back( CObjectDataMgr::Instance().getData2D( group, spriteNode.getAttribute("objectName") ) );
            m_spriteDeq.back().loadTransFromNode( spriteNode );
        }
    }
}


/************************************************************************
*    DESC:  One time transform to set the non-spinning wheel sprites
************************************************************************/
void CWheelView::preTransform()
{
    for( auto & iter : m_spriteDeq )
        iter.transform( getMatrix(), wasWorldPosTranformed() );
}


/************************************************************************
*    DESC:  Set the spin profile
************************************************************************/
void CWheelView::setSpinProfile( const CSpinProfile & spinProfile )
{
    m_spinProfile = spinProfile;
}


/************************************************************************
*    DESC:  Update the reel strip
************************************************************************/
void CWheelView::update()
{
    if( m_spin )
    {
        switch( m_spinState )
        {
            // Do the spin init and let it fall through to start the spin
            case NSlotDefs::ESS_STOPPED:
            {
                if( m_spinDir == NSlotDefs::EDS_CLOCKWISE )
                    m_winPointDegree = m_PI_2 - (m_degreePerWedge * (m_rSlotStripModel.getStop()));
                else
                {
                    // Special condition for stop zero checks
                    if( m_rSlotStripModel.getStop() == 0 )
                        m_winPointDegree = m_PI_2;
                    else
                        m_winPointDegree = m_degreePerWedge * m_rSlotStripModel.getStop();
                }

                m_velocity = 0.0;
                m_acceleration = m_spinProfile.getAccelation();
                m_saftyCheckDegree = m_degreePerWedge / m_spinProfile.getSafetyCheckDivisor();
                m_spinTimer.set( m_spinProfile.getStartDelay() );
                m_spinState = NSlotDefs::ESS_SPIN_STARTING;

                break;
            }

            case NSlotDefs::ESS_SPIN_STARTING:
            {
                if( m_spinTimer.expired() )
                {
                    // Increment the velocity and acceleration
                    const float elapsedTime = CHighResTimer::Instance().getElapsedTime();
                    m_velocity += m_acceleration * elapsedTime;
                    m_acceleration += m_spinProfile.getImpulse() * elapsedTime;

                    // Cap the velocity at the max speed
                    if( m_velocity >= m_spinProfile.getMaxVelocity() )
                    {
                        m_velocity = m_spinProfile.getMaxVelocity();
                        m_spinTimer.set( m_spinProfile.getMaxVelocityTime() );
                        m_spinState = NSlotDefs::ESS_SPINNING;
                    }

                    incSpin( m_velocity );
                }

                break;
            }

            case NSlotDefs::ESS_SPINNING:
            {
                incSpin( m_velocity );

                // Disable the timer if fast stop is needed
                m_spinTimer.disable( m_disableSpinTimer );

                // Wait for the spin to expire and the rotation to be less then the win point degree
                if( m_spinTimer.expired() && (m_rotation < m_winPointDegree)  )
                    m_spinState = NSlotDefs::ESS_PREPARE_TO_STOP;

                break;
            }

            case NSlotDefs::ESS_PREPARE_TO_STOP:
            {
                incSpin( m_velocity );

                // Wait for the unadjusted rotation to exceed the win point to start slowing down
                if( (m_currentRotation > m_winPointDegree) )
                {
                    // A velocity(4) / PI is the exact deceleration to slow down within 1 rotation
                    float vel = m_spinProfile.getMaxVelocity() * 1000.0;
                    m_acceleration =
                        (m_velocity / ((M_PI * (4.0 / vel)) * m_spinProfile.getDecelerationRotationCount())) / 1000.0;

                    m_spinState = NSlotDefs::ESS_SPIN_STOPPING;
                }

                break;
            }

            case NSlotDefs::ESS_SPIN_STOPPING:
            {
                const float elapsedTime = CHighResTimer::Instance().getElapsedTime();
                m_velocity -= m_acceleration * elapsedTime;

                if( m_velocity < 0.0 )
                {
                    // Stop the spin
                    m_velocity = 0.0;
                    m_spin = false;
                    m_disableSpinTimer = false;

                    // Sanity check the bounds in which we stopped and reset if needed
                    float maxRot = m_winPointDegree + m_saftyCheckDegree;
                    float minRot = m_winPointDegree - m_saftyCheckDegree;

                    // There's a special case for the first stop
                    if( (m_rSlotStripModel.getStop() == 0) && (m_rotation < 6.0) )
                    {
                        if( m_rotation > m_saftyCheckDegree )
                        {
                            m_rotation = maxRot;
                            incSpin( m_velocity );
                        }
                    }
                    else
                    {
                        if( m_rotation > maxRot )
                        {
                            m_rotation = maxRot;
                            incSpin( m_velocity );
                        }
                        else if( m_rotation < minRot )
                        {
                            m_rotation = minRot;
                            incSpin( m_velocity );
                        }
                    }

                    m_spinState = NSlotDefs::ESS_STOPPED;

                    // Send the signal of the spin state
                    m_spinStateSignal(m_wheelId, NSlotDefs::ESS_STOPPED);

                    if( m_allowStopSounds && (m_pSpinStopSnd != nullptr) )
                    {
                        const int channel = CSoundMgr::Instance().getNextChannel();
                        m_pSpinStopSnd->play( channel );
                    }

                    break;
                }

                incSpin( m_velocity );

                break;
            }
        }
    }
}


/************************************************************************
*    DESC:  Inc the wheel spin
************************************************************************/
void CWheelView::incSpin( float velocity )
{
    // Get the rotation traveled
    m_rotation += velocity * CHighResTimer::Instance().getElapsedTime();
    m_currentRotation = m_rotation;

    if( m_rotation >= m_PI_2 )
        m_rotation -= m_PI_2;

    setRot( 0, 0, m_rotation * m_spinDirVector, false );
}


/************************************************************************
*    DESC:  Transform the reel strip
************************************************************************/
void CWheelView::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    CObject2D::transform( matrix, tranformWorldPos );

    for( auto & iter : m_wheelSpriteDeq )
        iter.transform( getMatrix(), wasWorldPosTranformed() );

    for( auto & iter : m_symbolDeq )
        iter.transform( getMatrix(), wasWorldPosTranformed() );
}


/************************************************************************
*    DESC:  Do the render
************************************************************************/
void CWheelView::render( const CMatrix & matrix )
{
    for( auto & iter : m_wheelSpriteDeq )
        iter.render( matrix );

    for( auto & iter : m_symbolDeq )
        iter.render( matrix );

    for( auto & iter : m_spriteDeq )
        iter.render( matrix );
}


/************************************************************************
*    DESC:  Do the deferred render
************************************************************************/
void CWheelView::deferredRender( const CMatrix & matrix )
{

}


/************************************************************************
*    DESC:  Start the spin
************************************************************************/
void CWheelView::startSpin()
{
    m_spin = true;
}


/************************************************************************
*    DESC:  Stop the spin
************************************************************************/
void CWheelView::stopSpin()
{
    if( m_spin && (m_spinState < NSlotDefs::ESS_SPIN_STOPPING) )
        m_disableSpinTimer = true;
}


/************************************************************************
*    DESC:  Get the spin state
************************************************************************/
NSlotDefs::ESpinState CWheelView::getSpinState() const
{
    return m_spinState;
}


/************************************************************************
*    DESC:  Connect to the spin state signal
************************************************************************/
void CWheelView::connect_spinState( const SpinStateSignal::slot_type & slot )
{
    m_spinStateSignal.connect(slot);
}


/************************************************************************
*    DESC:  Do we allow the stop sounds?
************************************************************************/
void CWheelView::allowStopSounds( bool allow )
{
    m_allowStopSounds = allow;
}


/************************************************************************
*    DESC:  Get the number of visible symbols (wedges) on this wheel
************************************************************************/
size_t CWheelView::getVisibleSymbolCount() const
{
    return m_symbolDeq.size();
}


/************************************************************************
*    DESC:  Get the symbol for this spot on the wheel
************************************************************************/
CSymbol2d & CWheelView::getSymbol( int index )
{
    // Get the math symbol
    int stop = m_rSlotStripModel.getStop() + index;

    if( m_symbolDeq.size() == m_rSlotStripModel.getStripVec().size() )
    {
        const int index = m_rSlotStripModel.getSymbolIndex( stop );

        return m_symbolDeq.at(index);
    }

    // TODO: Need to handle this condition of unmatched math and visual symbol counts
    return m_symbolDeq.at(0);
}
