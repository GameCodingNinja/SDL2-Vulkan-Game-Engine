
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

CBasicStageStrategy::CBasicStageStrategy( const std::string & file )
{
    loadFromFile( file );
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

    // Load these sector data from node
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


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CBasicStageStrategy::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    for( auto & iter : m_sectorDeq )
        iter.recordCommandBuffer( index, cmdBuffer, viewProj );
}

void CBasicStageStrategy::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj )
{
    for( auto & iter : m_sectorDeq )
        iter.recordCommandBuffer( index, cmdBuffer, rotMatrix, viewProj );
}


/************************************************************************
*    DESC:  Get the default camera position
************************************************************************/
CObject & CBasicStageStrategy::getDefaultCameraPos()
{
    return m_defaultCameraPos;
}
