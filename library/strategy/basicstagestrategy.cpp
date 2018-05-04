
/************************************************************************
*    FILE NAME:       basicstagestrategy.cpp
*
*    DESCRIPTION:     Basic stage strategy
************************************************************************/

// Physical component dependency
#include <strategy/basicstagestrategy.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <managers/cameramanager.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CBasicStageStrategy::CBasicStageStrategy()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CBasicStageStrategy::~CBasicStageStrategy()
{
}


/************************************************************************
*    DESC:  Load the sector data from file
************************************************************************/
void CBasicStageStrategy::loadFromFile( const std::string & file )
{
    // open and parse the XML file:
    XMLNode node = XMLNode::openFileHelper( file.c_str(), "stage" );

    XMLNode cameraNode = node.getChildNode( "cameraPosition" );
    if( !cameraNode.isEmpty() )
        m_defaultCameraPos.loadTransFromNode( cameraNode );

    // Load thes sector data from node
    loadFromNode( node );
}


/************************************************************************
*    DESC:  Load thes sector data from node
************************************************************************/
void CBasicStageStrategy::loadFromNode( const XMLNode & node )
{
    XMLNode sectorsNode = node.getChildNode( "sectors" );
    if( !sectorsNode.isEmpty() )
    {
        const XMLNode sectorsNode = node.getChildNode( "sectors" );

        for( int i = 0; i < sectorsNode.nChildNode(); ++i )
        {
            XMLNode sectorNode = sectorsNode.getChildNode( i );

            m_sectorDeq.emplace_back();
            m_sectorDeq.back().loadFromNode( sectorNode );
            m_sectorDeq.back().loadTransFromNode( sectorNode );
        }
    }
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CBasicStageStrategy::init()
{
    for( auto & iter : m_sectorDeq )
        iter.init();
}


/************************************************************************
*    DESC:  Do some cleanup
************************************************************************/
void CBasicStageStrategy::cleanUp()
{
    for( auto & iter : m_sectorDeq )
        iter.cleanUp();
}


/***************************************************************************
*    DESC:  Update the sector
****************************************************************************/
void CBasicStageStrategy::update()
{
    for( auto & iter : m_sectorDeq )
        iter.update();
}


/************************************************************************
*    DESC:  Transform the sector
************************************************************************/
void CBasicStageStrategy::transform()
{
    for( auto & iter : m_sectorDeq )
        iter.transform();
}

void CBasicStageStrategy::transform( const CObject2D & object )
{
    for( auto & iter : m_sectorDeq )
        iter.transform( object );
}


/***************************************************************************
*    DESC:  Render the sector
****************************************************************************/
void CBasicStageStrategy::render( const CMatrix & matrix )
{
    for( auto & iter : m_sectorDeq )
        iter.render( matrix );
}

void CBasicStageStrategy::render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    for( auto & iter : m_sectorDeq )
        iter.render( matrix, rotMatrix );
}

void CBasicStageStrategy::render()
{
    const auto & camera = CCameraMgr::Instance().getCamera( m_cameraId );

    for( auto & iter : m_sectorDeq )
        iter.render( camera );
}


/************************************************************************
*    DESC:  Get the default camera position
************************************************************************/
CObject & CBasicStageStrategy::getDefaultCameraPos()
{
    return m_defaultCameraPos;
}


/************************************************************************
 *    DESC:  Find if the sprite exists
 ************************************************************************/
bool CBasicStageStrategy::find( iSprite * piSprite )
{
    for( auto & iter : m_sectorDeq )
    {
        if( iter.find( piSprite ) )
            return true;
    }

    return false;
}
