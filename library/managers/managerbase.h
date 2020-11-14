
/************************************************************************
*    FILE NAME:       managerbase.h
*
*    DESCRIPTION:     Base class for common manager behaviors
************************************************************************/

#pragma once

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
struct XMLNode;

class CManagerBase
{
public:

    // Load the data list table
    void loadListTable( const std::string & filePath );
    
    // Derived class loading of class specific data
    virtual void loadUniqueData( const XMLNode & node, const std::string & group );

protected:

    CManagerBase();
    virtual ~CManagerBase();

protected:

    // Holds the list table map
    std::map<const std::string, std::vector<std::string>> m_listTableMap;
    
    // Mobile extension
    std::string m_mobileExt;
};
