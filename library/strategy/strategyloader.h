
//
//    FILE NAME:       strategyloader.h
//
//    DESCRIPTION:     Class singleton for loading strategies
//

#pragma once

// Physical component dependency
#include <managers/managerbase.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward declaration(s)
class CObject;
class CSprite;
struct XMLNode;

class CStrategyloader : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CStrategyloader & Instance()
    {
        static CStrategyloader strategyloader;
        return strategyloader;
    }

    // Load a list of groups
    void loadGroupLst( const std::vector<std::string> & groupVec );

    // Load a group
    void loadGroup( const std::string & group );

    // Load the XML that describes how to load the strategy
    void load( const std::string & filePath );

protected:

    CStrategyloader();
    virtual ~CStrategyloader();

    // Init the object with the xmlNode data
    void init( const XMLNode & nodeXML, CObject * pObject );
    void init( const XMLNode & nodeXML, CSprite * pSprite );
};
