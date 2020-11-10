
/************************************************************************
*    FILE NAME:       keycodeaction.h
*
*    DESCRIPTION:     Class for holding key codes to test for action events
************************************************************************/

#pragma once

// Standard lib dependencies
#include <vector>
#include <algorithm>

class CKeyCodeAction
{
public:
    
    CKeyCodeAction(int id)
    {
        setId( id );
    }

    // Set additional id's
    void setId( int id )
    {
        // Only set id's that are positive numbers
        if( id > -1 )
            m_id.push_back(id);
    }
    
    // Remove an id
    void removeId( int id )
    {
        auto iter = std::find(m_id.begin(), m_id.end(), id);
        
        if( iter != m_id.end() )
            m_id.erase( iter );
    }

    // Check for action
    bool wasAction( const int id ) const
    {
        for( int iter : m_id )
            if( iter == id )
                return true;

        return false;
    }

private:

    std::vector<int> m_id;
};

