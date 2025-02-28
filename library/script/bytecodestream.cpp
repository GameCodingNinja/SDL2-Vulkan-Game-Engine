
/************************************************************************
*    FILE NAME:       bytecodestream.cpp
*
*    DESCRIPTION:     Class for loading and saving AngelScript byte code
************************************************************************/

// Physical component dependency
#include <script/bytecodestream.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CByteCodeStream::CByteCodeStream( const std::string & file, const std::string & mode )
{
    // Open file for reading
    m_scpFile.reset( SDL_IOFromFile( file.c_str(), mode.c_str() ) );
    if( m_scpFile.isNull() )
        throw NExcept::CCriticalException("AngelScript File Open Error!",
            boost::str( boost::format("Error Opening file (%s).\n\n%s\nLine: %s") % file % __FUNCTION__ % __LINE__ ));
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CByteCodeStream::~CByteCodeStream()
{
}

/************************************************************************
*    DESC:  Write the byte code to the file
************************************************************************/
int CByteCodeStream::Write( const void *ptr, asUINT sizeInBytes ) 
{
    if( sizeInBytes > 0 )
    {
        if( SDL_WriteIO( m_scpFile.get(), ptr, sizeInBytes ) == 0 )
            return -1;
    }

    return 0;
}

/************************************************************************
*    DESC:  Read the byte code from the file
************************************************************************/
int CByteCodeStream::Read( void *ptr, asUINT sizeInBytes ) 
{ 
    if( sizeInBytes > 0 )
    {
        if( SDL_ReadIO( m_scpFile.get(), ptr, sizeInBytes ) == 0 )
            return -1;
    }

    return 0;
}