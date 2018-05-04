
/************************************************************************
*    FILE NAME:       texture.h
*
*    DESCRIPTION:     Class for holding texture data
************************************************************************/

#ifndef __texture_h__
#define __texture_h__

// Game lib dependencies
#include <common/size.h>

// Texture type
enum ETextureType
{
    ETT_NULL = -1,
    ETT_DIFFUSE,
    ETT_NORMAL,
    ETT_SPECULAR,
    ETT_DISPLACEMENT,
    ETT_MAX_TEXT_TYPES,
};

class CTexture
{
public:

    // OpenGL texture ID
    uint32_t m_id = 0;
    
    // Texture type
    ETextureType m_type = ETT_DIFFUSE;
    
    // Texture size - mostly needed for 2D
    CSize<int> m_size;
    
    // Texture channels
    int m_channels = 0;
    
    // Texture byte data
    unsigned char * m_pData = nullptr;
    
    // Texture file path
    std::string m_textFilePath;

    uint32_t getID() const {return m_id;}
    const CSize<int> & getSize() const {return m_size;}
};

#endif  // __texture_h__


