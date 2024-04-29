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

#include "stlheader.hpp"
#include "editorHeaders.hpp"

#include "vulkanHeaders.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "helper.hpp"
#include "editorRenderer.hpp"
#include "logging.hpp"

void Chronos::Editor::EditorRenderer::init(Chronos::Engine::Device* device, GLFWwindow* window, Chronos::Engine::SwapChain* swapChain,
    VkInstance instance, VkSurfaceKHR surface)
{
    this->device = device;
    this->swapChain = swapChain;
    this->surface = surface;
    this->window = window;

    renderPass = Chronos::Engine::createRenderPass(
        *device, *swapChain, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        false, true, true);
    framebuffers = Chronos::Engine::createFramebuffer(*device, *swapChain, renderPass, false);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#ifdef WIN32
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
    ImGui::StyleColorsDark();

    std::array<VkDescriptorPoolSize, 1> poolSizes {};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolSize pool_sizes[] = {
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100}
    };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 100;
    pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;
    vkCreateDescriptorPool(device->device, &pool_info, nullptr, &descriptorPool);

    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance;
    init_info.PhysicalDevice = device->physicalDevice;
    init_info.Device = device->device;
    init_info.QueueFamily = Chronos::Engine::findQueueFamilies(device->physicalDevice, surface).graphicsFamily.value();
    init_info.Queue = device->graphicsQueue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descriptorPool;
    init_info.RenderPass = renderPass;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.CheckVkResultFn = nullptr; // add a fucntion to this
    ImGui_ImplVulkan_Init(&init_info);

    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex = Chronos::Engine::findQueueFamilies(device->physicalDevice, surface).graphicsFamily.value();
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device->device, &commandPoolCreateInfo, nullptr,
            &commandPool)
        != VK_SUCCESS) {
        LOG(1, "EditorRenderer", "Could not create graphics command pool")
        throw std::runtime_error("Could not create graphics command pool");
    }

    VkCommandBuffer command_buffer = Chronos::Engine::beginSingleTimeCommands(commandPool, device->device);
    ImGui_ImplVulkan_CreateFontsTexture();
    Chronos::Engine::endSingleTimeCommands(&command_buffer, *device, commandPool);

    commandBuffers.resize(swapChain->swapChainImageViews.size());
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    if (vkAllocateCommandBuffers(device->device, &allocInfo,
            commandBuffers.data())
        != VK_SUCCESS) {
        LOG(1, "EditorRenderer", "failed to allocate command buffers!")
        throw std::runtime_error("failed to allocate command buffers!");
    }
    LOG(3, "EditorRenderer", "EditorRenderer initialized")
}

void Chronos::Editor::EditorRenderer::destroy()
{
    ImGui_ImplVulkan_Shutdown();
    vkDestroyRenderPass(device->device, renderPass, nullptr);
    for (auto framebuffer : framebuffers)
        vkDestroyFramebuffer(device->device, framebuffer, nullptr);
    vkDestroyCommandPool(device->device, commandPool, nullptr);
    vkDestroyDescriptorPool(device->device, descriptorPool, nullptr);
    LOG(3, "EditorRenderer", "EditorRenderer destroyed")
}

void Chronos::Editor::EditorRenderer::update()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    this->addElements();
    ImGui::Render();
    LOG(4, "EditorRenderer", "EditorRenderer updated")
}

void Chronos::Editor::EditorRenderer::render(uint32_t currentFrame, uint32_t imageIndex, float bgColor[3])
{
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    VkClearValue clearColor = {{{ bgColor[0], bgColor[1], bgColor[2], 1.0f }}};
    vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo);
    VkRenderPassBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = renderPass;
    info.framebuffer = framebuffers[imageIndex];
    info.renderArea.extent = swapChain->swapChainExtent;
    info.clearValueCount = 1;
    info.pClearValues = &clearColor;
    vkCmdBeginRenderPass(commandBuffers[currentFrame], &info,
        VK_SUBPASS_CONTENTS_INLINE);
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
        commandBuffers[currentFrame]);
    vkCmdEndRenderPass(commandBuffers[currentFrame]);
    vkEndCommandBuffer(commandBuffers[currentFrame]);
    LOG(4, "EditorRenderer", "EditorRenderer rendered")
}

void Chronos::Editor::EditorRenderer::cleanup()
{
    for (auto framebuffer : framebuffers)
        vkDestroyFramebuffer(device->device, framebuffer, nullptr);
    LOG(3, "EditorRenderer", "EditorRenderer cleaned up")
}

void Chronos::Editor::EditorRenderer::recreate()
{
    cleanup();
    framebuffers = Chronos::Engine::createFramebuffer(*device, *swapChain, renderPass, false);
    LOG(3, "EditorRenderer", "EditorRenderer recreated")
}

void Chronos::Editor::EditorRenderer::renderAdditionalViewports(){
#ifdef WIN32
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    LOG(4, "EditorRenderer", "Rendered additional viewports")
#endif
}

void Chronos::Editor::EditorRenderer::changeMsaa(){
    vkDestroyRenderPass(device->device, renderPass, nullptr);
    renderPass = Chronos::Engine::createRenderPass(
        *device, *swapChain, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        false, false, true);
    recreate();
    
}
