/*
Copyright (c) 2024 Rahul Satish Vadhyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
/** \file helper.hpp

 \brief Contains various common functions used by other classes in the Engine namespace.

*/
namespace Chronos {
namespace Engine {

    /**
    \brief This is used to check if a given familty supports graphics and presentation.

    The check can be done using the ```isComplete()``` function.

    @param graphicsFamily The graphics family to check.
    @param presentFamily The presentation family to check.
    */
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    /**
    \brief THis contain the device extensions that are needed to be enabled in vulkan.

    */
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    /**
    \brief Begins recording of a command buffer that will be used once.

    This is used to create a temporary commandBuffer for single use and starts recording. Generally used for transfering data to the GPU.
    After using the buffer, it must be ended and destroyed using ```endSingleTimeCommands```.

    @param commandPool The command pool to create the temporary command buffer on.
    @param device The device that has the command pool.

    @return The created command buffer.
    */
    VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool,
        VkDevice device);

    /**
    \brief Ends recording of single time command buffer and destroys it.

    Ends recording of the command buffer and destroys the temporary ```commandBuffer``` created using ```beginSingleTimeCommands```.
    Generally used for transfering data.

    @param commandBuffer The command buffer created by ```beginSingleTimeCommands``` to destroy.
    @param device The device that has the command pool.
    @param commandPool The command pool that has the command buffer.
    */
    void endSingleTimeCommands(VkCommandBuffer* commandBuffer, Chronos::Engine::Device device,
        VkCommandPool commandPool);

    /**
    \brief Finds suitable memory type for given requrements.

    This is used to find a memory type on the physical device(GPU) that is suitable for the buffer that we are creating.
    It takes in the ```typeFilter``` and ```VkMemoryPropertyFlags``` to find such a memory type.

    @param typeFilter The filter to find the memory type.
    @param properties The properties of the memory type.
    @param physicalDevice The physical device to find the memory type on.

    @return The index of the memory type.
    */
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,
        VkPhysicalDevice physicalDevice);

    /**
    \brief Creates a buffer of a given size, usage and properties.

    This function creates a buffer of given size and usage.
    It then allocates the needed memory for this by finding the needed memory type using ```findMemoryType```.

    @param device The device that has the buffer.
    @param size The size of the buffer.
    @param usage The usage of the buffer.
    @param properties The memory property flags of the buffer.
    @param buffer The buffer to create.
    @param bufferMemory The associcated memory for the buffer.
    */
    void createBuffer(Chronos::Engine::Device device, VkDeviceSize size, VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties, VkBuffer* buffer,
        VkDeviceMemory* bufferMemory);

    /**
    \brief Copies data from one buffer to another.

    This is used to transfer data between host and device using staging buffers.

    @param device The device that has the commandPool.
    @param srcBuffer The source buffer to copy from.
    @param dstBuffer The destination buffer to copy to.
    @param size The size of the buffer to copy.
    @param commandPool The command pool to create the temporary command buffer on.
    */
    void copyBuffer(Chronos::Engine::Device device, VkBuffer srcBuffer, VkBuffer dstBuffer,
        VkDeviceSize size, VkCommandPool commandPool);

    /**
    \brief Gets the required extensions to be enabled

    The required extensions to be enabled include the GLFW extensions for graphics and presentation.
    Vulkan validation layers are also enabled.

    @return The required extensions to be enabled
    */
    std::vector<const char*> getRequiredExtensions();

    /**
    \brief Gets the indices of the needed queue families

    It finds the required graphics and presentation queue families on the physical device(GPU).
    It checks if the given physical device is capable of rendering to the surface that we provide it.

    @param device The physical device(GPU) to find the queue families on.
    @param surface The surface to which we are rendering.
    */
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
        VkSurfaceKHR surface);

    /**
    \brief Checks if the physical device that we provide it supports the extestions we need.
    This is used to check if the physical device that we provide it supports the extensions that we need.

    @param device The physical device to check.

    @return Whether the physical device supports the extensions that we need.
    */
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    /**
    \brief Checks if the physical device that we provide it is suitable for our needs.

    For a physical device to be suitable, it must support the extensions that we need,
    and it must be capable of rendering to the surface that we provide it along with rendering the frames.

    @param physicalDevice The physical device to check.
    @param surface The surface to which we are rendering.

    @return Whether the physical device is suitable for our needs.
    */
    bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    /**
    \brief Creates a command pool for a given device and surface.

    @param device The device to create the command pool on.
    @param surface The surface to which we are rendering.

    @return The created command pool.
    */
    VkCommandPool createCommandPool(Chronos::Engine::Device device, VkSurfaceKHR surface);

    /**
    \brief Creates a render pass.

    For a given swap chain, layouts, this creates a render pass.
    Please carefully read the parameters below before using this function to understand what each parameter does.

    During the creation of the render pass, the render pass will transition the image from the initial layout to the final layout.
    If MSAA is enabled, then the render pass will also transition the image from the final layout to the msaa final layout.
    The other parameters are also detailed below.

    @param initalLayout The initial layout of the incoming image. If this from the previous frame(no render pass before it) then this is ```VK_IMAGE_LAYOUT_UNDEFINED```,
    else it is advised to use ```VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL```.

    @param finalLayout The final layout of the outgoing image before MSAA. If this is going to be presented to the screen, then this is ```VK_IMAGE_LAYOUT_PRESENT_SRC_KHR```,
    else it is advised to use ```VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL```. If MSAA is enabled, then this is ```VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL```,
    as then msaaFinalLayout is used for the final layout if this is the final render pass.

    @param msaaFinalLayout The final layout of the outgoing image if MSAA is enabled.
    This is due to the fact that MSAA resolves the image, hence it change the layout of the image.
    If this is going to be presented to the screen, then this is ```VK_IMAGE_LAYOUT_PRESENT_SRC_KHR```,
    else it is advised to use ```VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL```.

    @param msaa Whether MSAA is enabled or not.

    @param clearFramebuffer Whether the framebuffer should be cleared or not. If this is the first render pass, then this should be true.
    Else it should be false, else the previous contents rendered to the framebuffer from the previous render pass will be cleared.

    @param dependency Whether the render pass should wait for other render passes to finish. If there are render passes before this, then this should be true.
    Else it should be false.
    */
    VkRenderPass createRenderPass(Chronos::Engine::Device device, Chronos::Engine::SwapChain swapChain,
        VkImageLayout initalLayout,
        VkImageLayout finalLayout,
        VkImageLayout msaaFinalLayout, bool msaa,
        bool clearFramebuffer, bool dependency);

    /**
    \brief Creates a set of framebuffers for use.

    It creates as many framebuffers as there are images in the swap chain.

    @param device The device that has the swap chain.
    @param swapChain The swap chain to create the framebuffers for.
    @param renderPass The render pass that the framebuffers are going to be used for.
    @param msaa Whether MSAA is enabled or not. This is to attach the necessary MSAA images to the framebuffer.
    */
    std::vector<VkFramebuffer> createFramebuffer(Chronos::Engine::Device device, Chronos::Engine::SwapChain swapChain,
        VkRenderPass renderPass,
        bool msaa);

    /**
    \brief Creates a set of command buffers for use.

    This creates the command buffers for a given swap chain on a given command pool.
    It creates as many command buffers as there are images in the swap chain.

    @param device The device that has the command pool.
    @param swapChain The swap chain to create the command buffers for.
    @param commandPool The command pool to create the command buffers on.

    @return The created command buffers.
    */
    std::vector<VkCommandBuffer> createCommandBuffer(Chronos::Engine::Device device,
        Chronos::Engine::SwapChain swapChain,
        VkCommandPool commandPool);

    /**
    \brief Returns the content of a shader file.

    This reads SPIV-V shader files and returns the contents of the file as a vector of chars.
    This is used to create shader modules.

    @param filename The filename of the shader file.

    @return The contents of the shader file as ```std::vector<char>```.
    */
    std::vector<char> readFile(const std::string& filename);

    /**
    \brief Creates a shader module from given shader(SPIV) code.

    In order to get ``std::vector<char>& code`` from a shader file, ``use readFile`` function.

    @param code The SPIV code to create the shader module from.
    @param device The device to create the shader module on.

    @return The created shader module.
    */
    VkShaderModule createShaderModule(const std::vector<char>& code,
        VkDevice device);

    /**
    \brief Gets the maxmimum supported MSAA sample count

    This gets the maximum supported MSAA sample count for the physical device that we provide it.

    @param physicalDevice The physical device to get the maxium supported MSAA sample count for.
    @return The maxium supported MSAA sample count.
    */
    VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);

};
};
