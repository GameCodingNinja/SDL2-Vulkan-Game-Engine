
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
#include <common/model.h>
#include <common/memorybuffer.h>
#include <common/uniformbufferobject.h>
#include <common/vertex.h>
#include <common/pipeline.h>
#include <common/pushdescriptorset.h>
#include <common/meshbinaryfileheader.h>
#include <utilities/xmlParser.h>
#include <utilities/smartpointers.h>

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
void CDevice::create( const std::string & pipelineCfg )
{
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
    
    // Make sure the depth buffer is active along with the stencil buffer
    if( CSettings::Instance().activateStencilBuffer() && !CSettings::Instance().activateDepthBuffer() )
        throw NExcept::CCriticalException("Vulkan Error!", "Can't activate stencil buffer without activating the depth buffer. They are one in the same." );

    uint32_t instanceExtensionCount(0);
    if( !SDL_Vulkan_GetInstanceExtensions(m_pWindow, &instanceExtensionCount, nullptr) || (instanceExtensionCount == 0) )
        throw NExcept::CCriticalException("Could not retrieve Vulkan instance extension count!", SDL_GetError() );

    std::vector<const char*> physicalDeviceExtensionNameVec = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
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

    // Enable extension required for push descriptors
    instanceExtensionNameVec.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    physicalDeviceExtensionNameVec.push_back(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME);

    // Create the Vulkan instance and graphics pipeline
    CDeviceVulkan::create( validationNameVec, instanceExtensionNameVec, physicalDeviceExtensionNameVec );

    // Create the pipelines
    createPipelines( pipelineCfg );

    // Set the full screen
    if( CSettings::Instance().getFullScreen() )
        setFullScreen( CSettings::Instance().getFullScreen() );

    // Init current gamepads plugged in at startup
    initStartupGamepads();
}

void CDevice::create( std::function<void(uint32_t)> callback, const std::string & pipelineCfg )
{
    // Set the command buffer call back to be called from the game
    RecordCommandBufferCallback = callback;
    
    create( pipelineCfg );
}


/***************************************************************************
*   DESC:  Set the callback for command buffer recording
****************************************************************************/
void CDevice::setRecordCommandBufferCallback( std::function<void(uint32_t)> callback )
{
    // Set the command buffer call back to be called from the game
    RecordCommandBufferCallback = callback;
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
            for( auto & iter : mapIter.second )
                freeMemoryBuffer( iter.second );

        m_memoryBufferMapMap.clear();
        
        // Free the shared font IBO buffer
        freeMemoryBuffer( m_sharedFontIbo );
        
        // Free the delete queue
        for( auto & mapIter : m_memoryDeleteMap )
            for( auto & vecIter : mapIter.second )
                freeMemoryBuffer( vecIter );
        
        m_memoryDeleteMap.clear();

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
    clearValues[1].depthStencil = {1.0f, 0xff};

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

    // Have the game sprites that are to be rendered update the vector with their command buffer
    RecordCommandBufferCallback( cmdBufIndex );

    // Execute the secondary command buffers
    if( !m_secondaryCommandBufVec.empty() )
        vkCmdExecuteCommands( m_primaryCmdBufVec[cmdBufIndex], m_secondaryCommandBufVec.size(), m_secondaryCommandBufVec.data() );

    // Clear out the vector for the next round of command buffers
    m_secondaryCommandBufVec.clear();

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

    m_currentFrame = (m_currentFrame + 1) % m_framebufferVec.size();
    
    // Delete the memory buffer if it's been in the queue for one cycle
    // NOTE: This needs to be done after the current frame increment
    auto mapIter = m_memoryDeleteMap.begin();
    while( mapIter != m_memoryDeleteMap.end() )
    {
        if( mapIter->first == m_currentFrame )
        {
            for( auto & vecIter : mapIter->second )
                freeMemoryBuffer( vecIter );
            
            mapIter = m_memoryDeleteMap.erase( mapIter );
        }
        else
        {
            ++mapIter;
        }
    }
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
*    DESC:  Create the command buffers from the pool group
************************************************************************/
std::vector<VkCommandBuffer> CDevice::createSecondaryCommandBuffers( const std::string & group )
{
    return CDeviceVulkan::createSecondaryCommandBuffers( createSecondaryCommandPool( group ) );
}


/************************************************************************
*    DESC:  Create the command pool group
************************************************************************/
VkCommandPool CDevice::createSecondaryCommandPool( const std::string & group )
{
    // Find the command pool group or create it if it doesn't exist
    auto iter = m_commandPoolMap.find( group );
    if( iter == m_commandPoolMap.end() )
    {
        // Create the command pool
        VkCommandPool commandPool = CDeviceVulkan::createCommandPool( m_graphicsQueueFamilyIndex );

        // Add the pool to the map
        iter = m_commandPoolMap.emplace( group, commandPool ).first;
    }

    return iter->second;
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
*   DESC:  Create the uniform buffer
****************************************************************************/
std::vector<CMemoryBuffer> CDevice::createUniformBufferVec( uint32_t pipelineIndex )
{
    // Get the descriptor data from the pipeline id
    const CPipelineData & rPipelineData = getPipelineData( pipelineIndex );
    const CDescriptorData & rDescriptorData = getDescriptorData( rPipelineData.m_descriptorId );

    // Create the uniform buffer
    return CDeviceVulkan::createUniformBufferVec( rDescriptorData.m_descriptorVec.front().m_ubo.uboSize );
}


/***************************************************************************
*   DESC:  Create push descriptor set
****************************************************************************/
void CDevice::createPushDescriptorSet(
    uint32_t pipelineIndex,
    const CTexture & texture,
    const std::vector<CMemoryBuffer> & uniformBufVec,
    CPushDescriptorSet & pushDescSet )
{
    // Get the descriptor data
    auto & rPipelineData = getPipelineData( pipelineIndex );
    auto & rDescData = getDescriptorData( rPipelineData.m_descriptorId );

    // Copy over the function call
    pushDescSet.vkCmdPushDescriptorSetKHR = vkCmdPushDescriptorSetKHR;

    for( auto & uboIter : uniformBufVec )
    {
        std::vector<VkWriteDescriptorSet> writeDescriptorSetVec;
        int bindingOffset = 0;

        for( auto & descIdIter : rDescData.m_descriptorVec )
        {
            if( descIdIter.m_descrId == "UNIFORM_BUFFER" )
            {
                // Make sure this UBO has a size
                if( descIdIter.m_ubo.uboSize == 0 )
                    throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Uniform Buffer UBO size is 0! %s") % descIdIter.m_descrId ) );

                VkDescriptorBufferInfo bufferInfo = {};
                bufferInfo.buffer = uboIter.m_buffer;
                bufferInfo.range = descIdIter.m_ubo.uboSize;

                pushDescSet.m_descriptorBufferInfoDeq.push_back( bufferInfo );

                VkWriteDescriptorSet writeDescriptorSet = {};
                writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSet.dstBinding = bindingOffset++;
                writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                writeDescriptorSet.descriptorCount = 1;
                writeDescriptorSet.pBufferInfo = &pushDescSet.m_descriptorBufferInfoDeq.back();

                writeDescriptorSetVec.push_back( writeDescriptorSet );
            }
            else if( descIdIter.m_descrId == "COMBINED_IMAGE_SAMPLER" )
            {
                VkDescriptorImageInfo imageInfo = {};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = texture.m_textureImageView;
                imageInfo.sampler = texture.m_textureSampler;

                pushDescSet.m_descriptorImageInfoDeq.push_back( imageInfo );

                VkWriteDescriptorSet writeDescriptorSet = {};
                writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSet.dstBinding = bindingOffset++;
                writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                writeDescriptorSet.descriptorCount = 1;
                writeDescriptorSet.pImageInfo = &pushDescSet.m_descriptorImageInfoDeq.back();

                writeDescriptorSetVec.push_back( writeDescriptorSet );
            }
            else
            {
                throw NExcept::CCriticalException( "Vulkan Error!", boost::str( boost::format("Create Descriptor Set binding not defined! %s") % descIdIter.m_descrId ) );
            }
        }
        pushDescSet.m_pushDescriptorSetVec.push_back(writeDescriptorSetVec);
    }
}


/************************************************************************
*    DESC:  Create the pipelines from config file
************************************************************************/
void CDevice::createPipelines( const std::string & filePath )
{
    // Map containing shader information
    std::map< const std::string, CShader > shaderMap;

    // Open and parse the XML file:
    XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "pipelinemap" );


    // Create the ubo list
    const XMLNode uboLstNode = node.getChildNode("uboList");

    for( int i = 0; i < uboLstNode.nChildNode(); ++i )
    {
        const std::string id = uboLstNode.getChildNode("ubo", i).getAttribute("id");

        m_uboDataMap.emplace( std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id, NUBO::GetUboSize(id)) );
    }


    // Create the descriptor list
    const XMLNode descriptorLstNode = node.getChildNode("descriptorList");

    for( int i = 0; i < descriptorLstNode.nChildNode(); ++i )
    {
        const XMLNode descriptorNode = descriptorLstNode.getChildNode(i);
        const std::string descId = descriptorNode.getAttribute("id");

        CDescriptorData descriptorData;

        // Populate with the descriptors with binding info
        for( int j = 0; j < descriptorNode.nChildNode("binding"); ++j )
        {
            CDescriptorData::CDescriptor descriptor;

            const XMLNode bindNode = descriptorNode.getChildNode(j);

            descriptor.m_descrId = bindNode.getAttribute("id");

            if( bindNode.isAttributeSet("uboId") )
            {
                const std::string uboId = bindNode.getAttribute("uboId");

                auto iter = m_uboDataMap.find( uboId );
                if( iter == m_uboDataMap.end() )
                    throw NExcept::CCriticalException(
                        "Vulkan Error!", boost::str( boost::format("UBO id not found! %s") % uboId ) );

                descriptor.m_ubo = iter->second;
            }

            descriptorData.m_descriptorVec.push_back( descriptor );
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
        
        // Get the attribute from the "depthStencilBuffer" node
        const XMLNode depthStencilBufferNode = pipelineNode.getChildNode("depthStencilBuffer");
        if( !depthStencilBufferNode.isEmpty() )
        {
            // Do we enable the depth testing
            if( depthStencilBufferNode.isAttributeSet("enableDepthTest") )
                pipelineData.m_enableDepthTest = ( std::strcmp( depthStencilBufferNode.getAttribute("enableDepthTest"), "true" ) == 0 );

            // Do we enable the stencil testing
            if( depthStencilBufferNode.isAttributeSet("enableStencilTest") )
                pipelineData.m_enableStencilTest = ( std::strcmp( depthStencilBufferNode.getAttribute("enableStencilTest"), "true" ) == 0 );
            
            // Is this the stencil pipeline
            if( depthStencilBufferNode.isAttributeSet("stencilPipeline") )
                pipelineData.m_stencilPipeline = ( std::strcmp( depthStencilBufferNode.getAttribute("stencilPipeline"), "true" ) == 0 );
        }

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
    if( !commandBufVec.empty() )
    {
        // A command pool shouldn't have been already created
        auto iter = m_commandPoolMap.find( group );
        if( iter != m_commandPoolMap.end() )
        {
            vkFreeCommandBuffers( m_logicalDevice, iter->second, commandBufVec.size(), commandBufVec.data() );
            commandBufVec.clear();
        }
    }
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
    
    // Delete the model group
    deleteModelGroup( group );
}


/************************************************************************
*    DESC:  Delete the texture in a group
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
    {
        vkDestroyCommandPool( m_logicalDevice, iter->second, nullptr );

        // Erase this group
        m_commandPoolMap.erase( iter );
    }
}


/************************************************************************
*    DESC:  Delete the Descriptor Pool group
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
*    DESC:  Delete the memory buffer group
************************************************************************/
void CDevice::deleteMemoryBufferGroup( const std::string & group )
{
    // Free the memory buffer group if it exists
    auto mapIter = m_memoryBufferMapMap.find( group );
    if( mapIter != m_memoryBufferMapMap.end() )
    {
        for( auto & iter : mapIter->second )
        {
            vkDestroyBuffer( m_logicalDevice, iter.second.m_buffer, nullptr );
            vkFreeMemory( m_logicalDevice, iter.second.m_deviceMemory, nullptr );
        }

        // Erase this group
        m_memoryBufferMapMap.erase( mapIter );
    }
}


/************************************************************************
*    DESC:  Delete the model group
*           NOTE: Nothing to delete here because the textures, vbo and ibo
*                 are being held in their respective groups
************************************************************************/
void CDevice::deleteModelGroup( const std::string & group )
{
    // Erase this group
    auto mapIter = m_modelMapMap.find( group );
    if( mapIter != m_modelMapMap.end() )
        m_modelMapMap.erase( mapIter );
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
    
    NGenFunc::PostDebugMsg( boost::str( boost::format("Error: %s, %s") % title.c_str() % msg.c_str() ) );
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
    if( m_logicalDevice != VK_NULL_HANDLE )
        vkDeviceWaitIdle( m_logicalDevice );
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
        throw NExcept::CCriticalException("Vulkan Error!", boost::str( boost::format("Pipeline Id does not exist: %s") % id ) );

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


/***************************************************************************
*   DESC:  Begin the recording of the command buffer
****************************************************************************/
void CDevice::beginCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer )
{
    // Setup to begine recording the command buffer
    VkCommandBufferInheritanceInfo cmdBufInheritanceInfo = {};
    cmdBufInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufInheritanceInfo.framebuffer = m_framebufferVec[index];
    cmdBufInheritanceInfo.renderPass = m_renderPass;

    VkCommandBufferBeginInfo cmdBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };  // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    cmdBeginInfo.flags =  VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    cmdBeginInfo.pInheritanceInfo = &cmdBufInheritanceInfo;

    // Start recording the command buffer
    vkBeginCommandBuffer( cmdBuffer, &cmdBeginInfo );
}


/***************************************************************************
*   DESC:  End the recording of the command buffer
****************************************************************************/
void CDevice::endCommandBuffer( VkCommandBuffer cmdBuffer )
{
    // Stop recording the command buffer
    vkEndCommandBuffer( cmdBuffer );

    // Pass the command buffer to the queue
    updateCommandBuffer( cmdBuffer );
}


/************************************************************************
*    DESC: Get the memory buffer if it exists
************************************************************************/
CMemoryBuffer CDevice::getMemoryBuffer( const std::string & group, const std::string & id )
{
    // See if the group exists
    auto mapMapIter = m_memoryBufferMapMap.find( group );
    if( mapMapIter == m_memoryBufferMapMap.end() )
        return CMemoryBuffer();

    // See if this vertex buffer ID has already been created
    auto mapIter = mapMapIter->second.find( id );
    if( mapIter == mapMapIter->second.end() )
        return CMemoryBuffer();

    return mapIter->second;
}


/************************************************************************
*    DESC:  Create the shared font IBO buffer
************************************************************************/
void CDevice::createSharedFontIBO( std::vector<uint16_t> & iboVec )
{
    // If the new indices are greater then the current, init the IBO with the newest
    if( iboVec.size() > m_currentMaxFontIndices )
    {
        if( !m_sharedFontIbo.isEmpty() )
        {
            vkDestroyBuffer( m_logicalDevice, m_sharedFontIbo.m_buffer, nullptr );
            vkFreeMemory( m_logicalDevice, m_sharedFontIbo.m_deviceMemory, nullptr );
        }

        CDeviceVulkan::creatMemoryBuffer( iboVec, m_sharedFontIbo, VK_BUFFER_USAGE_INDEX_BUFFER_BIT );

        m_currentMaxFontIndices = iboVec.size();
    }
}


/************************************************************************
*    DESC:  Get the shared font ibo buffer
************************************************************************/
CMemoryBuffer & CDevice::getSharedFontIBO()
{
    return m_sharedFontIbo;
}


/************************************************************************
*    DESC:  Get the shared font ibo max indice count
************************************************************************/
size_t CDevice::getSharedFontIBOMaxIndiceCount()
{
    return m_currentMaxFontIndices;
}


/************************************************************************
*    DESC:  Free the memory buffer
************************************************************************/
void CDevice::freeMemoryBuffer( CMemoryBuffer & memoryBuffer )
{
    if( memoryBuffer.m_buffer != VK_NULL_HANDLE )
    {
        vkDestroyBuffer( m_logicalDevice, memoryBuffer.m_buffer, nullptr );
        memoryBuffer.m_buffer = VK_NULL_HANDLE;
    }

    if( memoryBuffer.m_deviceMemory != VK_NULL_HANDLE )
    {
        vkFreeMemory( m_logicalDevice, memoryBuffer.m_deviceMemory, nullptr );
        memoryBuffer.m_deviceMemory = VK_NULL_HANDLE;
    }
}


/************************************************************************
*    DESC:  Add a memory buffer to the delete 
************************************************************************/
void CDevice::AddToDeleteQueue( CMemoryBuffer & memBuff )
{
    auto mapIter = m_memoryDeleteMap.find( m_currentFrame );
    if( mapIter != m_memoryDeleteMap.end() )
    {
        mapIter->second.push_back( memBuff );
    }
    else
    {
        auto iter = m_memoryDeleteMap.emplace( m_currentFrame, std::vector<CMemoryBuffer>() );
        iter.first->second.push_back( memBuff );
    }
}

void CDevice::AddToDeleteQueue( std::vector<CMemoryBuffer> & commandBufVec )
{
    for( auto & iter : commandBufVec )
        AddToDeleteQueue( iter );
}


/************************************************************************
 *    DESC: Create a model
 ************************************************************************/
void CDevice::createModel(
    const std::string & group,
    const std::string & filePath,
    CModel & model )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_modelMapMap.find( group );
    if( mapMapIter == m_modelMapMap.end() )
        mapMapIter = m_modelMapMap.emplace( group, std::map<const std::string, CModel>() ).first;

    // See if the ID has already been loaded
    auto mapIter = mapMapIter->second.find( filePath );

    // If it's not found, create the mesh buffer and add it to the list
    if( mapIter == mapMapIter->second.end() )
    {
        mapIter = mapMapIter->second.emplace( filePath, CModel() ).first;

        loadFrom3DM( group, filePath, mapIter->second );
    }

    // Copy the mesh data to the passed in mesh vector
    model = mapIter->second;
}


/************************************************************************
 *    DESC: Load 3d mesh file
 ************************************************************************/
void CDevice::loadFrom3DM(
    const std::string & group,
    const std::string & filePath,
    CModel & model )
{
    // Open file for reading
    NSmart::scoped_SDL_filehandle_ptr<SDL_RWops> scpFile( SDL_RWFromFile( filePath.c_str(), "rb" ) );
    if( scpFile.isNull() )
        throw NExcept::CCriticalException( "File Load Error!",
            boost::str( boost::format( "Error Loading file (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );

    // Read in the file header
    CMeshBinaryFileHeader fileHeader;
    SDL_RWread( scpFile.get(), &fileHeader, 1, sizeof( fileHeader ) );

    // Check to make sure we're loading in the right kind of file
    if( fileHeader.file_header != MESH_FILE_HEADER )
        throw NExcept::CCriticalException( "Visual Mesh Load Error!",
            boost::str( boost::format( "File header mismatch (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );

    if( fileHeader.text_count > 0 )
        // Load with textures
        load3DM( scpFile.get(), fileHeader, group, filePath, model );
    //else
        // Load without textures
        //LoadFromFile( scpFile.get(), fileHeader, filePath, modelVec );
}


/************************************************************************
 *    DESC: Load 3d mesh file with textures
 ************************************************************************/
void CDevice::load3DM(
    SDL_RWops * pFile,
    const CMeshBinaryFileHeader & fileHeader,
    const std::string & group,
    const std::string & filePath,
    CModel & model )
{
    // temporary texture vector
    std::vector<CTexture> textureVec;
    textureVec.reserve( fileHeader.text_count );

    // Check to insure we are in the correct spot in the binary file
    tagCheck( pFile, filePath );

    // Load the textures into a temporary vector
    for( int i = 0; i < fileHeader.text_count; ++i )
    {
        // Get the texture and it's type
        CBinaryTexture btext;
        SDL_RWread( pFile, &btext, 1, sizeof( btext ) );

        // Load the texture
        CTexture texture = createTexture( group, btext.path );
        texture.m_type = ETextureType(btext.type);

        textureVec.emplace_back( texture );
    }

    // Load in the verts
    std::vector<CPoint<float>> vertLstVec( fileHeader.vert_count );
    tagCheck( pFile, filePath );
    SDL_RWread( pFile, vertLstVec.data(), vertLstVec.size(), sizeof( vertLstVec.back() ) );

    // Load in the normals
    std::vector<CPoint<float>> normalLstVec( fileHeader.vert_norm_count );
    tagCheck( pFile, filePath );
    SDL_RWread( pFile, normalLstVec.data(), normalLstVec.size(), sizeof( normalLstVec.back() ) );

    // Check to insure we are in the correct spot in the binary file
    std::vector<CUV> uvLstVec( fileHeader.uv_count );
    tagCheck( pFile, filePath );
    SDL_RWread( pFile, uvLstVec.data(), uvLstVec.size(), sizeof( uvLstVec.back() ) );

    // Reserve the number of vbo groups
    model.m_meshVec.reserve( fileHeader.face_group_count );

    // Read in each face group
    for( int i = 0; i < fileHeader.face_group_count; ++i )
    {
        // Check to insure we are in the correct spot in the binary file
        tagCheck( pFile, filePath );

        // Get the number faces in the group as well as the material index
        CBinaryFaceGroup faceGroup;
        SDL_RWread( pFile, &faceGroup, 1, sizeof( faceGroup ) );
        
        // Allocate the buffers for loading the index lists
        std::vector<uint16_t> textIndexVec( faceGroup.textureCount );
        std::vector<CBinaryVertex> vertIndexVec( faceGroup.vertexBufCount );
        std::vector<uint16_t> indexBufVec( faceGroup.indexBufCount );
        
        // Allocate a temporary buffer for building the VBO
        std::vector<NVertex::vert_uv_normal> vert( faceGroup.vertexBufCount );

        // Read in the indexes that are the textures
        SDL_RWread( pFile, textIndexVec.data(), textIndexVec.size(), sizeof( textIndexVec.back() ) );

        // Read in the indexes used to create the VBO
        SDL_RWread( pFile, vertIndexVec.data(), vertIndexVec.size(), sizeof( vertIndexVec.back() ) );

        // Read in the indexes that are the IBO
        SDL_RWread( pFile, indexBufVec.data(), indexBufVec.size(), sizeof( indexBufVec.back() ) );

        // Build the VBO
        for( size_t j = 0; j < vertIndexVec.size(); ++j )
        {
            vert[j].vert = vertLstVec[ vertIndexVec[j].vert ];
            vert[j].norm = normalLstVec[ vertIndexVec[j].norm ];
            vert[j].uv = uvLstVec[ vertIndexVec[j].uv ];
            
            // flip the Y for Vulkan coordinate system vs OpenGL
            vert[j].vert.invertY();
        }

        // Add a new entry into the vector
        model.m_meshVec.emplace_back();

        // Create the VBO
        model.m_meshVec.back().m_vboBuffer =
            creatMemoryBuffer( group, filePath + "_vbo_" + std::to_string(i), vert, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
        
        // Create the IBO
        model.m_meshVec.back().m_iboBuffer =
            creatMemoryBuffer( group, filePath + "_ibo_" + std::to_string(i), indexBufVec, VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
        
        // Save the number of indexes in the IBO buffer - Will need this for the render call
        model.m_meshVec.back().m_iboCount = faceGroup.indexBufCount;

        // Reserve texture space
        model.m_meshVec.back().m_textureVec.reserve(faceGroup.textureCount);

        // Copy over the texture handles
        for( int j = 0; j < faceGroup.textureCount; ++j )
        {
            int textIndex = textIndexVec[j];
            model.m_meshVec.back().m_textureVec.emplace_back( textureVec.at(textIndex) );
        }
    }
}


/************************************************************************
 *    DESC: Do the tag check to insure we are in the correct spot
 ************************************************************************/
void CDevice::tagCheck( SDL_RWops * file, const std::string & filePath )
{
    int tag_check;

    // Read in new tag
    SDL_RWread( file, &tag_check, 1, sizeof( tag_check ) );

    if( tag_check != TAG_CHECK )
        throw NExcept::CCriticalException( "Visual Mesh Load Error!",
            boost::str( boost::format( "Tag check mismatch (%s).\n\n%s\nLine: %s" )
                % filePath % __FUNCTION__ % __LINE__ ) );

}
