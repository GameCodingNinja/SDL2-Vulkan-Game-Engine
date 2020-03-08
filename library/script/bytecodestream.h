
/************************************************************************
*    FILE NAME:       bytecodestream.h
*
*    DESCRIPTION:     Class for loading and saving AngelScript byte code
************************************************************************/

#ifndef __byte_code_stream_h__
#define __byte_code_stream_h__

// Physical component dependency
#include <angelscript.h>

// Game lib dependencies
#include <utilities/smartpointers.h>

// Standard lib dependencies
#include <string>

// Forward declaration(s)
struct SDL_RWops;

class CByteCodeStream : public asIBinaryStream
{
public:

    // Constructor
    CByteCodeStream( const std::string & file, const std::string & mode );

    // Destructor
    virtual ~CByteCodeStream();
 
    // Write the byte code to the file
    int Write( const void *ptr, asUINT sizeInBytes ) override;

    // Read the byte code from the file
    int Read( void *ptr, asUINT sizeInBytes ) override;

private:

    // Scoped file handle pointer
    NSmart::scoped_SDL_filehandle_ptr<SDL_RWops> m_scpFile;
};

#endif
