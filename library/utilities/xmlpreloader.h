
/************************************************************************
*    FILE NAME:       xmlpreloader.h
*
*    DESCRIPTION:     Preload XML data from file.
*                     Utility for preloading XML data for objects that can
*                     only be allocated in there specific state 
************************************************************************/

#pragma once

// Game lib dependencies
#include <utilities/xmlParser.h>

// Standard lib dependencies
#include <string>
#include <map>

class CXMLPreloader
{
public:

    // Get the instance of the singleton class
    static CXMLPreloader & Instance()
    {
        static CXMLPreloader xmlPreloader;
        return xmlPreloader;
    }
    
    // Load the XML file
    const XMLNode & load( const std::string & filePath, const std::string & firstNode );
    
    // Get the loaded XML node
    const XMLNode & getNode( const std::string & filePath ) const;
    
    // Clear all the XML data
    void clear();
    
private:
    
    // Constructor
    CXMLPreloader();

    // Destructor
    ~CXMLPreloader();
    
private:
    
    // Map of XML nodes
    std::map<const std::string, XMLNode> m_xmlNodeMap;
};
