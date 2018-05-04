
/************************************************************************
*    FILE NAME:       slotmath.cpp
*
*    DESCRIPTION:     Class to hold math data
************************************************************************/

// Physical component dependency
#include <slot/slotmath.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>
#include <cstdint>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSlotMath::CSlotMath( const std::string & group ) :
    m_group( group )
{
}   // constructor


/************************************************************************
*    DESC:  Get the payline set ID
************************************************************************/
const std::string & CSlotMath::getPaylineSetID() const
{
    return m_paylineSetId;
}


/************************************************************************
*    DESC:  Get the symbol set
************************************************************************/
const std::map<const std::string, CMathSymbol> &
    CSlotMath::getSymbolSet( const std::string & id ) const
{
    auto iter = m_symbolSetMapMap.find( id );
    if( iter == m_symbolSetMapMap.end() )
    {
        throw NExcept::CCriticalException("Math Data Get Error!",
            boost::str( boost::format("Math symbol set not found (%s - %s).\n\n%s\nLine: %s")
                % id % m_group % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Get the math strip
************************************************************************/
const std::vector<CStripStop> &
    CSlotMath::getStrip( const std::string & id ) const
{
    auto iter = m_stripMapVec.find( id );
    if( iter == m_stripMapVec.end() )
    {
        throw NExcept::CCriticalException("Math Data Get Error!",
            boost::str( boost::format("Math strip not found (%s - %s).\n\n%s\nLine: %s")
                % id % m_group % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Get the strip set
************************************************************************/
const std::vector<CStripSet> &
    CSlotMath::getStripSet( const std::string & id ) const
{
    auto iter = m_stripSetMapVec.find( id );
    if( iter == m_stripSetMapVec.end() )
    {
        throw NExcept::CCriticalException("Math Data Get Error!",
            boost::str( boost::format("Strip set not found (%s - %s).\n\n%s\nLine: %s")
                % id % m_group % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Get the pay combo set
************************************************************************/
const std::vector<CPayCombo> &
    CSlotMath::getPayComboSet( const std::string & id ) const
{
    auto iter = m_payComboMapVec.find( id );
    if( iter == m_payComboMapVec.end() )
    {
        throw NExcept::CCriticalException("Math Data Get Error!",
            boost::str( boost::format("Pay combo set not found (%s - %s).\n\n%s\nLine: %s")
                % id % m_group % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Get the paytable set
************************************************************************/
const std::vector<CPaytableSet> &
    CSlotMath::getPaytableSet( const std::string & id ) const
{
    auto iter = m_paytableSetMapVec.find( id );
    if( iter == m_paytableSetMapVec.end() )
    {
        throw NExcept::CCriticalException("Math Data Get Error!",
            boost::str( boost::format("Paytable set not found (%s - %s).\n\n%s\nLine: %s")
                % id % m_group % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Get the weighted table
************************************************************************/
const CWeightedTable &
    CSlotMath::getWeightedTable( const std::string & id ) const
{
    auto iter = m_weightedTableMap.find( id );
    if( iter == m_weightedTableMap.end() )
    {
        throw NExcept::CCriticalException("Math Data Get Error!",
            boost::str( boost::format("Weighted table not found (%s - %s).\n\n%s\nLine: %s")
                % id % m_group % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Get the value table
************************************************************************/
const CValueTable &
    CSlotMath::getValueTable( const std::string & id ) const
{
    auto iter = m_valueTableMap.find( id );
    if( iter == m_valueTableMap.end() )
    {
        throw NExcept::CCriticalException("Math Data Get Error!",
            boost::str( boost::format("Valuetable not found (%s - %s).\n\n%s\nLine: %s")
                % id % m_group % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Load the slot group data from node
************************************************************************/
void CSlotMath::loadFromNode( const XMLNode & node )
{
    // Get the math id
    m_id = node.getAttribute( "id" );

    // Get the payline id
    m_paylineSetId = node.getAttribute( "paylineSetId" );

    // Get the math percentage
    m_percenatge = static_cast<double>(std::atof(node.getAttribute( "percentage" )));

    // Load thes symbol set data from node
    loadSymbolSetsFromNode( node );

    // Load the math strip data from node
    loadStripFromNode( node );

    // Load the strip set list data from node
    loadStripSetListFromNode( node );

    // Load the pay combo data from node
    loadPayComboFromNode( node );

    // Load the paytable set list data from node
    loadPaytableSetListFromNode( node );

    // Load the weighted table data from node
    loadWeightedTableFromNode( node );

    // Load the value table data from node
    loadValueTableFromNode( node );
}


/************************************************************************
*    DESC:  Load the symbol set data from node
************************************************************************/
void CSlotMath::loadSymbolSetsFromNode( const XMLNode & node )
{
    // Get the node to the symbol set list
    const XMLNode symbSetLstNode = node.getChildNode( "symbolSetList" );

    for( int symbLst = 0; symbLst < symbSetLstNode.nChildNode(); ++symbLst )
    {
        // Get the symbol set node
        const XMLNode symbSetNode = symbSetLstNode.getChildNode( symbLst );

        // Get the symbol set id
        const std::string setId = symbSetNode.getAttribute( "id" );

        // Create a new symbol set map inside of our map
        auto symbSetIter = m_symbolSetMapMap.emplace( setId, std::map<const std::string, CMathSymbol>() );

        // Check for duplicate names
        if( !symbSetIter.second )
        {
            throw NExcept::CCriticalException("Math Data Load Group Error!",
                boost::str( boost::format("Duplicate symbol set name (%s - %s).\n\n%s\nLine: %s")
                    % setId % m_group % __FUNCTION__ % __LINE__ ));
        }

        // Get the wild list node
        const XMLNode wildLstNode = symbSetNode.getChildNode( "wildSymbolList" );

        // map of wild matches
        std::map<const std::string, std::vector<std::string>> wildMatchesMap;

        // Load up the wild matches first to feed the constructor
        if( !wildLstNode.isEmpty() )
        {
            for( int wildLst = 0; wildLst < wildLstNode.nChildNode(); ++wildLst )
            {
                // Get the wild node
                const XMLNode wildNode = wildLstNode.getChildNode( wildLst );

                // Get the wild id
                const std::string wildId = wildNode.getAttribute( "id" );

                // Create the map and check for duplicate names
                auto iter = wildMatchesMap.emplace( wildId, std::vector<std::string>() );
                if( !iter.second )
                {
                    throw NExcept::CCriticalException("Math Data Load Group Error!",
                        boost::str( boost::format("Duplicate wild symbol in math file (%s, %s).\n\n%s\nLine: %s")
                            % wildId % m_group % __FUNCTION__ % __LINE__ ));
                }

                iter.first->second.reserve( wildNode.nChildNode() );

                for( int symb = 0; symb < wildNode.nChildNode(); ++symb )
                {
                    // Get the wild symbol node
                    const XMLNode symbNode = wildNode.getChildNode( symb );

                    // Get the wild symbol id
                    const std::string id = symbNode.getAttribute( "id" );

                    // Add to the symbol's wild list
                    iter.first->second.push_back( id );
                }
            }
        }

        // Empty vector for symbols that don't have wild matches
        std::vector<std::string> dummy, *pWildMatches;

        // Get the symbol list node
        const XMLNode symbLstNode = symbSetNode.getChildNode( "symbolList" );

        for( int symb = 0; symb < symbLstNode.nChildNode(); ++symb )
        {
            // Get the symbol node
            const XMLNode symbNode = symbLstNode.getChildNode( symb );

            // Get the symbol id
            const std::string symbId = symbNode.getAttribute( "id" );

            // See if this symbol has a wild match
            pWildMatches = &dummy;
            auto wildMatchIter = wildMatchesMap.find( symbId );
            if( wildMatchIter != wildMatchesMap.end() )
                pWildMatches = &wildMatchIter->second;

            // Create the math symbol
            auto symbIter = symbSetIter.first->second.emplace(
                std::piecewise_construct, std::forward_as_tuple(symbId), std::forward_as_tuple(symbId, *pWildMatches) );

            // Check for duplicate names
            if( !symbIter.second )
            {
                throw NExcept::CCriticalException("Math Data Load Group Error!",
                    boost::str( boost::format("Duplicate symbol name (%s - %s).\n\n%s\nLine: %s")
                        % symbId % m_group % __FUNCTION__ % __LINE__ ));
            }

            // If a wild was found, erase it from the map
            if( wildMatchIter != wildMatchesMap.end() )
                wildMatchesMap.erase( wildMatchIter );
        }

        // Check for any dangling wild defines
        if( !wildMatchesMap.empty() )
        {
            throw NExcept::CCriticalException("Math Data Load Group Error!",
                boost::str( boost::format("Wild symbol defined but doesn not exist in symbol set (%s - %s).\n\n%s\nLine: %s")
                    % wildMatchesMap.begin()->first % m_group % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
*    DESC:  Load the math strip data from node
*           Stores a reference from m_symbolSetMapMap
************************************************************************/
void CSlotMath::loadStripFromNode( const XMLNode & node )
{
    // Get the node to the strip list
    const XMLNode stripLstNode = node.getChildNode( "stripList" );

    for( int i = 0; i < stripLstNode.nChildNode(); ++i )
    {
        // Get the strip node
        const XMLNode stripNode = stripLstNode.getChildNode(i);

        // Get the strip id
        const std::string stripId = stripNode.getAttribute( "id" );

        // Get the symbol set id
        const std::string symbSetId = stripNode.getAttribute( "symbSetId" );

        // Find the symbol set this strip is using
        auto symbSetIter = m_symbolSetMapMap.find( symbSetId );
        if( symbSetIter == m_symbolSetMapMap.end() )
        {
            throw NExcept::CCriticalException("Math Data Load Group Error!",
                boost::str( boost::format("Can't find math strip symbol set name (%s - %s).\n\n%s\nLine: %s")
                    % symbSetId % m_group % __FUNCTION__ % __LINE__ ));
        }

        // Create a new math strip inside the map
        auto stripIter = m_stripMapVec.emplace( stripId, std::vector<CStripStop>() );

        // Check for duplicate names
        if( !stripIter.second )
        {
            throw NExcept::CCriticalException("Math Data Load Group Error!",
                boost::str( boost::format("Duplicate math strip name (%s - %s).\n\n%s\nLine: %s")
                    % stripId % m_group % __FUNCTION__ % __LINE__ ));
        }

        // Reserve the number of vectors needed
        stripIter.first->second.reserve( stripNode.nChildNode() );

        for( int j = 0; j < stripNode.nChildNode(); ++j )
        {
            // Get the symbol node
            const XMLNode symbolNode = stripNode.getChildNode(j);

            // Get the symbol id
            const std::string symbId = symbolNode.getAttribute( "id" );

            auto iter = symbSetIter->second.find( symbId );
            if( iter == symbSetIter->second.end() )
            {
                throw NExcept::CCriticalException("Math Data Load Group Error!",
                    boost::str( boost::format("Math symbol not found in symbol set (%s - %s).\n\n%s\nLine: %s")
                        % symbId % m_group % __FUNCTION__ % __LINE__ ));
            }

            int weight = 1;
            if( symbolNode.isAttributeSet( "weight" ) )
                weight = std::atoi(symbolNode.getAttribute( "weight" ));

            // Add symbol to the strip stop
            stripIter.first->second.emplace_back( iter->second, weight );
        }
    }
}


/************************************************************************
*    DESC:  Load the strip set list data from node
************************************************************************/
void CSlotMath::loadStripSetListFromNode( const XMLNode & node )
{
    // Get the node to the strip set list
    const XMLNode stripSetLstNode = node.getChildNode( "stripSetList" );

    for( int set = 0; set < stripSetLstNode.nChildNode(); ++set )
    {
        // Get the strip node
        const XMLNode stripSetNode = stripSetLstNode.getChildNode( set );

        // Get the strip id
        const std::string stripSetId = stripSetNode.getAttribute( "id" );

        // Create a new strip inside the map
        auto stripSetIter = m_stripSetMapVec.emplace( stripSetId, std::vector<CStripSet>() );

        // Check for duplicate names
        if( !stripSetIter.second )
        {
            throw NExcept::CCriticalException("Math Data Load Group Error!",
                boost::str( boost::format("Duplicate strip set name (%s - %s).\n\n%s\nLine: %s")
                    % stripSetId % m_group % __FUNCTION__ % __LINE__ ));
        }

        // Reserve the number of vectors needed
        stripSetIter.first->second.reserve( stripSetNode.nChildNode() );

        for( int tbl = 0; tbl < stripSetNode.nChildNode(); ++tbl )
        {
            // Get the strip node
            const XMLNode stripNode = stripSetNode.getChildNode( tbl );

            // Get the strip id
            const std::string stripId = stripNode.getAttribute( "id" );

            // Check that this strip exists in the strip map vec
            auto iter = m_stripMapVec.find( stripId );
            if( iter == m_stripMapVec.end() )
            {
                throw NExcept::CCriticalException("Math Data Load Group Error!",
                    boost::str( boost::format("Strip not found in strip list (%s - %s).\n\n%s\nLine: %s")
                        % stripId % m_group % __FUNCTION__ % __LINE__ ));
            }

            // Get the evaluation symbol indexes. Reels tend to be in succession but wheels can be disbursed.
            std::vector<int8_t> evalSymbIndex;
            for( int i = 0; i < stripNode.nChildNode(); ++i )
                evalSymbIndex.push_back( std::atoi(stripNode.getChildNode( i ).getAttribute( "index" )) );

            // Add the strip id
            stripSetIter.first->second.emplace_back( stripId, evalSymbIndex );
        }
    }
}


/************************************************************************
*    DESC:  Load the pay combo data from node
************************************************************************/
void CSlotMath::loadPayComboFromNode( const XMLNode & node )
{
    // Get the node to the combo set set list
    const XMLNode payComboSetLstNode = node.getChildNode( "comboSetList" );

    if( !payComboSetLstNode.isEmpty() )
    {
        for( int set = 0; set < payComboSetLstNode.nChildNode(); ++set )
        {
            // Get the pay combo set node
            const XMLNode payComboSetNode = payComboSetLstNode.getChildNode( set );

            // Get the pay combo set id
            const std::string payComboSetId = payComboSetNode.getAttribute( "id" );

            // Create a new pay combo in the map
            auto payComboMapIter = m_payComboMapVec.emplace( payComboSetId, std::vector<CPayCombo>() );

            // Check for duplicate names
            if( !payComboMapIter.second )
            {
                throw NExcept::CCriticalException("Math Data Load Group Error!",
                    boost::str( boost::format("Duplicate pay combo set name (%s - %s).\n\n%s\nLine: %s")
                        % payComboSetId % m_group % __FUNCTION__ % __LINE__ ));
            }

            payComboMapIter.first->second.reserve( payComboSetNode.nChildNode() );

            for( int pay = 0; pay < payComboSetNode.nChildNode(); ++pay )
            {
                // Get the pay combo node
                const XMLNode payComboNode = payComboSetNode.getChildNode( pay );

                // Get the symbol
                const std::string symb = payComboNode.getAttribute( "symb" );

                // Get the count
                const int count = std::atoi(payComboNode.getAttribute( "count" ));

                // Get the value
                const int award = std::atoi(payComboNode.getAttribute( "award" ));

                // Get the value
                int bonusCode = 0;
                if( payComboNode.isAttributeSet( "bonusCode" ))
                    bonusCode = std::atoi(payComboNode.getAttribute( "bonusCode" ));

                // Add the combo pay
                payComboMapIter.first->second.emplace_back( symb, count, award, bonusCode );
            }
        }
    }
}


/************************************************************************
*    DESC:  Load the paytable set list data from node
************************************************************************/
void CSlotMath::loadPaytableSetListFromNode( const XMLNode & node )
{
    // Get the node to the paytable set list
    const XMLNode paytableSetLstNode = node.getChildNode( "paytableSetList" );

    for( int set = 0; set < paytableSetLstNode.nChildNode(); ++set )
    {
        // Get the paytable node
        const XMLNode paytableSetNode = paytableSetLstNode.getChildNode( set );

        // Get the paytable id
        const std::string paytableSetId = paytableSetNode.getAttribute( "id" );

        // Create a new paytable inside the map
        auto paytableSetIter = m_paytableSetMapVec.emplace( paytableSetId, std::vector<CPaytableSet>() );

        // Check for duplicate names
        if( !paytableSetIter.second )
        {
            throw NExcept::CCriticalException("Math Data Load Group Error!",
                boost::str( boost::format("Duplicate paytable set name (%s - %s).\n\n%s\nLine: %s")
                    % paytableSetId % m_group % __FUNCTION__ % __LINE__ ));
        }

        // Reserve the number of vectors needed
        paytableSetIter.first->second.reserve( paytableSetNode.nChildNode() );

        for( int tbl = 0; tbl < paytableSetNode.nChildNode(); ++tbl )
        {
            // Get the paytable node
            const XMLNode paytableNode = paytableSetNode.getChildNode( tbl );

            // Get the paytable type
            NSlotDefs::EPayType paytableType = NSlotDefs::EP_PAYLINE;
            if( std::strcmp( paytableNode.getAttribute("type"), "scatter" ) == 0 )
                paytableType = NSlotDefs::EP_SCATTER;

            // Get the paytable id
            const std::string paytableStrId = paytableNode.getAttribute( "id" );

            // Check that this pay combo exists in the pay combo map vec
            auto paylineIter = m_payComboMapVec.find( paytableStrId );
            if( paylineIter == m_payComboMapVec.end() )
            {
                throw NExcept::CCriticalException("Math Data Load Group Error!",
                    boost::str( boost::format("Pay combo not found in paytable list (%s - %s).\n\n%s\nLine: %s")
                        % paytableStrId % m_group % __FUNCTION__ % __LINE__ ));
            }

            // Add the paytable id
            paytableSetIter.first->second.emplace_back( paytableType, paytableStrId );
        }
    }
}


/************************************************************************
*    DESC:  Load the weighted table data from node
************************************************************************/
void CSlotMath::loadWeightedTableFromNode( const XMLNode & node )
{
    // Get the node to the weighted table list
    const XMLNode weightedTableLstNode = node.getChildNode( "weightedTableList" );

    if( !weightedTableLstNode.isEmpty() )
    {
        for( int set = 0; set < weightedTableLstNode.nChildNode(); ++set )
        {
            // Get the weighted table node
            const XMLNode weightedTableNode = weightedTableLstNode.getChildNode( set );

            // Get the weighted table id
            const std::string weightedTableId = weightedTableNode.getAttribute( "id" );

            int totalWeight(0);
            std::vector<int> weightVec;
            std::vector<int> valueVec;

            weightVec.reserve( weightedTableNode.nChildNode() );
            valueVec.reserve( weightedTableNode.nChildNode() );

            for( int tbl = 0; tbl < weightedTableNode.nChildNode(); ++tbl )
            {
                // Get the table node
                const XMLNode tableNode = weightedTableNode.getChildNode( tbl );

                // Get the weight
                const int weight = std::atoi(tableNode.getAttribute( "weight" ));

                // Get the value
                const int value = std::atoi(tableNode.getAttribute( "value" ));

                totalWeight += weight;
                weightVec.push_back( weight );
                valueVec.push_back( value );
            }

            // Create a new weighted table inside the map
            auto weightedTableIter = m_weightedTableMap.emplace(
                std::piecewise_construct, std::forward_as_tuple(weightedTableId), std::forward_as_tuple(totalWeight, weightVec, valueVec) );

            // Check for duplicate names
            if( !weightedTableIter.second )
            {
                throw NExcept::CCriticalException("Math Data Load Group Error!",
                    boost::str( boost::format("Duplicate weighted table name (%s - %s).\n\n%s\nLine: %s")
                        % weightedTableId % m_group % __FUNCTION__ % __LINE__ ));
            }
        }
    }
}


/************************************************************************
*    DESC:  Load the value table data from node
************************************************************************/
void CSlotMath::loadValueTableFromNode( const XMLNode & node )
{
    // Get the node to the value table list
    const XMLNode valueTableLstNode = node.getChildNode( "valueTableList" );

    if( !valueTableLstNode.isEmpty() )
    {
        for( int set = 0; set < valueTableLstNode.nChildNode(); ++set )
        {
            // Get the value table node
            const XMLNode valueTableNode = valueTableLstNode.getChildNode( set );

            // Get the value table id
            const std::string valueTableId = valueTableNode.getAttribute( "id" );

            std::vector<int> valueVec;
            valueVec.reserve( valueTableNode.nChildNode() );

            for( int tbl = 0; tbl < valueTableNode.nChildNode(); ++tbl )
            {
                // Get the table node
                const XMLNode tableNode = valueTableNode.getChildNode( tbl );

                // Get the value
                const int value = std::atoi(tableNode.getAttribute( "value" ));

                // Add table element
                valueVec.push_back( value );
            }

            // Create a new value table inside the map
            auto valueTableIter = m_valueTableMap.emplace( valueTableId, valueVec );

            // Check for duplicate names
            if( !valueTableIter.second )
            {
                throw NExcept::CCriticalException("Math Data Load Group Error!",
                    boost::str( boost::format("Duplicate value table name (%s - %s).\n\n%s\nLine: %s")
                        % valueTableId % m_group % __FUNCTION__ % __LINE__ ));
            }
        }
    }
}
