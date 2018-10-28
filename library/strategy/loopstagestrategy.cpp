
/************************************************************************
*    FILE NAME:       loopstagestrategy.h
*
*    DESCRIPTION:     Loop stage strategy
*                     Strategy is optimized to move in a looping fassion
************************************************************************/

// Physical component dependency
#include <strategy/loopstagestrategy.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <utilities/xmlparsehelper.h>
#include <strategy/sector.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CLoopStageStrategy::CLoopStageStrategy()
{
}

CLoopStageStrategy::CLoopStageStrategy( const std::string & file ) :
    CLinearStageStrategy( file )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CLoopStageStrategy::~CLoopStageStrategy()
{
}


/************************************************************************
*    DESC:  Load thes object data from node
************************************************************************/
void CLoopStageStrategy::loadFromNode( const XMLNode & node )
{
    CLinearStageStrategy::loadFromNode( node );

    const XMLNode loopStageNode = node.getChildNode( "loopingStage" );

    // Load the start position
    const XMLNode startNode = loopStageNode.getChildNode( "start" );
    if( !startNode.isEmpty() )
        m_loopStart = -NParseHelper::LoadPosition( startNode );

    // Load the end position
    const XMLNode endNode = loopStageNode.getChildNode( "end" );
    if( !endNode.isEmpty() )
        m_loopEnd = -NParseHelper::LoadPosition( endNode );

    // Load the inc position
    const XMLNode incNode = loopStageNode.getChildNode( "inc" );
    if( !incNode.isEmpty() )
        m_loopInc = -NParseHelper::LoadPosition( incNode );

    // Init the offset
    m_loopOffset.setPos( m_loopStart );
}


/************************************************************************
*    DESC:  Transform the actor
************************************************************************/
void CLoopStageStrategy::transform()
{
    if( m_dirType > ESD_NULL )
    {
        m_loopOffset.incPos( m_loopInc * (float)CHighResTimer::Instance().getElapsedTime() );

        if( m_dirType == ESD_FORWARD )
        {
            if( m_loopOffset.getPos() < m_loopEnd )
            {
                m_loopOffset.setPos( m_loopStart + (m_loopOffset.getPos() - m_loopEnd) );
                initRange();
            }
        }
        else if( m_dirType == ESD_BACKWARD )
        {
            if( m_loopOffset.getPos() > m_loopEnd )
            {
                m_loopOffset.setPos( m_loopStart + (m_loopOffset.getPos() - m_loopEnd) );
                initRange();
            }
        }

        m_loopOffset.transform();

        CLinearStageStrategy::transform( m_loopOffset );
    }
    else
        CLinearStageStrategy::transform();
}
