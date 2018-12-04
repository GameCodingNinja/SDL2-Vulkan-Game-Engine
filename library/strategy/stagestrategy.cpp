
/************************************************************************
*    FILE NAME:       stagestrategy.cpp
*
*    DESCRIPTION:     Stage strategy
************************************************************************/

// Physical component dependency
#include <strategy/stagestrategy.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <system/device.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CStageStrategy::CStageStrategy()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CStageStrategy::~CStageStrategy()
{
}


/************************************************************************
*    DESC:  Load the sector data from file
************************************************************************/
void CStageStrategy::loadFromFile( const std::string & file )
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
void CStageStrategy::loadFromNode( const XMLNode & node )
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


/***************************************************************************
*    DESC:  Update the sector
****************************************************************************/
void CStageStrategy::update()
{
    for( auto & iter : m_sectorDeq )
        iter.update();
}


/************************************************************************
*    DESC:  Transform the sector
************************************************************************/
void CStageStrategy::transform()
{
    for( auto & iter : m_sectorDeq )
        iter.transform();
}


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CStageStrategy::recordCommandBuffer( uint32_t index, const CCamera & camera )
{
    auto cmdBuf( m_commandBufVec[index] );

    CDevice::Instance().beginCommandBuffer( index, cmdBuf );

    for( auto & iter : m_sectorDeq )
        iter.recordCommandBuffer( index, cmdBuf, camera );

    CDevice::Instance().endCommandBuffer( cmdBuf );
}


/************************************************************************
*    DESC:  Get the default camera position
************************************************************************/
CObject & CStageStrategy::getDefaultCameraPos()
{
    return m_defaultCameraPos;
}