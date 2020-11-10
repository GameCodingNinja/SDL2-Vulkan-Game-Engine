
/************************************************************************
*    FILE NAME:       uiprogressbar.cpp
*
*    DESCRIPTION:     Class for user interface progress bar
************************************************************************/

// Physical component dependency
#include <gui/uiprogressbar.h>

// Game lib dependencies
#include <sprite/sprite.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>
#include <utilities/xmlParser.h>
#include <common/ivisualcomponent.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIProgressBar::CUIProgressBar( const std::string & group ) :
    CUIControl( group ),
    m_curValue(0),
    m_minValue(0),
    m_maxValue(100),
    m_spriteApplyIndex(0),
    m_orentation(EOrientation::HORZ)
{
    m_type = EControlType::PROGRESS_BAR;
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CUIProgressBar::~CUIProgressBar()
{
}

/************************************************************************
*    DESC:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CUIProgressBar::loadFromNode( const XMLNode & node )
{
    CUIControl::loadFromNode( node );

    // See if a range of values was specified
    const XMLNode rangeNode = node.getChildNode( "range" );
    if( !rangeNode.isEmpty() )
    {
        if( rangeNode.isAttributeSet( "cur" ) )
            m_curValue = std::atof( rangeNode.getAttribute( "cur" ) );

        if( rangeNode.isAttributeSet( "min" ) )
            m_minValue = std::atof( rangeNode.getAttribute( "min" ) );

        if( rangeNode.isAttributeSet( "max" ) )
            m_maxValue = std::atof( rangeNode.getAttribute( "max" ) );
    }

    const XMLNode orentNode = node.getChildNode( "orentation" );
    if( !orentNode.isEmpty() )
    {
        if( orentNode.isAttributeSet("type") )
            if( std::strcmp( orentNode.getAttribute("type"), "vert") == 0 )
                m_orentation = EOrientation::VERT;

        if( orentNode.isAttributeSet("alignment") )
        {
            const char * pAlign = orentNode.getAttribute("alignment");

            if( m_orentation == EOrientation::HORZ )
            {
                if( std::strcmp( pAlign, "right") == 0 )
                    m_alignment.horz = EHorzAlignment::HORZ_RIGHT;

                else if( std::strcmp( pAlign, "center") == 0 )
                    m_alignment.horz = EHorzAlignment::HORZ_CENTER;
            }
            else
            {
                if( std::strcmp( pAlign, "bottom") == 0 )
                    m_alignment.vert = EVertAlignment::VERT_BOTTOM;

                else if( std::strcmp( pAlign, "center") == 0 )
                    m_alignment.vert = EVertAlignment::VERT_CENTER;
            }
        }
    }

    // Calculate the progress bar size and position
    setSizePos();
}

/************************************************************************
*    DESC:  Load the control specific info from XML node
*
*    param: controlNode - XML node
************************************************************************/
void CUIProgressBar::loadControlFromNode( const XMLNode & controlNode )
{
    CUIControl::loadControlFromNode( controlNode );

    // Get the stencil mask node
    const XMLNode stencilMaskNode = controlNode.getChildNode( "stencilMask" );
    if( !stencilMaskNode.isEmpty() )
    {
        const std::string objectName = stencilMaskNode.getAttribute( "objectName" );

        m_spriteApplyIndex = std::atoi(stencilMaskNode.getAttribute( "spriteIndex" ));

        if( !objectName.empty() )
        {
            m_upStencilMaskSprite.reset( new CSprite( CObjectDataMgr::Instance().getData( getGroup(), objectName ) ) );

            // Load the transform data
            m_upStencilMaskSprite->loadTransFromNode( stencilMaskNode );

            // Get the size
            m_size = m_upStencilMaskSprite->getVisualComponent()->getSize();

            // Get the initial position
            m_pos = m_upStencilMaskSprite->getPos();

            // Get the initial scale
            m_scale = m_upStencilMaskSprite->getScale();
        }
        else
        {
            // Get the size
            m_size = m_pSpriteVec.at(m_spriteApplyIndex)->getVisualComponent()->getSize();

            // Get the initial position
            m_pos = m_pSpriteVec.at(m_spriteApplyIndex)->getPos();

            // Get the initial scale
            m_scale = m_pSpriteVec.at(m_spriteApplyIndex)->getScale();
        }
    }
}

/************************************************************************
*    DESC:  Transform the control
************************************************************************/
void CUIProgressBar::transform( const CObject & object )
{
    CUIControl::transform( object );

    if( m_upStencilMaskSprite )
        m_upStencilMaskSprite->transform( *this );
}

/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CUIProgressBar::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera )
{
    if( m_upStencilMaskSprite )
    {
        for( size_t i  = 0; i < m_pSpriteVec.size(); ++i )
        {
            if( static_cast<int>(i) == m_spriteApplyIndex )
            {
                // Record the command buffer for the stencil mask
                m_upStencilMaskSprite->recordCommandBuffer( index, cmdBuf, camera );

                // Record the command buffer for the inside of the stencil mask
                m_pSpriteVec[i]->recordCommandBuffer( index, cmdBuf, camera );
            }
            else
                m_pSpriteVec[i]->recordCommandBuffer( index, cmdBuf, camera );
        }
    }
    else
    {
        CUIControl::recordCommandBuffer( index, cmdBuf, camera );
    }
}

/************************************************************************
*    DESC:  Calculate the progress bar size and position
************************************************************************/
void CUIProgressBar::setSizePos()
{
    CPoint<float> scale(m_scale);
    CPoint<float> pos(m_pos);

    // Calculate the new scale for the progress bar
    float scaler = (m_curValue - m_minValue) / (m_maxValue - m_minValue);

    if( m_orentation == EOrientation::HORZ )
    {
        scale.x *= scaler;
        float offset = m_size.w * scaler;

        if( m_alignment.horz == EHorzAlignment::HORZ_LEFT )
            pos.x -= (m_size.w - offset) / 2;

        else if( m_alignment.horz == EHorzAlignment::HORZ_RIGHT )
            pos.x += (m_size.w - offset) / 2;
    }
    else
    {
        scale.y *= scaler;
        float offset = m_size.h * scaler;

        if( m_alignment.vert == EVertAlignment::VERT_TOP )
            pos.y += (m_size.h - offset) / 2;

        else if( m_alignment.vert == EVertAlignment::VERT_BOTTOM )
            pos.y -= (m_size.h - offset) / 2;
    }

    if( m_upStencilMaskSprite )
    {
        m_upStencilMaskSprite->setScale( scale );
        m_upStencilMaskSprite->setPos( pos );
    }
    else
    {
        m_pSpriteVec.at(m_spriteApplyIndex)->setScale( scale );
        m_pSpriteVec.at(m_spriteApplyIndex)->setPos( pos );
    }
}

/************************************************************************
*    DESC:  Set/Get/Inc current value
************************************************************************/
void CUIProgressBar::setProgressBarValue( float value )
{
    m_curValue = value;
}

float CUIProgressBar::getProgressBarValue()
{
    return m_curValue;
}

void CUIProgressBar::incProgressBarValue( float value )
{
    m_curValue += value;
}

/************************************************************************
*    DESC:  Set/Get min value
************************************************************************/
void CUIProgressBar::setProgressBarMinValue( float value )
{
    m_minValue = value;
}

float CUIProgressBar::getProgressBarMinValue()
{
    return m_minValue;
}

/************************************************************************
*    DESC:  Set/Get max value
************************************************************************/
void CUIProgressBar::setProgressBarMaxValue( float value )
{
    m_maxValue = value;
}

float CUIProgressBar::getProgressBarMaxValue()
{
    return m_maxValue;
}
