
/************************************************************************
*    FILE NAME:       strategyloader.h
*
*    DESCRIPTION:     Class for loading strategies
************************************************************************/

#pragma once

// Standard lib dependencies
#include <string>

namespace NStrategyloader
{
    // Load the XML that describes how to load the strategy
    void load( const std::string & filePath );
}
