
/************************************************************************
*    FILE NAME:       menustrategy.h
*
*    DESCRIPTION:     Menu strategy
************************************************************************/

#ifndef __menu_strategy_h__
#define __menu_strategy_h__

// Physical component dependency
#include <strategy/istrategy.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

class CMenuStrategy : public iStrategy, boost::noncopyable
{
public:

    // Constructor
    CMenuStrategy();

    // Destructor
    virtual ~CMenuStrategy(){};

    // Transform the actor
    virtual void transform() override;

    // Render the actors
    void render() override;
};

#endif
