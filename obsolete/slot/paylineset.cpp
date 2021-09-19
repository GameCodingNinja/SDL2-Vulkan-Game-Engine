
/************************************************************************
*    FILE NAME:       paylineset.cpp
*
*    DESCRIPTION:     Class for holding the payline and scatter eval symbol 
*                     positions that are allowed for evaluation
************************************************************************/

// Physical component dependency
#include <slot/paylineset.h>

// Game lib dependencies
#include <utilities/xmlParser.h>

// Standard lib dependencies
#include <algorithm>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CPaylineSet::CPaylineSet( const XMLNode & node )
{
    // Pre-reserve the amount
    const int paylineCount( node.nChildNode("line") );
    const int scatterCount( node.nChildNode("scatter") );
    
    m_lineVecVec.reserve( paylineCount );
    m_scatterVecVec.reserve( scatterCount );
    
    for( int i = 0; i < paylineCount; ++i )
    {
         const XMLNode lineNode = node.getChildNode( "line", i );

        m_lineVecVec.emplace_back();
        m_lineVecVec.back().reserve( lineNode.nAttribute() );

        for( int j = 0; j < lineNode.nAttribute(); ++j )
            m_lineVecVec.at(i).push_back( std::atoi(lineNode.getAttributeValue(j)) );
    }
    
    for( int i = 0; i < scatterCount; ++i )
    {
         const XMLNode scatterNode = node.getChildNode( "scatter", i );

        m_scatterVecVec.emplace_back();
        m_scatterVecVec.back().reserve( scatterNode.nAttribute() );

        for( int j = 0; j < scatterNode.nAttribute(); ++j )
            m_scatterVecVec.at(i).push_back( std::atoi(scatterNode.getAttributeValue(j)) );
    }
}


/************************************************************************
*    DESC:  Get the line data
************************************************************************/
const std::vector<std::vector<int8_t>> & CPaylineSet::getLineData() const
{
    return m_lineVecVec;
}


/************************************************************************
*    DESC:  See if the scatter eval symbol index is allowed
************************************************************************/
bool CPaylineSet::indexOfScaterData( int strip, int index ) const
{
    return std::find(m_scatterVecVec.at(strip).begin(), m_scatterVecVec.at(strip).end(), index) != m_scatterVecVec.at(strip).end();
}
