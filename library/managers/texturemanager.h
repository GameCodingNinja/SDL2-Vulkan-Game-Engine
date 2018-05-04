
/************************************************************************
*    FILE NAME:       texturemanager.h
*
*    DESCRIPTION:     texture class singleton
************************************************************************/ 

#ifndef __texture_manager_h__
#define __texture_manager_h__

// Game lib dependencies
#include <common/texture.h>

// Standard lib dependencies
#include <string>
#include <map>


class CTextureMgr
{
public:

    // Get the instance of the singleton class
    static CTextureMgr & Instance()
    {
        static CTextureMgr textMgr;
        return textMgr;
    }
    
    // Load the image from file path
    void loadImageFor2D( const std::string & group, const std::string & filePath );
    void loadImageFor3D( const std::string & group, const std::string & filePath );
    
    // Create the texture from file path
    const CTexture & createTextureFor2D( const std::string & group, const std::string & filePath, bool compressed = false );
    const CTexture & createTextureFor3D( const std::string & group, const std::string & filePath, bool compressed = false );

    // Texture deleting
    void deleteTextureGroupFor2D( const std::string & group );
    void deleteTextureGroupFor3D( const std::string & group );

    // Function call used to manage what texture is currently bound
    void bind( uint32_t textureID );

    // Unbind the texture and reset the flag
    void unbind();
    
    // Init the Anisotropic filtering
    void initAnisotropic();

private:

    // Constructor
    CTextureMgr();

    // Destructor
    ~CTextureMgr();
    
    // Load the texture from file path
    void loadTexture( CTexture & texture, const std::string & filePath, bool compressed );
    
    // Load the just the image from file path
    void loadImage( CTexture & texture, const std::string & filePath );
    
    // Create the texture from image data loaded into memory
    void createTexture( CTexture & texture, bool compressed );

private:

    // Map containing a group of texture handles
    std::map< const std::string, std::map< const std::string, CTexture > > m_textureFor2DMapMap;
    std::map< const std::string, std::map< const std::string, CTexture > > m_textureFor3DMapMap;

    // Current texture ID
    uint32_t m_currentTextureID;
    
    // Largest possible anisotropic value
    int32_t m_anisotropicLevel;
};

#endif  // __texture_manager_h__

