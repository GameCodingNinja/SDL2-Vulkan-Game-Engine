
/************************************************************************
*    FILE NAME:       bitmask.h
*
*    DESCRIPTION:     Class to handle a bit mask
************************************************************************/

#pragma once

template <typename type>
class CBitmask
{
public:

    // Constructor
    CBitmask() : bitmask(0)
    {}

    // Copy Constructors
    CBitmask( const type args ) : bitmask(args)
    {}

    template <typename U>
    CBitmask( const CBitmask<U> bitmask ) : bitmask(bitmask.bitmask)
    {}


    /************************************************************************
    *    desc:  Add the parameters to the bitmask
    ************************************************************************/
    void add( const type args )
    {
        bitmask |= args;
    }


    /************************************************************************
    *    desc:  Remove the parameters from the bitmask
    ************************************************************************/
    void remove( const type args )
    {
        bitmask &= args ^ -1;
    }


    /************************************************************************
    *    desc:  Remove all parameters except for the ones passed in
    ************************************************************************/
    void removeAllExcept( const type args )
    {
        bitmask &= args;
    }


    /************************************************************************
    *    desc:  Set the bitmask to zero
    ************************************************************************/
    void clear()
    {
        bitmask = 0;
    }


    /************************************************************************
    *    desc:  Check if all of the parameters are set
    ************************************************************************/
    bool isEmpty() const
    {
        return (bitmask == 0);
    }


    /************************************************************************
    *    desc:  Check if one of the parameters is set
    ************************************************************************/
    bool isSet( const type args ) const
    {
        return (bitmask & args) != 0;
    }


    /************************************************************************
    *    desc:  Check if all of the parameters are set
    ************************************************************************/
    bool areAllSet( const type args ) const
    {
        return (bitmask & args) == args;
    }


    /************************************************************************
    *    desc:  Get a copy of the bitmask including the parameters
    ************************************************************************/
    type getIncluding( const type args ) const
    {
        return bitmask | args;
    }


    /************************************************************************
    *    desc:  Get a copy of the bitmask excluding the parameters
    ************************************************************************/
    type getExcluding( const type args ) const
    {
        return bitmask & (args ^ -1);
    }


    /************************************************************************
    *    desc:  Overloaded assignment operator
    ************************************************************************/
    void operator = ( const type value )
    {
        bitmask = value;
    }

private:

    // bit mask
    type bitmask;

};
