
//
//  FILE NAME:       settings.h
//
//  DESCRIPTION:     game settings class
//

#pragma once

// Game lib dependencies
#include <common/size.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>

class CSettings
{
public:

    // Get the instance of the singleton class
    static CSettings & Instance()
    {
        static CSettings settings;
        return settings;
    }

    // Save the settings file
    void saveSettings();

    // Init the file path for loading the settings file
    void initFilePath( const std::string & filePath );

    // Load settings data from xml file
    void loadXML();
    
    // Get debug info
    bool isDebugMode() const;

    // Get game info
    const std::string & getGameName() const;
    const std::string & getEngineName() const;
    uint32_t getGameVersion() const;
    uint32_t getEngineVersion() const;

    // Get game window size
    const CSize<float> & getDisplaySize() const;
    const CSize<float> & getDisplaySizeHalf() const;
    const CSize<float> & getDefaultSize() const;
    const CSize<float> & getDefaultSizeHalf() const;
    void setDisplaySize( const CSize<float> & size );
    bool allowWindowResize() const;

    // Do we want vSync?
    bool getVSync() const;
    void setVSync( bool value );

    // Get the Vulkan major version
    int getMajorVersion() const;

    // Get the Vulkan minor version
    int getMinorVersion() const;

    // Do we want validation layers
    bool isValidationLayers() const;

    // Height and width screen ratio for orthographic objects.
    // The difference between screen and the default size
    const CSize<float> & getOrthoAspectRatio() const;
    float getOrthoAspectRatioOrientation() const;

    // Height and width screen ratio for perspective projection
    const CSize<float> & getScreenAspectRatio() const;

    // Is the gamepad enabled
    bool isGamePadEnabled() const;

    // Get the gamepad stick dead zone
    int getGamePadStickDeadZone() const;

    // Set the gamepad stick dead zone
    void setGamePadStickDeadZone( int value );

    // Get/Set full screen
    bool getFullScreen() const;
    void setFullScreen( bool value );

    // Calculate the ratio
    void calcRatio();

    // Is the depth buffer activated by default
    bool activateDepthBuffer() const;

    // Is the stencil buffer activated by default
    bool activateStencilBuffer() const;
    
    // Get the script info
    const std::string & getScriptListTable() const;
    const std::string & getScriptGroup() const;
    const std::string & getScriptMain() const;
    bool getSaveByteCode() const;
    bool getLoadByteCode() const;
    bool getStripDebugInfo() const;
    
    // Get the sound frequency
    int getFrequency() const;

    // Get the sound channels: mono, stero, quad, suround, etc
    int getSoundChannels() const;

    // Get the number of channels used for mixing
    int getMixChannels() const;

    // Get the chunk size.
    int getChunkSize() const;

    // Get the Anisotropic setting
    int getAnisotropicLevel() const;
    void setAnisotropicLevel( int level );
    
    // Do we want tripple buffering?
    bool getTripleBuffering() const;

private:

    // Constructor
    CSettings();

    // Destructor
    virtual ~CSettings();

private:

    // file path string
    std::string m_filePath;
    
    // debug members
    bool m_debugMode;
    
    // The game/engine info
    std::string m_gameName;
    std::string m_engineName;
    uint32_t m_gameVersion;
    uint32_t m_engineVersion;

    // with and height of game window
    CSize<float> m_display_size;
    CSize<float> m_display_size_half;
    CSize<float> m_default_size;
    CSize<float> m_default_size_half;

    // Vulkan data
    int m_major;
    int m_minor;
    bool m_validationLayers;

    // Orientation of game window
    EOrientation m_orientation;

    // Height and width screen ratio for perspective projection
    CSize<float> m_screenAspectRatio;

    // Pre-calculated aspect ratios for orthographic projection
    CSize<float> m_orthoAspectRatio;

    // Full screen flag loaded from file
    bool m_fullScreen;

    // Allow the window to be resized
    bool m_allowWindowResize;

    // VSync flag
    bool m_vSync;

    // Do we activate the depth buffer
    bool m_activateDepthBuffer;

    // Do we activate the stencil buffer
    bool m_activateStencilBuffer;

    // Triple buffering flag
    bool m_tripleBuffering;

    // Game pad enabled
    bool m_gamepadEnabled;

    // Gamepad dead zone
    int m_gamepadStickDeadZone;

    // Sound members
    int m_frequency;
    int m_sound_channels;
    int m_mix_channels;
    int m_chunksize;

    // Anisotropic filtering level
    ETextFilter m_anisotropicLevel;
    
    // Scripting string members
    std::string m_scriptListTable;
    std::string m_scriptGroup;
    std::string m_scriptMain;
    bool m_saveByteCode;
    bool m_loadByteCode;
    bool m_stripDebugInfo;
};
