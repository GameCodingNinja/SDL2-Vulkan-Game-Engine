
/************************************************************************
*    FILE NAME:       symbolsetview.cpp
*
*    DESCRIPTION:     Symbol view set
************************************************************************/

// Physical component dependency
#include <slot/symbolsetview.h>

// Game lib dependencies
#include <slot/symbol2d.h>
#include <utilities/deletefuncs.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSymbolSetView::CSymbolSetView( const std::string & group ) :
    m_group( group )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSymbolSetView::~CSymbolSetView()
{
}


/************************************************************************
*    DESC:  Load thes reel group data from node
************************************************************************/
void CSymbolSetView::loadFromNode( const XMLNode & node, const std::string & name )
{
    for( int i = 0; i < node.nChildNode(); ++i )
    {
        // Get the child node
        const XMLNode symbolNode = node.getChildNode(i);

        // Get the symbols string id
        const std::string id = symbolNode.getAttribute( "id" );

        // Add the vector to the map
        auto iter = m_symbolSetDataMap.emplace( id, std::vector<CSpriteData>() );

        // Check for duplicate id's
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Symbol Config Load Group Error!",
                boost::str( boost::format("Duplicate symbol id (%s - %s - %s).\n\n%s\nLine: %s")
                    % id % m_group % name % __FUNCTION__ % __LINE__ ));
        }

        for( int j = 0; j < symbolNode.nChildNode(); ++j )
            iter.first->second.emplace_back( symbolNode.getChildNode(j), m_group );
    }
}


/************************************************************************
*    DESC:  Get the vector of sprite data
************************************************************************/
const std::vector<CSpriteData> & CSymbolSetView::getSpriteData( const std::string & symb ) const
{
    // Use the math symbol ID to find the visual symbol
    auto iter = m_symbolSetDataMap.find( symb );
    if( iter == m_symbolSetDataMap.end() )
    {
        throw NExcept::CCriticalException("Reel Strip Init Error!",
            boost::str( boost::format("View symbol data not found in symbol set (%s).\n\n%s\nLine: %s")
                % symb % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Build the visible symbol set
************************************************************************/
void CSymbolSetView::buildSymbolSetView()
{
    // Build the symbol set when asked for it if empty
    if( m_symbolSetMap.empty() )
    {
        // Make a visual symbol set for this group
        for( auto & iter : m_symbolSetDataMap )
        {
            auto symSetIter = m_symbolSetMap.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(iter.first),
                std::forward_as_tuple(iter.second, iter.first) );

            // Check for duplicate names
            if( !symSetIter.second )
            {
                throw NExcept::CCriticalException("Symbol Set Load Error!",
                    boost::str( boost::format("Duplicate symbol name (%s).\n\n%s\nLine: %s")
                        % iter.first % __FUNCTION__ % __LINE__ ));
            }
        }
    }
}


/************************************************************************
*    DESC:  Get the symbol set created by the data
************************************************************************/
CSymbol2d & CSymbolSetView::getSymbol( const std::string & symb )
{
    // Use the math symbol ID to find the visual symbol
    auto iter = m_symbolSetMap.find( symb );
    if( iter == m_symbolSetMap.end() )
    {
        throw NExcept::CCriticalException("Symbol find Error!",
            boost::str( boost::format("View symbol not found in symbol set (%s).\n\n%s\nLine: %s")
                % symb % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}
