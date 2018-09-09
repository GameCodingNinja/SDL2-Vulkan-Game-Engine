
/************************************************************************
*    FILE NAME:       device.cpp
*
*    DESCRIPTION:     Interface class to Vulkan API
************************************************************************/

// Physical component dependency
#include <system/device.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <utilities/matrix.h>
#include <common/texture.h>
#include <common/color.h>
#include <common/memorybuffer.h>
#include <common/uniformbufferobject.h>
#include <common/vertex.h>
#include <common/pipeline.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>
#include <SDL_vulkan.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CDevice::CDevice()
{
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CDevice::~CDevice()
{
}


/***************************************************************************
*   DESC:  Create the window and Vulkan instance
****************************************************************************/
void CDevice::create( std::function<void(uint32_t)> callback, const std::string & pipelineCfg )
{
    // Set the command buffer call back to be called from the game
    RecordCommandBufferCallback = callback;

    // Initialize SDL - The File I/O and Threading subsystems are initialized by default.
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_TIMER ) < 0 )
        throw NExcept::CCriticalException("SDL could not initialize!", SDL_GetError() );

    // All file I/O is handled by SDL and SDL_Init must be called before doing any I/O.
    CSettings::Instance().loadXML();

    // Get the render size of the window
    const CSize<int> size( CSettings::Instance().getSize() );

    uint32_t flags( SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN );
    #if !(defined(__IOS__) || defined(__ANDROID__))
    flags |= SDL_WINDOW_RESIZABLE;
    #endif

    // Create window
    m_pWindow = SDL_CreateWindow( "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.getW(), size.getH(), flags );
    if( m_pWindow == nullptr )
        throw NExcept::CCriticalException("Game window could not be created!", SDL_GetError() );

    uint32_t instanceExtensionCount(0);
    if( !SDL_Vulkan_GetInstanceExtensions(m_pWindow, &instanceExtensionCount, nullptr) || (instanceExtensionCount == 0) )
        throw NExcept::CCriticalException("Could not retrieve Vulkan instance extension count!", SDL_GetError() );

    std::vector<const char*> instanceExtensionNameVec(instanceExtensionCount);
    std::vector<const char*> validationNameVec;

    if( !SDL_Vulkan_GetInstanceExtensions(m_pWindow, &instanceExtensionCount, instanceExtensionNameVec.data()) )
        throw NExcept::CCriticalException("Could not retrieve Vulkan instance extension names!", SDL_GetError() );

    // If we want validation, add it and debug reporting extension
    if( CSettings::Instance().isValidationLayers() )
    {
        validationNameVec.push_back("VK_LAYER_LUNARG_standard_validation");
        instanceExtensionNameVec.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );

        // Print out extension list for validation layers
        for( auto iter : instanceExtensionNameVec )
            NGenFunc::PostDebugMsg( "Instance Extension: " + std::string(iter));
    }

    // Create the Vulkan instance and graphics pipeline
    CDeviceVulkan::create( validationNameVec, instanceExtensionNameVec );

    // Create the pipelines
    createPipelines( pipelineCfg );

    // Set the full screen
    if( CSettings::Instance().getFullScreen() )
        setFullScreen( CSettings::Instance().getFullScreen() );

    // Depth testing is off by default. Enable it?
    //if( CSettings::Instance().getEnableDepthBuffer() )
    //    glEnable(GL_DEPTH_TEST);

    // Init current gamepads plugged in at startup
    initStartupGamepads();
}


/***************************************************************************
*   DESC:  Destroy the window and Vulkan instance
****************************************************************************/
void CDevice::destroy()
{
    // Destroy the Vulkan instance
    CDeviceVulkan::destroy();

    if( m_pWindow != nullptr )
    {
        SDL_DestroyWindow( m_pWindow );
        m_pWindow = nullptr;
    }
}


/***************************************************************************
*   DESC:  A controlled way to destroy the game created assets
****************************************************************************/
void CDevice::destroyAssets()
{
    if( m_logicalDevice != VK_NULL_HANDLE )
    {
        // Free all command pool groups
        for( auto & iter : m_commandPoolMap )
            vkDestroyCommandPool( m_logicalDevice, iter.second, nullptr );

        m_commandPoolMap.clear();

        // Free all textures in all groups
        for( auto & mapIter : m_textureMapMap )
        {
            for( auto & iter : mapIter.second )
            {
                vkDestroyImage( m_logicalDevice, iter.second.m_textureImage, nullptr );
                vkFreeMemory( m_logicalDevice, iter.second.m_textureImageMemory, nullptr );
                vkDestroyImageView( m_logicalDevice, iter.second.m_textureImageView, nullptr );
                vkDestroySampler( m_logicalDevice, iter.second.m_textureSampler, nullptr );
            }
        }

        m_textureMapMap.clear();

        // Free all descriptor pool groups
        for( auto & mapIter : m_descriptorPoolMapMap )
            for( auto & iter : mapIter.second )
                vkDestroyDescriptorPool( m_logicalDevice, iter.second, nullptr );

        m_descriptorPoolMapMap.clear();

        // Free all memory buffer groups
        for( auto & mapIter : m_memoryBufferMapMap )
        {
            for( auto & iter : mapIter.second )
            {
                vkDestroyBuffer( m_logicalDevice, iter.second.m_buffer, nullptr );
                vkFreeMemory( m_logicalDevice, iter.second.m_deviceMemory, nullptr );
            }
        }

        m_memoryBufferMapMap.clear();

        // Free all the shader modules
        for( auto & iter : m_shaderModuleMap )
            vkDestroyShaderModule( m_logicalDevice, iter.second, nullptr );

        m_shaderModuleMap.clear();

        // Free pipeline descriptor set layout
        for( auto & iter : m_descriptorSetLayoutMap )
            vkDestroyDescriptorSetLayout( m_logicalDevice, iter.second, nullptr );

        m_descriptorSetLayoutMap.clear();

        // Free pipeline descriptor set layout
        for( auto & iter : m_pipelineLayoutMap )
            vkDestroyPipelineLayout( m_logicalDevice, iter.second, nullptr );

        m_pipelineLayoutMap.clear();

    }
}


/***************************************************************************
*   DESC:  Destroy the swap chain
****************************************************************************/
void CDevice::destroySwapChain()
{
    CDeviceVulkan::destroySwapChain();

    if( m_logicalDevice != VK_NULL_HANDLE )
    {
        // Free all pipelines. DO NOT clear the map!
        // Need the handles to the shaders to recreate the pipeline
        for( auto & iter : m_pipelineDataVec )
        {
            vkDestroyPipeline( m_logicalDevice, iter.m_pipeline, nullptr );
            iter.m_pipeline = VK_NULL_HANDLE;
        }
    }
}


/***************************************************************************
*   DESC:  Delete memory buffer
****************************************************************************/
void CDevice::deleteMemoryBuffer( std::vector<CMemoryBuffer> & uniformBufVec )
{
    if( !uniformBufVec.empty()  )
    {
        for( auto & iter : uniformBufVec )
        {
            vkDestroyBuffer( m_logicalDevice, iter.m_buffer, nullptr );
            vkFreeMemory( m_logicalDevice, iter.m_deviceMemory, nullptr );
        }

        uniformBufVec.clear();
    }
}


/***************************************************************************
*   DESC:  Update the command buffer vector
****************************************************************************/
void CDevice::updateCommandBuffer( VkCommandBuffer cmdBuf )
{
    m_secondaryCommandBufVec.push_back( cmdBuf );
}


/***************************************************************************
*   DESC:  Record the command buffers
****************************************************************************/
void CDevice::recordCommandBuffers( uint32_t cmdBufIndex )
{
    //vkResetCommandBuffer( m_primaryCmdBufVec[cmdBufIndex], 0 );

    // Start command buffer recording
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

    if( (m_lastResult = vkBeginCommandBuffer( m_primaryCmdBufVec[cmdBufIndex], &beginInfo )) )
        throw NExcept::CCriticalException(
            "Vulkan Error!",
            boost::str( boost::format("Could not begin recording command buffer! %s") % getError() ) );

    // Accessed by attachment index. Current attachments are color and depth
    std::vector<VkClearValue> clearValues(2);
    clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    // Start a render pass
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_framebufferVec[cmdBufIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapchainInfo.imageExtent;
    renderPassInfo.clearValueCount = clearValues.size();
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass( m_primaryCmdBufVec[cmdBufIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS );

    // Clear out the vector for the next round of command buffers
    m_secondaryCommandBufVec.clear();

    // Have the game sprites that are to be rendered update the vector with their command buffer
    RecordCommandBufferCallback( cmdBufIndex );

    // Execute the secondary command buffers
    if( !m_secondaryCommandBufVec.empty() )
        vkCmdExecuteCommands( m_primaryCmdBufVec[cmdBufIndex], m_secondaryCommandBufVec.size(), m_secondaryCommandBufVec.data() );

    vkCmdEndRenderPass( m_primaryCmdBufVec[cmdBufIndex] );

    if( (m_lastResult = vkEndCommandBuffer( m_primaryCmdBufVec[cmdBufIndex] )) )
        throw NExcept::CCriticalException(
            "Vulkan Error!",
            boost::str( boost::format("Could not record command buffer! %s") % getError() ) );
}


/***************************************************************************
*   DESC:  Render the frame
****************************************************************************/
void CDevice::render()
{
    vkWaitForFences( m_logicalDevice, 1, &m_frameFenceVec[m_currentFrame], VK_TRUE, UINT64_MAX );

    uint32_t imageIndex(0);
    m_lastResult = vkAcquireNextImageKHR( m_logicalDevice, m_swapchain, UINT64_MAX, m_imageAvailableSemaphoreVec[m_currentFrame], VK_NULL_HANDLE, &imageIndex );
    if( (m_lastResult == VK_ERROR_OUT_OF_DATE_KHR) || (m_lastResult == VK_SUBOPTIMAL_KHR) )
    {
        recreateSwapChain();
        return;
    }
    else if( m_lastResult == VK_ERROR_SURFACE_LOST_KHR )
        createSurface();

    else if( m_lastResult != VK_SUCCESS )
        throw NExcept::CCriticalException(
            "Vulkan Error!",
            boost::str( boost::format("Could not present swap chain image! %s") % getError() ) );

    // Record the command buffers
    recordCommandBuffers( imageIndex );

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphoreVec[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_primaryCmdBufVec[imageIndex];

    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphoreVec[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences( m_logicalDevice, 1, &m_frameFenceVec[m_currentFrame] );

    if( (m_lastResult = vkQueueSubmit( m_graphicsQueue, 1, &submitInfo, m_frameFenceVec[m_currentFrame] )) )
        throw NExcept::CCriticalException(
            "Vulkan Error!",
            boost::str( boost::format("Could not submit draw command buffer! %s") % getError() ) );

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // Present the swap chain
    m_lastResult = vkQueuePresentKHR( m_presentQueue, &presentInfo );
    if( (m_lastResult == VK_ERROR_OUT_OF_DATE_KHR) || (m_lastResult == VK_SUBOPTIMAL_KHR) )
        recreateSwapChain();

    else if( m_lastResult == VK_ERROR_SURFACE_LOST_KHR )
        createSurface();

    else if( m_lastResult != VK_SUCCESS )
        throw NExcept::CCriticalException(
            "Vulkan Error!",
            boost::str( boost::format("Could not present swap chain image! %s") % getError() ) );

    m_currentFrame = (m_currentFrame + 1) % m_maxConcurrentFrames;
}


/***************************************************************************
*   DESC:  Create the surface
****************************************************************************/
void CDevice::createSurface()
{
    if( !SDL_Vulkan_CreateSurface( m_pWindow, m_vulkanInstance, &m_vulkanSurface ) )
        throw NExcept::CCriticalException("Could not create Vulkan surface!", SDL_GetError() );
}


/************************************************************************
*    DESC:  Create the command pool group
************************************************************************/
std::vector<VkCommandBuffer> CDevice::createSecondaryCommandBuffers( const std::string & group )
{
    // Find the command pool group
    auto iter = m_commandPoolMap.find( group );
    if( iter == m_commandPoolMap.end() )
        throw NExcept::CCriticalException(
            "Vulkan Error!",
            boost::str( boost::format("Command pool group not found! %s") % group ) );

    return CDeviceVulkan::createSecondaryCommandBuffers( iter->second );
}


/************************************************************************
*    DESC:  Load the image from file path
************************************************************************/
CTexture & CDevice::createTexture( const std::string & group, const std::string & filePath, bool mipMap )
{
    // Create the map group if it doesn't already exist
    auto mapIter = m_textureMapMap.find( group );
    if( mapIter == m_textureMapMap.end() )
        mapIter = m_textureMapMap.emplace( group, std::map<const std::string, CTexture>() ).first;

    // See if this texture has already been loaded
    auto iter = mapIter->second.find( filePath );

    // If it's not found, load the texture and add it to the list
    if( iter == mapIter->second.end() )
    {
        CTexture texture;
        texture.m_textFilePath = filePath;

        // Load the image from file path
        CDeviceVulkan::createTexture( texture, mipMap );

        // Insert the new texture info
        iter = mapIter->second.emplace( filePath, texture ).first;
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Create the descriptor pool group for the textures
************************************************************************/
void CDevice::createDescriptorPoolGroup(
    const std::string & group, const std::string & descrId, const CDescriptorData & descData, size_t count )
{
    // Create the map group if it doesn't already exist
    auto mapIter = m_descriptorPoolMapMap.find( group );
    if( mapIter == m_descriptorPoolMapMap.end() )
        mapIter = m_descriptorPoolMapMap.emplace( group, std::map< const std::string, VkDescriptorPool >() ).first;

    // See if this texture has already been loaded
    auto iter = mapIter->second.find( descrId );

    // If it's not found, create the descriptor pool
    if( iter == mapIter->second.end() )
    {
        // Create the descriptor pool with enough allocations for this group of specific descriptors
        VkDescriptorPool descriptorPool = CDeviceVulkan::createDescriptorPool( descData, count );

        // Add the pool to the map
        mapIter->second.emplace( descrId, descriptorPool );
    }
}


/***************************************************************************
*   DESC:  Create descriptor sets
****************************************************************************/
std::vector<VkDescriptorSet> CDevice::createDescriptorSet(
    const std::string & group,
    int pipelineIndex,
    const CTexture & texture,
    const std::vector<CMemoryBuffer> & uniformBufVec )
{
    // Create the map group if it doesn't already exist
    auto descrSetMapIter = m_descriptorSetMapMap.find( group );
    if( descrSetMapIter == m_descriptorSetMapMap.end() )
        descrSetMapIter = m_descriptorSetMapMap.emplace( group, std::map< const std::string, std::vector<VkDescriptorSet> >() ).first;
    
    auto & rPipelineData = getPipelineData( pipelineIndex );

    // See if this descriptor set has already been created. The key is the descriptor Id and the texture path
    auto descrSetIter = descrSetMapIter->second.find( rPipelineData.m_descriptorId + texture.m_textFilePath );
    if( descrSetIter == descrSetMapIter->second.end() )
    {
        // Fetch the descriptor pool. It's needed for allocating a new descriptor set
        // Descriptor pool should have already been created by this point
        auto descrPoolMapIter = m_descriptorPoolMapMap.find( group );
        if( descrPoolMapIter == m_descriptorPoolMapMap.end() )
            throw NExcept::CCriticalException(
                "Vulkan Error!", boost::str( boost::format("Descriptor pool group hasn't been created! %s") % group ) );
        
        auto descrPoolIter = descrPoolMapIter->second.find( rPipelineData.m_descriptorId );
        if( descrPoolIter == descrPoolMapIter->second.end() )
            throw NExcept::CCriticalException(
                "Vulkan Error!", boost::str( boost::format("Descriptor pool hasn't been created! %s") % group ) );
        
        // Get the descriptor data
        auto & rDescData = getDescriptorData( rPipelineData.m_descriptorId );
        
        // Create the descriptor set
        auto descrSetVec = CDeviceVulkan::createDescriptorSet(
            texture, rDescData, rPipelineData, uniformBufVec, descrPoolIter->second );
        
        // Add the descriptor set to the map
        descrSetIter = descrSetMapIter->second.emplace( rPipelineData.m_descriptorId + texture.m_textFilePath, descrSetVec ).first;
    }

    return descrSetIter->second;
}


/************************************************************************
*    DESC:  Create the command pool group for command buffer generation
************************************************************************/
void CDevice::createCommandPoolGroup( const std::string & group )
{
    // Create the command pool. It shouldn't have been already created
    if( m_commandPoolMap.find( group ) != m_commandPoolMap.end() )
        throw NExcept::CCriticalException(
            "Vulkan Error!",
            boost::str( boost::format("Command pool already created! %s") % group ) );

    // Create the command pool
    VkCommandPool commandPool = CDeviceVulkan::createCommandPool();

    // Add the pool to the map
    m_commandPoolMap.emplace( group, commandPool );
}


/************************************************************************
*    DESC:  Create the pipelines from config file
************************************************************************/
void CDevice::createPipelines( const std::string & filePath )
{
    // Map containing UBO information
    std::map< const std::string, CUBO > uboMap;

    // Map containing shader information
    std::map< const std::string, CShader > shaderMap;

    // Open and parse the XML file:
    XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "pipelinemap" );


    // Create the ubo list
    const XMLNode uboLstNode = node.getChildNode("uboList");

    for( int i = 0; i < uboLstNode.nChildNode(); ++i )
    {
        const std::string id = uboLstNode.getChildNode("ubo", i).getAttribute("id");

        uboMap.emplace( id, NUBO::GetUboSize(id) );
    }


    // Create the descriptor list
    const XMLNode descriptorLstNode = node.getChildNode("descriptorList");

    for( int i = 0; i < descriptorLstNode.nChildNode(); ++i )
    {
        const XMLNode descriptorNode = descriptorLstNode.getChildNode(i);

        const std::string descId = descriptorNode.getAttribute("id");

        CDescriptorData descriptorData;

        // Populate with the descriptors with UBO info
        for( int j = 0; j < descriptorNode.nChildNode("ubo"); ++j )
        {
            const std::string id = descriptorNode.getChildNode("ubo", j).getAttribute("id");

            auto iter = uboMap.find( id );
            if( iter == uboMap.end() )
                throw NExcept::CCriticalException(
                    "Vulkan Error!",
                    boost::str( boost::format("UBO id not found! %s") % id ) );

            descriptorData.m_uboVec.push_back( iter->second );
        }

        // Populate with the descriptors with binding info
        for( int j = 0; j < descriptorNode.nChildNode("binding"); ++j )
        {
            const std::string id = descriptorNode.getChildNode("binding", j).getAttribute("id");

            descriptorData.m_descriptorIdVec.push_back( id );
        }

        m_descriptorDataMap.emplace( descId, descriptorData );
    }


    // Create the shaders
    const XMLNode shaderLstNode = node.getChildNode("shaderList");

    for( int i = 0; i < shaderLstNode.nChildNode(); ++i )
    {
        const XMLNode shaderNode = shaderLstNode.getChildNode(i);

        const std::string id = shaderNode.getAttribute("id");

        CShader shader;

        shader.m_vert = createShader( shaderNode.getChildNode("vert").getAttribute("file") );
        shader.m_frag = createShader( shaderNode.getChildNode("frag").getAttribute("file") );

        shaderMap.emplace( id, shader );
    }


    // Create the descriptor set and pipeline layout
    for( auto & iter : m_descriptorDataMap )
    {
        // Create the descriptor set layout
        VkDescriptorSetLayout descriptorSetLayout = CDeviceVulkan::createDescriptorSetLayout( iter.second );
        m_descriptorSetLayoutMap.emplace( iter.first, descriptorSetLayout );

        // Create the pipeline layout
        VkPipelineLayout pipelineLayout = CDeviceVulkan::createPipelineLayout( descriptorSetLayout );
        m_pipelineLayoutMap.emplace( iter.first, pipelineLayout );
    }


    // Create the pipeline list
    const XMLNode pipelineLstNode = node.getChildNode("pipelineList");

    for( int i = 0; i < pipelineLstNode.nChildNode(); ++i )
    {
        const XMLNode pipelineNode = pipelineLstNode.getChildNode(i);

        CPipelineData pipelineData;

        const std::string pipelineId = pipelineNode.getAttribute("id");

        // Get the shader
        const std::string shaderId = pipelineNode.getAttribute("shaderId");

        auto shaderIter = shaderMap.find( shaderId );
        if( shaderIter == shaderMap.end() )
            throw NExcept::CCriticalException(
                "Vulkan Error!",
                boost::str( boost::format("Shader id not found! %s") % shaderId ) );

        pipelineData.m_shader = shaderIter->second;

        // Get the descriptor layout
        pipelineData.m_descriptorId = pipelineNode.getAttribute("descriptorId");

        auto discrIter = m_descriptorSetLayoutMap.find( pipelineData.m_descriptorId );
        if( discrIter == m_descriptorSetLayoutMap.end() )
            throw NExcept::CCriticalException(
                "Vulkan Error!",
                boost::str( boost::format("Descriptor id not found! %s") % pipelineData.m_descriptorId ) );

        pipelineData.m_descriptorSetLayout = discrIter->second;

        // Get the pipeline layout. Same id as the descriptor
        auto pipelineLayoutIter = m_pipelineLayoutMap.find( pipelineData.m_descriptorId );
        if( pipelineLayoutIter == m_pipelineLayoutMap.end() )
            throw NExcept::CCriticalException(
                "Vulkan Error!",
                boost::str( boost::format("Pipeline id not found! %s") % pipelineData.m_descriptorId ) );

        pipelineData.m_pipelineLayout = pipelineLayoutIter->second;

        // Get the vertex input descriptions
        const std::string vertexInputDescrId = pipelineNode.getAttribute("vertexInputDescrId");
        pipelineData.vertInputBindingDesc = NVertex::getBindingDesc( vertexInputDescrId );
        pipelineData.vertInputAttrDescVec = NVertex::getAttributeDesc( vertexInputDescrId );

        // Create the graphics pipeline
        CDeviceVulkan::createPipeline( pipelineData );

        // Map for holding index of the pipeline in the vector
        m_pipelineIndexMap.emplace( pipelineId, i );

        // Vector of pipeline data for quick access
        m_pipelineDataVec.emplace_back( pipelineData );
    }
}


/************************************************************************
*    DESC:  Recreate the pipeline
************************************************************************/
void CDevice::recreatePipelines()
{
    for( auto & iter : m_pipelineDataVec )
        CDeviceVulkan::createPipeline( iter );
}


/***************************************************************************
*   DESC:  Create the shader
****************************************************************************/
VkShaderModule CDevice::createShader( const std::string & filePath )
{
    // See if this shader has already been created
    auto iter = m_shaderModuleMap.find( filePath );
    if( iter == m_shaderModuleMap.end() )
    {
        VkShaderModule shader = CDeviceVulkan::createShader( filePath );
        iter = m_shaderModuleMap.emplace(filePath, shader).first;
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Delete a secondary command buffer of a specific group
************************************************************************/
void CDevice::deleteCommandBuffer( const std::string & group, std::vector<VkCommandBuffer> & commandBufVec )
{
    // A command pool shouldn't have been already created
    auto iter = m_commandPoolMap.find( group );
    if( iter == m_commandPoolMap.end() )
        throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Command pool has not been created! %s") % group ) );

    vkFreeCommandBuffers( m_logicalDevice, iter->second, commandBufVec.size(), commandBufVec.data() );
    commandBufVec.clear();
}


/***************************************************************************
*   DESC:  Create the uniform buffer
****************************************************************************/
std::vector<CMemoryBuffer> CDevice::createUniformBuffer( VkDeviceSize sizeOfUniformBuf )
{
    std::vector<CMemoryBuffer> uniformBufVec( m_framebufferVec.size() );

    for( size_t i = 0; i < m_framebufferVec.size(); ++i )
        CDeviceVulkan::createBuffer(
            sizeOfUniformBuf,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBufVec[i].m_buffer,
            uniformBufVec[i].m_deviceMemory );

    return uniformBufVec;
}


/************************************************************************
*    DESC:  Delete group assets
************************************************************************/
void CDevice::deleteGroupAssets( const std::string & group )
{
    // Delete all textures and related assets
    deleteTextureGroup( group );

    // This also deletes all the descriptor sets
    deleteDescriptorPoolGroup( group );

    // Delete the memory buffers
    deleteMemoryBufferGroup( group );

    // This also deletes all the command buffers
    deleteCommandPoolGroup( group );
}


/************************************************************************
*    DESC:  Delete a texture in a group
************************************************************************/
void CDevice::deleteTextureGroup( const std::string & group )
{
    // Free the texture group if it exists
    auto mapIter = m_textureMapMap.find( group );
    if( mapIter != m_textureMapMap.end() )
    {
        // Delete all the textures in this group
        for( auto & iter : mapIter->second )
        {
            vkDestroyImage( m_logicalDevice, iter.second.m_textureImage, nullptr );
            vkFreeMemory( m_logicalDevice, iter.second.m_textureImageMemory, nullptr );
            vkDestroyImageView( m_logicalDevice, iter.second.m_textureImageView, nullptr );
            vkDestroySampler( m_logicalDevice, iter.second.m_textureSampler, nullptr );
        }

        // Erase this group
        m_textureMapMap.erase( mapIter );
    }
}


/************************************************************************
*    DESC:  Delete the command pool group
************************************************************************/
void CDevice::deleteCommandPoolGroup( const std::string & group )
{
    // Free the command pool group if it exists
    auto iter = m_commandPoolMap.find( group );
    if( iter != m_commandPoolMap.end() )
        vkDestroyCommandPool( m_logicalDevice, iter->second, nullptr );

    // Erase this group
    m_commandPoolMap.erase( iter );
}


/************************************************************************
*    DESC:  Delete a Descriptor Pool group
************************************************************************/
void CDevice::deleteDescriptorPoolGroup( const std::string & group )
{
    // Free the descriptor pool group if it exists
    auto mapIter = m_descriptorPoolMapMap.find( group );
    if( mapIter != m_descriptorPoolMapMap.end() )
    {
        for( auto & iter : mapIter->second )
            vkDestroyDescriptorPool( m_logicalDevice, iter.second, nullptr );

        // Erase this group
        m_descriptorPoolMapMap.erase( mapIter );
    }
}


/************************************************************************
*    DESC:  Delete a memory buffer group
************************************************************************/
void CDevice::deleteMemoryBufferGroup( const std::string & group )
{
    // Free the texture group if it exists
    auto mapIter = m_memoryBufferMapMap.find( group );
    if( mapIter != m_memoryBufferMapMap.end() )
    {
        // Delete all the textures in this group
        for( auto & iter : mapIter->second )
        {
            vkDestroyBuffer( m_logicalDevice, iter.second.m_buffer, nullptr );
            vkFreeMemory( m_logicalDevice, iter.second.m_deviceMemory, nullptr );
        }

        // Erase this group
        m_memoryBufferMapMap.erase( mapIter );
    }
}


/***************************************************************************
*   DESC:  Enable/disable v-sync
****************************************************************************/
void CDevice::enableVSync( bool enable )
{
    //if( SDL_GL_SetSwapInterval( (enable == true) ? 1 : 0 ) < 0 )
    //    NGenFunc::PostDebugMsg( boost::str( boost::format("Warning: Unable to set VSync! SDL GL Error: %s") % SDL_GetError() ) );
}


/***************************************************************************
*   DESC:  Set full screen or windowed mode
****************************************************************************/
void CDevice::setFullScreen( bool fullscreen )
{
    if( m_pWindow )
    {
        int flag(0);

        if( fullscreen )
            flag = SDL_WINDOW_FULLSCREEN;

        if( SDL_SetWindowFullscreen( m_pWindow, flag ) < 0 )
            NGenFunc::PostDebugMsg( boost::str( boost::format("Warning: Unable to set full screen! SDL GL Error: %s") % SDL_GetError() ) );
    }
}


/***************************************************************************
*   DESC:  Display error message
****************************************************************************/
void CDevice::displayErrorMsg( const std::string & title, const std::string & msg )
{
    if( m_pWindow )
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(), m_pWindow );
}


/***************************************************************************
*   DESC:  Set window title
****************************************************************************/
void CDevice::setWindowTitle( const std::string & title )
{
    if( m_pWindow )
        SDL_SetWindowTitle( m_pWindow, title.c_str() );
}


/***************************************************************************
*   DESC:  Init current gamepads plugged in at startup
****************************************************************************/
void CDevice::initStartupGamepads()
{
    // May not need this anymore
    int newMappings = SDL_GameControllerAddMappingsFromFile("data/settings/gamecontrollerdb.txt");
    NGenFunc::PostDebugMsg( boost::str( boost::format("New controller mappings found: %d - Number of controllers found: %d") % newMappings % (int)SDL_NumJoysticks() ) );

    for( int i = 0; i < SDL_NumJoysticks(); ++i )
        addGamepad( i );
}


/***************************************************************************
*   DESC:  Add/Remove the game pad
****************************************************************************/
void CDevice::addGamepad( int id )
{
    if( SDL_IsGameController( id ) )
    {
        SDL_GameController * pGamePad = SDL_GameControllerOpen(id);
        if( pGamePad != NULL )
        {
            //NGenFunc::PostDebugMsg( boost::str( boost::format("Game controller added: %d - %s") % id % SDL_GameControllerNameForIndex(id) ) );
            m_pGamepadMap.emplace( id, pGamePad );
        }
    }
}

void CDevice::removeGamepad( int id )
{
    auto iter = m_pGamepadMap.find( id );
    if( iter != m_pGamepadMap.end() )
    {
        SDL_GameControllerClose( iter->second );
        m_pGamepadMap.erase( iter );
    }
}


/***************************************************************************
*   DESC:  Get the gamepad count
****************************************************************************/
size_t CDevice::getGamepadCount()
{
    return m_pGamepadMap.size();
}


/***************************************************************************
*   DESC:  Get window
****************************************************************************/
SDL_Window * CDevice::getWindow()
{
    return m_pWindow;
}


/***************************************************************************
*   DESC:  Show/Hide the Window
****************************************************************************/
void CDevice::showWindow( bool visible )
{
    if( visible )
        SDL_ShowWindow( m_pWindow );
    else
        SDL_HideWindow( m_pWindow );
}


/***************************************************************************
*   DESC:  Wait for Vulkan render to finish
****************************************************************************/
void CDevice::waitForIdle()
{
    // Wait for the logical device to be idle before doing the clean up
    vkDeviceWaitIdle( m_logicalDevice );
}


/***************************************************************************
*   DESC:  Get the frame buffer index
****************************************************************************/
VkFramebuffer CDevice::getFrameBuffer( uint32_t index )
{
    return m_framebufferVec[index];
}


/***************************************************************************
*   DESC:  Get the render pass
****************************************************************************/
VkRenderPass CDevice::getRenderPass()
{
    return m_renderPass;
}


/***************************************************************************
*   DESC:  Get the pipeline
****************************************************************************/
const CPipelineData & CDevice::getPipelineData( int index ) const
{
    return m_pipelineDataVec.at(index);
}


/***************************************************************************
*   DESC:  Get the pipeline index
****************************************************************************/
int CDevice::getPipelineIndex( const std::string & id )
{
    auto iter = m_pipelineIndexMap.find( id );
    if( iter == m_pipelineIndexMap.end() )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Pipeline Id does not exist: %s") % id ) );

    return iter->second;
}


/***************************************************************************
*   DESC:  Get descriptor data map
****************************************************************************/
const std::map< const std::string, CDescriptorData > & CDevice::getDescriptorDataMap() const
{
    return m_descriptorDataMap;
}


/***************************************************************************
*   DESC:  Get descriptor data
****************************************************************************/
const CDescriptorData & CDevice::getDescriptorData( const std::string & id ) const
{
    auto iter = m_descriptorDataMap.find( id );
    if( iter == m_descriptorDataMap.end() )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Descriptor data Id does not exist: %s") % id ) );
    
    return iter->second;
}
