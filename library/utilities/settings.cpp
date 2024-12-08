
//
//  FILE NAME:       settings.cpp
//
//  DESCRIPTION:     game settings class
//

// Physical component dependency
#include <utilities/settings.h>

// Game lib dependencies
#include <utilities/genfunc.h>
#include <utilities/json.hpp>

// Boost lib dependencies
#include <boost/lexical_cast.hpp>

// Standard lib dependencies
#include <cstring>

// SDL lib dependencies
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

//
//  DESC:  Constructor
//
CSettings::CSettings() :
    m_filePath("data/settings/settings.cfg"),
    m_debugMode(false),
    m_display_size(1280,768),
    m_default_size(1280,768),
    m_major(1),
    m_minor(0),
    m_validationLayers(false),
    m_orientation(EOrientation::LANDSCAPE),
    m_fullScreen(false),
    m_vSync(false),
    m_activateDepthBuffer(false),
    m_activateStencilBuffer(false),
    m_tripleBuffering(false),
    m_gamepadEnabled(false),
    m_gamepadStickDeadZone(2500),
    m_frequency(44100),
    m_sound_channels(MIX_DEFAULT_CHANNELS),
    m_mix_channels(MIX_CHANNELS),
    m_chunksize(1024),
    m_anisotropicLevel(ETextFilter::ANISOTROPIC_0X),
    m_saveByteCode(false),
    m_loadByteCode(false),
    m_stripDebugInfo(false)
{
}

//
//  DESC:  destructor
//
CSettings::~CSettings()
{
}

//
//  DESC:  Init the file path for loading the settings file
//
void CSettings::initFilePath( const std::string & filePath )
{
    m_filePath = filePath;
}

//
//  DESC:  Load settings data from xml file
//
//  param: string & _filePath - path to file
//
//  ret: string - class name
//
void CSettings::loadXML()
{
    std::vector<char> bufVec = NGenFunc::FileToVec("data/settings/settings.json", NGenFunc::TERMINATE);
    nlohmann::json json = nlohmann::json::parse(bufVec.data());

    // Get the debug mode
    m_debugMode = json["debug"]["debugMode"];

    // Get the game info
    m_gameName = json["info"]["gameName"];
    m_gameVersion = json["info"]["gameVersion"];
    m_engineName = json["info"]["engineName"];
    m_engineVersion = json["info"]["engineVersion"];

    // Get display info
    m_display_size.w = json["resolution"]["display"]["width"];
    m_display_size.h = json["resolution"]["display"]["height"];
    m_fullScreen = json["resolution"]["display"]["fullScreen"];
    m_allowWindowResize = json["resolution"]["display"]["allowWindowResize"];
    m_display_size_half = m_display_size / 2.f;

    // The default size is the 2d design size
    // This helps to scale 2d GUI elements on the screen
    // so that the mouse pointer lines up with them
    m_default_size.w = json["resolution"]["default"]["width"];
    m_default_size.h = json["resolution"]["default"]["height"];
    m_orientation = json["resolution"]["default"]["orientation"];
    m_default_size_half = m_default_size / 2.f;

    // Get device info
    // Vulkan info
    m_major = json["device"]["Vulkan"]["major"];
    m_minor = json["device"]["Vulkan"]["minor"];
    m_validationLayers = json["device"]["Vulkan"]["validationLayers"];

    // Back buffer
    m_tripleBuffering = json["device"]["backbuffer"]["tripleBuffering"];
    m_vSync = json["device"]["backbuffer"]["VSync"];
    m_anisotropicLevel = json["device"]["backbuffer"]["anisotropicFiltering"];
    m_activateDepthBuffer = json["device"]["backbuffer"]["activateDepthBuffer"];
    m_activateStencilBuffer = json["device"]["backbuffer"]["activateStencilBuffer"];

    // Gamepad
    m_gamepadEnabled = json["device"]["gamepad"]["allow"];
    m_gamepadStickDeadZone = json["device"]["gamepad"]["stickDeadZone"];

    // Scripting
    m_scriptListTable = json["scripting"]["scriptListTable"];
    m_scriptGroup = json["scripting"]["group"];
    m_scriptMain = json["scripting"]["mainFunction"];
    m_saveByteCode = json["scripting"]["saveByteCode"];
    m_loadByteCode = json["scripting"]["loadByteCode"];

    // Sound
    m_frequency = json["sound"]["frequency"];
    m_sound_channels = json["sound"]["sound_channels"];
    m_mix_channels = json["sound"]["mix_channels"];
    m_chunksize = json["sound"]["chunksize"];

    calcRatio();
}

//
//  DESC:  Get debug info
//
bool CSettings::isDebugMode() const
{
    return m_debugMode;
}

//
//  DESC:  Get game info
//
const std::string & CSettings::getGameName() const
{
    return m_gameName;
}

const std::string & CSettings::getEngineName() const
{
    return m_engineName;
}

uint32_t CSettings::getGameVersion() const
{
    return m_gameVersion;
}

uint32_t CSettings::getEngineVersion() const
{
    return m_engineVersion;
}

//
//  DESC:  Calculate the ratios
//
void CSettings::calcRatio()
{
    m_screenAspectRatio.w = m_display_size.w / m_display_size.h;
    m_screenAspectRatio.h = m_display_size.h / m_display_size.w;

    if( m_allowWindowResize )
    {
        if( m_display_size.w / m_display_size.h > m_screenAspectRatio.w )
        {
            // NOTE: The default height is based on the current aspect ratio
            // NOTE: Make sure the height does not have a floating point component
            m_default_size.h = (float)(int)std::ceil((m_screenAspectRatio.h * m_default_size.w) + 0.5);
        }
        else
        {
            // NOTE: The default width is based on the current aspect ratio
            // NOTE: Make sure the width does not have a floating point component
            m_default_size.w = (float)(int)std::ceil((m_screenAspectRatio.w * m_default_size.h) + 0.5);
        }
    }

    // Get half the size for use with screen boundaries
    m_default_size_half = m_default_size / 2.f;

    // Screen size divided by two
    m_display_size_half = m_display_size / 2.f;

    // Pre-calculate the aspect ratios for orthographic projection
    m_orthoAspectRatio.h = m_display_size.h / m_default_size.h;
    m_orthoAspectRatio.w = m_display_size.w / m_default_size.w;
}

//
//  DESC:  Get/Set game window size
//
const CSize<float> & CSettings::getDisplaySize() const
{
    return m_display_size;
}

void CSettings::setDisplaySize( const CSize<float> & size )
{
    m_display_size = size;
}

//
//  DESC:  Get game window size / 2
//
//  ret: CSize - size of game window / 2
//
const CSize<float> & CSettings::getDisplaySizeHalf() const
{
    return m_display_size_half;
}

//
//  DESC:  Get default size specified in config file
//
const CSize<float> & CSettings::getDefaultSize() const
{
    return m_default_size;
}

//
//  DESC:  Get default size in half
//
const CSize<float> & CSettings::getDefaultSizeHalf() const
{
    return m_default_size_half;
}

//
//  DESC:  Height and width screen ratio for orthographic objects
//         The difference between screen and the default size
//
const CSize<float> & CSettings::getOrthoAspectRatio() const
{
    return m_orthoAspectRatio;
}

float CSettings::getOrthoAspectRatioOrientation() const
{
    if( m_orientation == EOrientation::PORTRAIT )
        return m_orthoAspectRatio.w;
    else
        return m_orthoAspectRatio.h;
}

//
//  DESC:  Allow the window to be resized
//
bool CSettings::allowWindowResize() const
{
    return m_allowWindowResize;
}

//
//  DESC: Height and width screen ratio for perspective projection
//
const CSize<float> & CSettings::getScreenAspectRatio() const
{
    return m_screenAspectRatio;
}

//
//  DESC:  Get/Set vSync?
//
bool CSettings::getVSync() const
{
    return m_vSync;
}

void CSettings::setVSync( bool value )
{
    m_vSync = value;
}

//
//  DESC:  Get the Vulkan major version
//
int CSettings::getMajorVersion() const
{
    return m_major;
}

//
//  DESC:  Get the Vulkan minor version
//
int CSettings::getMinorVersion() const
{
    return m_minor;
}

//
//  DESC:  Do we want validation layers
//
bool CSettings::isValidationLayers() const
{
    return m_validationLayers;
}

//
//  DESC:  Is the gamepad enabled
//
bool CSettings::isGamePadEnabled() const
{
    return m_gamepadEnabled;
}

//
//  DESC:  Get the gamepad stick dead zone
//
int CSettings::getGamePadStickDeadZone() const
{
    return m_gamepadStickDeadZone;
}

//
//  DESC:  Set the gamepad stick dead zone
//
void CSettings::setGamePadStickDeadZone( int value )
{
    m_gamepadStickDeadZone = value;
}

//
//  DESC:  Get/Set full screen
//
bool CSettings::getFullScreen() const
{
    return m_fullScreen;
}

void CSettings::setFullScreen( bool value )
{
    m_fullScreen = value;
}

//
//   DESC:  Is the depth buffer activated by default
//
bool CSettings::activateDepthBuffer() const
{
    return m_activateDepthBuffer;
}

//
//  DESC:  Is the stencil buffer activated by default
//
bool CSettings::activateStencilBuffer() const
{
    return m_activateStencilBuffer;
}

//
//  DESC:  Get the script members
//
const std::string & CSettings::getScriptListTable() const
{
    return m_scriptListTable;
}

const std::string & CSettings::getScriptGroup() const
{
    return m_scriptGroup;
}

const std::string & CSettings::getScriptMain() const
{
    return m_scriptMain;
}

bool CSettings::getSaveByteCode() const
{
    return m_saveByteCode;
}

bool CSettings::getLoadByteCode() const
{
    return m_loadByteCode;
}

bool CSettings::getStripDebugInfo() const
{
    return m_stripDebugInfo;
}

/************************************************************************
*    DESC:  Get the sound frequency
************************************************************************/
int CSettings::getFrequency() const
{
    return m_frequency;
}

/************************************************************************
*    DESC:  Get the sound channels: mono, stero, quad, suround, etc
************************************************************************/
int CSettings::getSoundChannels() const
{
    return m_sound_channels;
}

/************************************************************************
*    DESC:  Get the number of channels used for mixing
************************************************************************/
int CSettings::getMixChannels() const
{
    return m_mix_channels;
}

/************************************************************************
*    DESC:  Get the chunk size. The amount of memory used for mixing.
*           The higher the number, the more latency in sound responce
*           stopping and starting
************************************************************************/
int CSettings::getChunkSize() const
{
    return m_chunksize;
}


/************************************************************************
*    DESC:  Get/Set the Anisotropic setting
************************************************************************/
int CSettings::getAnisotropicLevel() const
{
    return static_cast<int>(m_anisotropicLevel);
}

void CSettings::setAnisotropicLevel( int level )
{
    m_anisotropicLevel = static_cast<ETextFilter>(level);
}

/************************************************************************
*    desc:  Do we want triple buffering?
************************************************************************/
bool CSettings::getTripleBuffering() const
{
    return m_tripleBuffering;
}

/************************************************************************
*    DESC:  Save the settings file
************************************************************************/
void CSettings::saveSettings()
{
    #if 0
    if( !m_mainNode.isEmpty() )
    {
        XMLNode displayListNode = m_mainNode.getChildNode("display");

        if( !displayListNode.isEmpty() )
        {
            // Get the attributes from the "resolution" node
            XMLNode resolutionNode = displayListNode.getChildNode("resolution");

            {
                std::string tmpStr = boost::lexical_cast<std::string>( m_display_size.w );
                resolutionNode.updateAttribute(tmpStr.c_str(), "width", "width");

                tmpStr = boost::lexical_cast<std::string>( m_display_size.h );
                resolutionNode.updateAttribute(tmpStr.c_str(), "height", "height");
            }

            {
                std::string tmpStr = "false";

                if( m_fullScreen )
                    tmpStr = "true";

                resolutionNode.updateAttribute(tmpStr.c_str(), "fullscreen", "fullscreen");
            }
        }

        XMLNode deviceNode = m_mainNode.getChildNode("device");

        if( !deviceNode.isEmpty() )
        {
            {
                // Get the attribute from the "backbuffer" node
                XMLNode backBufferNode = deviceNode.getChildNode("backbuffer");

                std::string tmpStr = "false";

                if( m_vSync )
                    tmpStr = "true";

                backBufferNode.updateAttribute(tmpStr.c_str(), "VSync", "VSync");
            }
            /*{
                // Get the attribute from the "backbuffer" node
                XMLNode textFilterNode = deviceNode.getChildNode("textureFiltering");

                std::string tmpStr = GetTextFilterString();

                textFilterNode.updateAttribute(tmpStr.c_str(), "filter", "filter");
            }*/
        }

        {
            // Get the attribute from the "joypad" node
            XMLNode joypadNode = deviceNode.getChildNode("joypad");

            std::string tmpStr = boost::lexical_cast<std::string>( m_gamepadStickDeadZone );

            joypadNode.updateAttribute(tmpStr.c_str(), "stickDeadZone", "stickDeadZone");
        }

        // Save the settings file
        m_mainNode.writeToFile(m_filePath.c_str(), "utf-8");
    }
    #endif
}
