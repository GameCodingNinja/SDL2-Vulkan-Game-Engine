/************************************************************************
*    FILE NAME:       genfunc.cpp
*
*    DESCRIPTION:     General mutipurpose functions
************************************************************************/           

#if defined(_WINDOWS)
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// Physical component dependency
#include <utilities/genfunc.h>

// Game lib dependencies
#include <utilities/smartpointers.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>
#include <boost/crc.hpp>

// SDL lib dependencies
#include <SDL3/SDL.h>

// Standard lib dependencies
#include <iostream>
#include <random>
#include <chrono>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

// disable warning about unsafe functions. Can't use allocated arrays with the safe functions
#if defined(_WINDOWS)
#pragma warning(disable : 4996)
#endif

namespace NGenFunc
{
    /************************************************************************
    *    DESC:  Count the number of occurrences of sub string
    ************************************************************************/
    int CountStrOccurrence( const std::string & searchStr, const std::string & subStr )
    {
        int result = 0;
        std::size_t found = std::string::npos;

        do
        {
            found = searchStr.find(subStr, found+1);

            if( found != std::string::npos )
                ++result;
        }
        while( found != std::string::npos );

        return result;
    }
    
    /************************************************************************
    *    DESC:  Read in a file and return it as a vector buffer
    ************************************************************************/
    std::vector<char> FileToVec( const std::string & file, bool terminate )
    {
        std::size_t terminatorSize = 0;
        if( terminate )
            terminatorSize = 1;

        // Open file for reading
        NSmart::scoped_SDL_filehandle_ptr<SDL_IOStream> scpFile( SDL_IOFromFile( file.c_str(), "rb" ) );
        if( scpFile.isNull() )
            throw NExcept::CCriticalException("File Load Error!",
                boost::str( boost::format("Error Loading file (%s).\n\n%s\nLine: %s") % file % __FUNCTION__ % __LINE__ ));

        // Seek to the end of the file to find out how many 
        // bytes into the file we are and add one for temination
        std::size_t sizeInBytes = (std::size_t)SDL_SeekIO( scpFile.get(), 0, SDL_IO_SEEK_END );
        
        if( (int)sizeInBytes == -1 )
            throw NExcept::CCriticalException("File Load Error!",
                boost::str( boost::format("Error Loading file (%s).\n\n%s\nLine: %s") % file % __FUNCTION__ % __LINE__ ));

        // Allocate a vector to the entire length of the file
        std::vector<char> bufferVec(sizeInBytes + terminatorSize);

        // Go back to the beginning of the file and 
        // read the contents of the file in to the buffer
        SDL_SeekIO( scpFile.get(), 0, SDL_IO_SEEK_SET );
        SDL_ReadIO( scpFile.get(), bufferVec.data(), sizeInBytes );

        if( terminate )
            bufferVec[sizeInBytes] = 0;

        return bufferVec;
    }

    /************************************************************************
    *    DESC:  Dispatch and event
    *
    * _event.user.code = (code4 << 24) | (code3 << 16) | (code2 << 8) | code1;
    ************************************************************************/
    int DispatchEvent( int type, int code, void * pData1, void * pData2 )
    {
        SDL_Event _event;
        _event.type = type;
        _event.user.code = code;
        _event.user.data1 = pData1;
        _event.user.data2 = pData2;

        return SDL_PushEvent(&_event);
    }

    /************************************************************************
    *    DESC: Output string info
    ************************************************************************/
    void PostDebugMsg( const std::string & msg )
    {
    #if defined(_WINDOWS)
        std::string tmp = msg + "\n";
        OutputDebugString( tmp.c_str() );
    #elif defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_DEBUG, "LapCatGames", "%s", msg.c_str());
    #else
        std::cout << msg << std::endl;
    #endif
    }

    /************************************************************************
    *    DESC:  Convert 2d screen coordinates to 3D perspective space
    ************************************************************************/
    void Convert2Dto3D( float & destX, float & destY, float x, float y, float width, float height )
    {
        const float aspect = width / height;

        // Convert upper left 0,0 to center screen 0,0 coordinates
        const float fx = (2.f * (x / (width - 1))) - 1.f;
        const float fy = (2.f * (y / (height - 1))) - 1.f;

        // (pi/4 radians = 45 degrees / 2) or ((pi/4) / 2))
        const float tangent = tan( 0.392699082 );
        
        // Project x,y to a z plane of 1
        destX = aspect * tangent* fx;
        destY = -tangent * fy;
    }

    /************************************************************************
    *    DESC:  Uniform int random number generation
    ************************************************************************/
    int UniformRandomInt( int startRange, int endRange, int seed )
    {
        std::mt19937 generator;

        if( seed == 0 )
        {
            std::random_device rseed;
            generator.seed( rseed() );
        }
        else
        {
            generator.seed( seed );
        }

        std::uniform_int_distribution<int> distribution( startRange, endRange );
        return distribution( generator );
    }

    /************************************************************************
    *    DESC:  Uniform float random number generation
    ************************************************************************/
    float UniformRandomFloat( float startRange, float endRange, int seed )
    {
        std::mt19937 generator;

        if( seed == 0 )
        {
            std::random_device rseed;
            generator.seed( rseed() );
        }
        else
        {
            generator.seed( seed );
        }

        std::uniform_real_distribution<float> distribution( startRange, endRange );
        return distribution( generator );
    }
    
    /************************************************************************
    *    DESC:  Add a name extension to a file
    ************************************************************************/
    void AddFileExt( const std::string & source, std::string & dest, const std::string & ext )
    {
        std::size_t index = source.rfind('.', source.length());
        if( index != std::string::npos )
        {
            dest = source.substr(0, index);
            dest.append( ext );
            dest.append( source.substr(index, source.length() - 1) );
        }
    }

    /************************************************************************
    *    DESC:  Calculate the CRC from a string
    ************************************************************************/
    uint16_t CalcCRC16( const std::string & str )
    {
        boost::crc_16_type result;
        result.process_bytes(str.data(), str.length());
        return result.checksum();
    }
}

