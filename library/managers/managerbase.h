
/************************************************************************
*    FILE NAME:       managerbase.h
*
*    DESCRIPTION:     Base class for common manager behaviors
************************************************************************/

#ifndef __manager_base_h__
#define __manager_base_h__

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

protected:

    CManagerBase();
    virtual ~CManagerBase();

protected:

    // Holds the list table map
    std::map<const std::string, std::vector<std::string>> m_listTableMap;
    
    // Mobile extension
    std::string m_mobileExt;

};

#endif  // __manager_base_h__


