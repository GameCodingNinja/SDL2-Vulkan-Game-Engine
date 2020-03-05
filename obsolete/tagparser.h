
/************************************************************************
*    FILE NAME:       tagparser.h
*
*    DESCRIPTION:     Class to handle parsing xml and json file
************************************************************************/

#ifndef __tag_parser_h__
#define __tag_parser_h__

// Physical component dependency

// Game lib dependencies

// Boost lib dependencies
#include <boost/property_tree/ptree.hpp>

// Standard lib dependencies

// Forward declaration(s)


class CTagParser
{
public:

    // Constructor
    CTagParser();

    // Destructor
    virtual ~CTagParser();

    // Load the file
    void loadXML( const std::string & filePath );
    void loadJSON( const std::string & filePath );
    void loadIni( const std::string & filePath );
    void loadInfo( const std::string & filePath );

private:

    // property tree root
    boost::property_tree::ptree root;
};

#endif
