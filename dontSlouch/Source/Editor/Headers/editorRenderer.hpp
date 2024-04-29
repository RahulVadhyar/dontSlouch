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
namespace Chronos {
namespace Editor {

    class EditorRenderer {
    public:
        Chronos::Engine::Device* device;
        Chronos::Engine::SwapChain* swapChain;
        VkSurfaceKHR surface;
        GLFWwindow* window;

        VkCommandPool commandPool;
        VkDescriptorPool descriptorPool;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<VkFramebuffer> framebuffers;
        VkRenderPass renderPass;

        void init(Chronos::Engine::Device* device, GLFWwindow* window, Chronos::Engine::SwapChain* swapChain,
            VkInstance instance, VkSurfaceKHR surface);
        void render(uint32_t currentFrame, uint32_t imageIndex, float bgColor[3]);
        void destroy();
        void update(); // need to reorganize thnis
        void recreate(); // destroy framebuffer
        void changeMsaa(); // destroy renderpass
        void cleanup();
        void renderAdditionalViewports();
        void (*addElements)();
    };
}; // namespace Editor
}; // namespace Chronos
