
/************************************************************************
*    FILE NAME:       texturemanager.h
*
*    DESCRIPTION:     texture class singleton
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <managers/texturemanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>

// SOIL lib dependency
#include <soil/SOIL.h>
#include <soil/stb_image_aug.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CTextureMgr::CTextureMgr() :
    m_currentTextureID(0),
    m_anisotropicLevel(0)
{
    initAnisotropic();
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CTextureMgr::~CTextureMgr()
{
    // Free all textures in all groups
    for( auto & mapMapIter : m_textureFor2DMapMap )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            glDeleteTextures(1, &mapIter.second.m_id);
        }
    }

    for( auto & mapMapIter : m_textureFor3DMapMap )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            glDeleteTextures(1, &mapIter.second.m_id);
        }
    }
}


/************************************************************************
*    DESC:  Load the image from file path
************************************************************************/
void CTextureMgr::loadImageFor2D( const std::string & group, const std::string & filePath )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_textureFor2DMapMap.find( group );
    if( mapMapIter == m_textureFor2DMapMap.end() )
        mapMapIter = m_textureFor2DMapMap.emplace( group, std::map<const std::string, CTexture>() ).first;

    // See if this texture has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, load the texture and add it to the list
    if( mapIter == mapMapIter->second.end() )
    {
        CTexture texture;

        // Load the image from file path
        loadImage( texture, filePath );

        // Insert the new texture info
        mapIter = mapMapIter->second.emplace( filePath, texture ).first;
    }
}


/************************************************************************
*    DESC:  Create the texture from file path
************************************************************************/
const CTexture & CTextureMgr::createTextureFor2D( const std::string & group, const std::string & filePath, bool compressed )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_textureFor2DMapMap.find( group );
    if( mapMapIter == m_textureFor2DMapMap.end() )
        mapMapIter = m_textureFor2DMapMap.emplace( group, std::map<const std::string, CTexture>() ).first;

    // See if this texture has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, there's a problem
    if( mapIter == mapMapIter->second.end() )
        throw NExcept::CCriticalException("Create Texture Error!",
            boost::str( boost::format("Error creating texture (%s)(%s).\n\n%s\nLine: %s")
                % filePath % __FUNCTION__ % __LINE__ ));

    if( mapIter->second.getID() == 0 )
    {
        // Load the texture from file path
        createTexture( mapIter->second, compressed );

        // NOTE: Now handled in SOIL (reason for commenting it out))
        // Init with common features until I need to configure differently
        //glBindTexture(GL_TEXTURE_2D, texture.GetID());
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    return mapIter->second;
}


/************************************************************************
*    DESC:  Load the texture from file path
************************************************************************/
void CTextureMgr::loadImageFor3D( const std::string & group, const std::string & filePath )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_textureFor3DMapMap.find( group );
    if( mapMapIter == m_textureFor3DMapMap.end() )
        mapMapIter = m_textureFor3DMapMap.emplace( group, std::map<const std::string, CTexture>() ).first;

    // See if this texture has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, load the texture and add it to the list
    if( mapIter == mapMapIter->second.end() )
    {
        CTexture texture;

        // Load the texture from file path
        loadImage( texture, filePath );

        // Insert the new texture info
        mapIter = mapMapIter->second.emplace( filePath, texture ).first;
    }
}


/************************************************************************
*    DESC:  Load the texture from file path
************************************************************************/
const CTexture & CTextureMgr::createTextureFor3D( const std::string & group, const std::string & filePath, bool compressed )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_textureFor3DMapMap.find( group );
    if( mapMapIter == m_textureFor3DMapMap.end() )
        mapMapIter = m_textureFor3DMapMap.emplace( group, std::map<const std::string, CTexture>() ).first;

    // See if this texture has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, there's a problem
    if( mapIter == mapMapIter->second.end() )
        throw NExcept::CCriticalException("Create Texture Error!",
            boost::str( boost::format("Error creating texture (%s)(%s).\n\n%s\nLine: %s")
                % filePath % group % __FUNCTION__ % __LINE__ ));

    // If it's not found, create the texture and add it to the list
    if( mapIter->second.getID() == 0 )
    {
        // Load the texture from file path
        createTexture( mapIter->second, compressed );

        // Init with common features until I need to configure differently
        glBindTexture(GL_TEXTURE_2D, mapIter->second.getID());

        // Set the anisotropic value
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropicLevel );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    return mapIter->second;
}


/************************************************************************
*    DESC:  Load the texture from file path
************************************************************************/
void CTextureMgr::loadTexture( CTexture & texture, const std::string & filePath, bool compressed )
{
    texture.m_id = SOIL_load_OGL_texture(
        filePath.c_str(),
        &texture.m_size.w,
        &texture.m_size.h,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        (compressed == true) ? SOIL_FLAG_COMPRESS_TO_DXT : SOIL_FLAG_ORIGINAL_TEXTURE_FORMAT );

    if( texture.getID() == 0 )
        throw NExcept::CCriticalException("Load Texture Error!",
            boost::str( boost::format("Error loading texture (%s)(%s).\n\n%s\nLine: %s")
                % stbi_failure_reason() % filePath % __FUNCTION__ % __LINE__ ));
}


/************************************************************************
*    DESC:  Load the just the image from file path
************************************************************************/
void CTextureMgr::loadImage( CTexture & texture, const std::string & filePath )
{
    texture.m_pData = SOIL_load_image(
        filePath.c_str(),
        &texture.m_size.w,
        &texture.m_size.h,
        &texture.m_channels,
        SOIL_LOAD_AUTO
    );

    if( texture.m_pData == nullptr )
        throw NExcept::CCriticalException("Load Image Error!",
            boost::str( boost::format("Error loading image (%s)(%s).\n\n%s\nLine: %s")
                % stbi_failure_reason() % filePath % __FUNCTION__ % __LINE__ ));
}


/************************************************************************
*    DESC:  Create the texture from image data loaded into memory
************************************************************************/
void CTextureMgr::createTexture( CTexture & texture, bool compressed )
{
    if( texture.m_pData == nullptr )
        throw NExcept::CCriticalException("Create Texture Error!",
            boost::str( boost::format("Can't create texture from null pointer.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    texture.m_id = SOIL_create_OGL_texture(
        texture.m_pData,
        texture.m_size.w,
        texture.m_size.h,
        texture.m_channels,
        SOIL_CREATE_NEW_ID,
        (compressed == true) ? SOIL_FLAG_COMPRESS_TO_DXT : SOIL_FLAG_ORIGINAL_TEXTURE_FORMAT
    );

    SOIL_free_image_data( texture.m_pData );

    texture.m_pData = nullptr;

    if( texture.getID() == 0 )
        throw NExcept::CCriticalException("Load Texture Error!",
            boost::str( boost::format("Error loading texture (%s).\n\n%s\nLine: %s")
                % stbi_failure_reason() % __FUNCTION__ % __LINE__ ));
}


/************************************************************************
*    DESC:  Delete a texture in a group
************************************************************************/
void CTextureMgr::deleteTextureGroupFor2D( const std::string & group )
{
    // Free the texture group if it exists
    auto mapMapIter = m_textureFor2DMapMap.find( group );
    if( mapMapIter != m_textureFor2DMapMap.end() )
    {
        // Delete all the textures in this group
        for( auto & mapIter : mapMapIter->second )
            glDeleteTextures(1, &mapIter.second.m_id);

        // Erase this group
        m_textureFor2DMapMap.erase( mapMapIter );
    }
}


/************************************************************************
*    DESC:  Delete a texture in a group
************************************************************************/
void CTextureMgr::deleteTextureGroupFor3D( const std::string & group )
{
    // Free the texture group if it exists
    auto mapMapIter = m_textureFor3DMapMap.find( group );
    if( mapMapIter != m_textureFor3DMapMap.end() )
    {
        // Delete all the textures in this group
        for( auto & mapIter : mapMapIter->second )
            glDeleteTextures(1, &mapIter.second.m_id);

        // Erase this group
        m_textureFor3DMapMap.erase( mapMapIter );
    }
}


/************************************************************************
*    DESC:  Function call used to manage what texture is currently bound.
*           This insures that we don't keep rebinding the same texture
************************************************************************/
void CTextureMgr::bind( uint32_t textureID )
{
    if( m_currentTextureID != textureID )
    {
        // save the current binding
        m_currentTextureID = textureID;

        // Have OpenGL bind this texture now
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}


/************************************************************************
*    DESC:  Unbind the texture and reset the flag
************************************************************************/
void CTextureMgr::unbind()
{
    m_currentTextureID = 0;
    glBindTexture(GL_TEXTURE_2D, 0);
}


/************************************************************************
*    DESC:  Init the Anisotropic filtering
************************************************************************/
void CTextureMgr::initAnisotropic()
{
    int32_t maxAnisotropicLevel(0);
    glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropicLevel );

    if( maxAnisotropicLevel < 0 )
        maxAnisotropicLevel = 0;

    if( CSettings::Instance().getAnisotropicLevel() <= maxAnisotropicLevel )
        m_anisotropicLevel = CSettings::Instance().getAnisotropicLevel();
    else
        m_anisotropicLevel = maxAnisotropicLevel;
}
