/************************************************************************
*    FILE NAME:       sensor.h
*
*    DESCRIPTION:     sensor class
************************************************************************/  

#pragma once

#if defined(_WINDOWS)
#pragma warning(disable : 4244)
#endif

class CSensor
{
public:

    float v1, v2, v3, v4, v5, v6;

    /************************************************************************                                                             
    *    desc:  Constructor
    ************************************************************************/
    CSensor():v1(0),v2(0),v3(0),v4(0),v5(0),v6(0)
    {};

    // Copy constructor
    CSensor( const CSensor & s ) :
        v1(s.v1),v2(s.v2),v3(s.v3),v4(s.v4),v5(s.v5),v6(s.v6)
    {}
};
