/************************************************************************
*    FILE NAME:       genfunc.h
*
*    DESCRIPTION:     General mutipurpose functions
************************************************************************/           

#ifndef __genfunc_h__
#define __genfunc_h__

// Standard lib dependencies
#include <string>
#include <vector>
#include <memory>
#include <assert.h>

namespace NGenFunc
{
    // Count the number of occurrences of sub string
    int CountStrOccurrence( const std::string & searchStr, const std::string & subStr );

    // Read in a file and return it as a buffer
    std::shared_ptr<char> FileToBuf( const std::string & file );
    std::shared_ptr<char> FileToBuf( const std::string & file, size_t & sizeInBytes );
    std::vector<char> FileToVec( const std::string & file );

    // Output string info
    void PostDebugMsg( const std::string & msg );

    // Dispatch and event
    int DispatchEvent( int type, int code = 0, void * pData1 = nullptr, void * pData2 = nullptr );

    // Convert 2d screen coordinates to 3D perspective space
    void Convert2Dto3D( float & destX, float & destY, float x, float y, float width, float height );

    // Uniform float random number generation
    int UniformRandomInt( int startRange, int endRange, int seed = 0 );

    // Uniform float random number generation
    float UniformRandomFloat( float startRange, float endRange, int seed = 0 );
    
    // Add a name extension to a file
    void AddFileExt( const std::string & source, std::string & dest, const std::string & ext );
    
    // dynamic_cast type with error checking
    template <typename target, class source>
    inline target * DynCast( source * ptr )
    {
        target * result = dynamic_cast<target *>(ptr);
        assert(result != nullptr);
        return result;
    }

    // dynamic_cast type with error checking
    template <typename target, class source>
    inline target * ReintCast( source * ptr )
    {
        target * result = reinterpret_cast<target *>(ptr);
        assert(result != nullptr);
        return result;
    }
    
}   // NGenFunc

#endif  // __genfunc_h__
