
/************************************************************************
*    FILE NAME:       tagparser.cpp
*
*    DESCRIPTION:     Class to handle parsing xml and json file
************************************************************************/

// Physical component dependency
#include <utilities/tagparser.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

// SDL lib dependencies
#include <SDL2/SDL.h>

// Standard lib dependencies
#include <iostream>
#include <istream>

struct membuf : std::streambuf
{
    membuf(char* pData, size_t sizeInBytes)
    {
        this->setg(pData, pData, pData + sizeInBytes);
    }
};

/************************************************************************
*    desc:  Constructor
************************************************************************/
CTagParser::CTagParser()
{
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CTagParser::~CTagParser()
{
}

/************************************************************************
*    DESC:  Load the xml file
************************************************************************/
void CTagParser::loadXML( const std::string & filePath )
{
    // Load xml into memory
    size_t sizeInBytes = 0;
    std::shared_ptr<char> spChar = NGenFunc::FileToBuf( filePath, sizeInBytes );

    try
    {
        // Move memory to stream buffer so as to allow the read function to parse it
        membuf sbuf(spChar.get(), sizeInBytes);
        std::istream in(&sbuf);

        // Parse the xml
        boost::property_tree::read_xml(in, root);
    }
    catch (const boost::property_tree::xml_parser_error & er)
    {
        // Re-throw error in our format
        throw NExcept::CCriticalException("XML Parser Error!",
                boost::str(boost::format("File name: %s.\nError: %s")
                    % filePath % er.what()));
    }
}


/************************************************************************
*    DESC:  Load the json file
************************************************************************/
void CTagParser::loadJSON( const std::string & filePath )
{
    // Load xml into memory
    size_t sizeInBytes = 0;
    std::shared_ptr<char> spChar = NGenFunc::FileToBuf( filePath, sizeInBytes );

    try
    {
        // Move memory to stream buffer so as to allow the read function to parse it
        membuf sbuf(spChar.get(), sizeInBytes);
        std::istream in(&sbuf);

        // Parse the xml
        boost::property_tree::read_json(in, root);
    }
    catch (const boost::property_tree::json_parser_error & er)
    {
        // Re-throw error in our format
        throw NExcept::CCriticalException("JSON Parser Error!",
                boost::str(boost::format("File name: %s.\nError: %s")
                    % filePath % er.what()));
    }
}


/************************************************************************
*    DESC:  Load the ini file
************************************************************************/
void CTagParser::loadIni( const std::string & filePath )
{
    // Load xml into memory
    size_t sizeInBytes = 0;
    std::shared_ptr<char> spChar = NGenFunc::FileToBuf( filePath, sizeInBytes );

    try
    {
        // Move memory to stream buffer so as to allow the read function to parse it
        membuf sbuf(spChar.get(), sizeInBytes);
        std::istream in(&sbuf);

        // Parse the xml
        boost::property_tree::read_ini(in, root);
    }
    catch (const boost::property_tree::ini_parser_error & er)
    {
        // Re-throw error in our format
        throw NExcept::CCriticalException("JSON Parser Error!",
                boost::str(boost::format("File name: %s.\nError: %s")
                    % filePath % er.what()));
    }
}


/************************************************************************
*    DESC:  Load the info file
************************************************************************/
void CTagParser::loadInfo( const std::string & filePath )
{
    // Load xml into memory
    size_t sizeInBytes = 0;
    std::shared_ptr<char> spChar = NGenFunc::FileToBuf( filePath, sizeInBytes );

    try
    {
        // Move memory to stream buffer so as to allow the read function to parse it
        membuf sbuf(spChar.get(), sizeInBytes);
        std::istream in(&sbuf);

        // Parse the xml
        boost::property_tree::read_info(in, root);
    }
    catch (const boost::property_tree::info_parser_error & er)
    {
        // Re-throw error in our format
        throw NExcept::CCriticalException("JSON Parser Error!",
                boost::str(boost::format("File name: %s.\nError: %s")
                    % filePath % er.what()));
    }
}