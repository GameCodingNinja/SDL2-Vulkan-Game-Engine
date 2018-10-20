/************************************************************************
*    FILE NAME:       nodefunc.h
*
*    DESCRIPTION:     Node functions
************************************************************************/           

#ifndef __node_func_h__
#define __node_func_h__

// Forward declaration(s)
template <typename type>
class iNode;

namespace NNodeFunc
{
    /************************************************************************
    *    DESC:  Find the parent
    *           NOTE: This is a recursive function
    ************************************************************************/
    template <typename type>
    iNode<type> * FindParent( iNode<type> * pNode, iNode<type> * pSearchNode )
    {
        iNode * pResult = nullptr;

        if( pSearchNode != nullptr )
        {
            if( pSearchNode->getId() == pNode->getParentId() )
            {
                pResult = pSearchNode;
            }
            else
            {
                iNode * pNextNode = nullptr;

                do
                {
                    // get the next node
                    pNextNode = pSearchNode->next();

                    if( pNextNode != nullptr )
                    {
                        // Call a recursive function to find the parent node
                        pResult = FindParent( node, pNextNode );
                    }
                }
                while( pNextNode != nullptr );
            }
        }

        return pResult;
    }
    
}

#endif
