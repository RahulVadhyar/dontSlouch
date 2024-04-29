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

/** \file swapchain.hpp

 \brief Contains the swapChain class along with all the swapChain related functions.
*/
#pragma once
namespace Chronos {
namespace Engine {

    /**
    \brief Contains the fields to the support details of a physical device.
    */
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    /**
    \brief Gets the maximum extent of the swapchain images(framebuffer size).

    We need to calculate the maximum extent of the swapchain. Preferably we want to support the width and height of the window. However the hardware may not support this. Hence after getting the desired height and and width, we clamp it to the hardware limits.

    @param capabilites The capabailites of the hardware/surface to display the image(max/min height and width)

    @param window The GLFW window instance to be rendered to. This contains the desired height and width.

    @return The chosen extent of the swapchain based on the capabilites of the hardware.
    */
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
        GLFWwindow* window);

    /**
    \brief Chooses the best present mode among the supported modes.

    It chooses RGBA format along with SRGB for color space. If such a format is not available then it chooses the first one that is available.

    @param availableFormats The available formats available to choose from.

    @return The best format chosen for rendering
    */
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);

    /**
    \brief For a given swapchain mode, it gets the capabilites, formats and present modes.

    For a given device and surface, there are multiple available swapchain modes available for use. This gets those details for further use.

    @param device The physical device(GPU) on which we are rendering.

    @param surface The rendering surface(basically a vulkan window.)

    @return The support details of the swapchain.
    */
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
        VkSurfaceKHR surface);

    /**
    /brief Responsible for initializing, managing, recreating and presenting the
    swapchain images.

    In Vulkan, we render to images(texture). These images are then present to the GLFW window.
    Since there will be multiple images in flight, along with multiple other assets for
    this, we need to manage these assets.
    */
    class SwapChain {
    public:
        /**
        \brief Initilize the swapchain

        The swapchain is an insanely complicated process to setup.
        We need to create multiple textures, along with their memory and
        views. We also need to select the format to render to. Along with that we need the color rendering texture attachment, which has its own texture, memory and view.

        @param device The device to create the swapchain on
        @param surface The surface(VK version of window texture) to render to
        @param window Thw window to display the image on.
        */
        void init(Chronos::Engine::Device* device, VkSurfaceKHR surface, GLFWwindow* window);

        /**
        \brief When the swapchain is rendered invalid, recreate it.

        Due to various user activities such as window resizing and minimization, the swapchain can become invalid.
        At that point, we need to recreate the assets based on the new dimensions.
        */
        void recreate();

        /**
        \brief Cleans up the assets that are rendered invalid during MSAA changes or swapchain.
        */
        void cleanup();

        /**
        \brief Changes the MSAA count of the swapchain images

        When the user decides to change the MSAA, we need to recreate the swapchain. This does it.
        */
        void changeMsaa();

        /**
        \brief Dimenisons of the current swapchain
        */
        VkExtent2D swapChainExtent;

        /**
        \brief Vulkan swapchain object
        */
        VkSwapchainKHR swapChain;

        /**
        \brief Surface to which we need to present the swapchain images
        */
        VkSurfaceKHR surface;

        /**
        \brief Image views of the swapchain textures(images, aka window)
        */
        std::vector<VkImageView> swapChainImageViews;

        /**
        \brief image view of the color attachement
        */
        VkImageView colorImageView;

        /**
        \brief Chosen swapchain image format
        */
        VkFormat swapChainImageFormat;


        /**
        \brief Preferred present mode for the swapchain
        */
        VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;

        bool changePresentMode = false;

    private:
        /**
        \brief Device to which swapchain needs to be created
        */
        Chronos::Engine::Device* device;

        /**
        \brief The window to present the surface to.
        */
        GLFWwindow* window;

        /**
        \brief The color image
        */
        VkImage colorImage;

        /**
        \brief Memory to store the color image
        */
        VkDeviceMemory colorImageMemory;

        /**
        \brief Swapchaim images to render and present to.
        */
        std::vector<VkImage> swapChainImages;

        /**
        \brief Creates the swapchain image resources and chooses the optimal settings for given hardware
        */
        void create();

        /**
        \brief Creates the image views of the swapchain images
        */
        void createImageViews();

        /**
        \brief Creates the color images along with the associated objects
        */
        void createColorResources();

        /**
        \brief From the available present mode, this chooses the the best present mode.

        If available ```VK_PRESENT_MODE_MAILBOX_KHR``` will be chosen.

        Some of the presentation modes available are:
        - ```VK_PRESENT_MODE_IMMEDIATE_KHR```: Images submitted by your application are transferred to the screen right away, which may result in tearing.
        - ```VK_PRESENT_MODE_FIFO_KHR``` : The swap chain is a queue where the display takes an image from the front of the queue when the display is refreshed and the program inserts rendered images at the back of the queue.If the queue is full then the program has to wait. This is most similar to vertical sync as found in modern games.The moment that the display is refreshed is known as "vertical blank".
        - ```VK_PRESENT_MODE_FIFO_RELAXED_KHR``` : This mode only differs from the previous one if the application is late and the queue was empty at the last vertical
        blank. Instead of waiting for the next vertical blank, the image is transferred right away when it 	finally arrives.This may result in visible tearing.
        - ```VK_PRESENT_MODE_MAILBOX_KHR``` : This is another variation of the
        second mode.Instead of blocking the application when the queue is full, the
        images 	that are already queued are simply replaced with the newer ones.This
        mode can be used to render frames as fast as possible while still avoiding
        tearing, resulting in fewer latency issues than standard vertical sync.This
        is commonly known as "triple buffering", although the existence of three buffers alone does not necessarily mean that the framerate

        @param availablePresentModes The available present modes to choose from.

        @return The best present mode based on availability and preference.

        */
        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);
    };
};
};
