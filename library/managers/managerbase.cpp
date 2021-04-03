
/************************************************************************
*    FILE NAME:       managerbase.cpp
*
*    DESCRIPTION:     Base class for common manager behaviors
************************************************************************/

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CManagerBase::CManagerBase()
{
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CManagerBase::~CManagerBase()
{
}

/************************************************************************
*    DESC:  Load the data list tables
************************************************************************/
void CManagerBase::loadListTable( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper(filePath.c_str(), "listTable");

    if( node.isAttributeSet("mobileExt") )
        m_mobileExt = node.getAttribute("mobileExt");

    for( int i = 0; i < node.nChildNode(); ++i )
    {
        const XMLNode dataListNode = node.getChildNode("groupList", i);

        const std::string group = dataListNode.getAttribute("groupName");

        auto iter = m_listTableMap.emplace(group, std::vector<std::string>());

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Object Data List Load Group Error!",
                boost::str(boost::format("Duplicate object name (%s).\n\n%s\nLine: %s")
                    % group % __FUNCTION__ % __LINE__));
        }

        for( int j = 0; j < dataListNode.nChildNode("file"); ++j )
        {
            const XMLNode dataNode = dataListNode.getChildNode("file", j);
            iter.first->second.push_back(dataNode.getAttribute("path"));
        }
        
        // Derived class loading of class specific data
        loadUniqueData( dataListNode, group );
    }
}

/************************************************************************
*    DESC:  Derived class loading of class specific data
************************************************************************/
void CManagerBase::loadUniqueData( const XMLNode & node, const std::string & group )
{
    // Empty for purposes of overriding
}
