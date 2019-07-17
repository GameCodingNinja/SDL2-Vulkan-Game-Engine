
/************************************************************************
*    FILE NAME:       symbol2d.cpp
*
*    DESCRIPTION:     Class for rendering a symbol and it's animations
************************************************************************/

// Physical component dependency
#include <slot/symbol2d.h>

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <common/ivisualcomponent.h>
#include <common/fontdata.h>
#include <sprite/spritedata.h>
#include <sprite/sprite.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSymbol2d::CSymbol2d( const std::vector<CSpriteData> & rSpriteDataVec, const std::string & id ) :
    m_id(id),
    m_deferredRender(false)
{
    for( auto & iter : rSpriteDataVec )
    {
        m_spriteDeq.emplace_back( CObjectDataMgr::Instance().getData( iter.getGroup(), iter.getObjectName() ) );

        m_spriteDeq.back().getObject()->setVisible( iter.isVisible() );

        if( m_spriteDeq.back().getVisualComponent()->isFontSprite() )
        {
            m_spriteDeq.back().getVisualComponent()->setFontProperties( iter.getFontData()->m_fontProp );
            m_spriteDeq.back().getVisualComponent()->createFontString( iter.getFontData()->m_fontString );
        }

        m_spriteDeq.back().getObject()->copyTransform( &iter );

        m_spriteDeq.back().copyScriptFunctions( iter.getScriptFunctions() );
    }
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSymbol2d::~CSymbol2d()
{
}


/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite & CSymbol2d::getSprite( int index )
{
    return m_spriteDeq.at( index );
}


/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
size_t CSymbol2d::getSpriteCount()
{
    return m_spriteDeq.size();
}


/************************************************************************
*    DESC:  Get the string id
************************************************************************/
const std::string & CSymbol2d::getId()
{
    return m_id;
}


/************************************************************************
*    DESC:  Update the symbol
************************************************************************/
void CSymbol2d::update()
{
    for( auto & iter : m_spriteDeq )
        iter.update();
}


/************************************************************************
*    DESC:  Transform
************************************************************************/
void CSymbol2d::transform()
{
    m_parameters.remove( NDefs::WAS_TRANSFORMED );

    if( m_parameters.isSet( NDefs::TRANSFORM ) )
    {
        transformLocal( m_finalMatrix );

        m_transPos = m_pos;
    }

    for( auto & iter : m_spriteDeq )
        iter.getObject()->transform( m_finalMatrix, wasWorldPosTranformed() );
}

void CSymbol2d::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_parameters.remove( NDefs::WAS_TRANSFORMED );

    if( m_parameters.isSet( NDefs::TRANSFORM ) )
        transformLocal( m_matrix );

    if( m_parameters.isSet( NDefs::WAS_TRANSFORMED ) || tranformWorldPos )
    {
        m_parameters.add( NDefs::WAS_TRANSFORMED );

        m_finalMatrix.copy( m_matrix );

        m_finalMatrix.mergeMatrix( matrix );
    }

    for( auto & iter : m_spriteDeq )
        iter.getObject()->transform( m_finalMatrix, wasWorldPosTranformed() );
}


/************************************************************************
*    DESC:  do the render
************************************************************************/
/*void CSymbol2d::render( const CMatrix & matrix )
{
    for( auto & iter : m_spriteDeq )
        iter.render( matrix );
}*/


/************************************************************************
*    DESC:  Get/Set if deferred render
************************************************************************/
bool CSymbol2d::isDeferredRender()
{
    return m_deferredRender;
}

void CSymbol2d::setDeferredRender( bool value )
{
    m_deferredRender = value;
}
