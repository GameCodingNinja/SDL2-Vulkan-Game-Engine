
/************************************************************************
*    FILE NAME:       strategyloader.h
*
*    DESCRIPTION:     Class for loading strategies
************************************************************************/

#ifndef __strategy_loader_h__
#define __strategy_loader_h__

// Standard lib dependencies
#include <string>

namespace NStrategyloader
{
    // Load the XML that describes how to load the strategy
    void load( const std::string & filePath );
}

#endif
