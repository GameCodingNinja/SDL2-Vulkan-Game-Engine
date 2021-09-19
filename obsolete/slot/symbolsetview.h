
/************************************************************************
*    FILE NAME:       symbolsetview.h
*
*    DESCRIPTION:     Symbol view set
************************************************************************/

#ifndef __symbol_set_view_h__
#define __symbol_set_view_h__

// Game lib dependencies
#include <sprite/spritedata.h>
#include <slot/symbol2d.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
struct XMLNode;

class CSymbolSetView
{
public:

    // Constructor
    CSymbolSetView( const std::string & group );

    // Destructor
    virtual ~CSymbolSetView();
    
    // Load thes symbol set from node
    void loadFromNode( const XMLNode & node, const std::string & name );
    
    // Get the sprite data
    const std::vector<CSpriteData> & getSpriteData( const std::string & symb ) const;
    
    // Build the visible symbol set
    void buildSymbolSetView();
    
    // Get the symbol set created by the data
    CSymbol2d & getSymbol( const std::string & symb );
    
private:
    
    // Map of the data for creating symbols
    std::map<const std::string, std::vector<CSpriteData>> m_symbolSetDataMap;
    
    // Map of symbols created by the data
    std::map<const std::string, CSymbol2d> m_symbolSetMap;
    
    // Name of the default group
    const std::string m_group;
};

#endif
