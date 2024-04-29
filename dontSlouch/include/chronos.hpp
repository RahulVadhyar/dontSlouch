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
/** \file chronos.hpp

 \brief Main API for chronos. Any applications should include this file.
*/
#include "vulkanHeaders.hpp"
#include "stlheader.hpp"
#include "logging.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "validation.hpp"

#include "Vertex.hpp"
#include "helper.hpp"
#include "buffers.hpp"
#include "texture.hpp"
#include "engine.hpp"

namespace Chronos {
namespace Manager {
    /**
    \brief This struct contains the details for initialing the manager.

    @param WindowWidth The initial window width. This can be later resized by the user.
    @param WindowHeight The initial window height. This can be later resized by the user.
    @param BackgroundColor The background color of the window.(range 0-255) by default it is black(0, 0, 0)
    */
    struct Initializer {
        int WindowWidth = 800;
        int WindowHeight = 600;
        int BackgroundColor[3] = { 0, 0, 0 };
#ifdef ENABLE_EDITOR
        //function pointer for calling the function that will render the editor elements on the screen
        void (*editorAddElements)();
#endif
    };

    /**
    \brief The main manager of chronos.

    Only this class must be used for interfacing with chronos. Do not use the internal classes for this
    Any changes to the window can be done via this object. Please use only the functions and variables in this class.
    For more information, refer to the individual methods and variables and the examples
    */
    class Manager {
    public:
        /**
        \brief Constructor for chronos. The initalizer struct with the necessary options to be passed.

        For options, refer to the Initializer struct
        */
        Manager(Initializer initializer);
        /**
        \brief Gets a reference to the GLFWWindow

        Since Chronos initialzes the window, along with managing it, the window is with chronos.

        However for input handling and advanced window management, chronos will not do this.
        Instead you are expected to directly call the GLFW functions and define the options yourself.

        @return The GLFWWindow instance created and used by Chronos
        */
        GLFWwindow* getWindow();

        /**
        \brief Draws the current frame to the window.

        This must called every frame during the game loop.

        This function updates all the attributes of the objects defined by the user and submitted to chronos.
        It then submits sthe necessary information to the GPU to render the frame.
        */
        void drawFrame();

        /**
        \brief Changes the background color of the window.

        If changing the background color of the window is desired, then by passing the RGB values to this function, it may be accomplished.

        @param r Red value
        @param g Green value
        @param b Blue value
        */
        int changeBackgroundColor(float r, float g, float b);

        /**
        \brief Changes the present mode of the swapchain.

        The present mode is the mode in which the images are presented to the screen.
        There are various methods available. this can be used to cap or unlock the framerate, control tearing etc.
        Some of the presentation modes available are:
        - immediate: Images submitted by your application are transferred to the screen right away, which may result in tearing.
        - fifo : The swap chain is a queue where the display takes an image from the front of the queue when the display is refreshed and the program inserts rendered images at the back of the queue.If the queue is full then the program has to wait. This is most similar to vertical sync as found in modern games.The moment that the display is refreshed is known as "vertical blank".
        - fifo_relaxed : This mode only differs from the previous one if the application is late and the queue was empty at the last vertical
        blank. Instead of waiting for the next vertical blank, the image is transferred right away when it 	finally arrives.This may result in visible tearing.
        - mailbox : This is another variation of the
        second mode.Instead of blocking the application when the queue is full, the
        images 	that are already queued are simply replaced with the newer ones.This
        mode can be used to render frames as fast as possible while still avoiding
        tearing, resulting in fewer latency issues than standard vertical sync.This
        is commonly known as "triple buffering", although the existence of three buffers alone does not necessarily mean that the framerate
        
        @param mode The mode to change to. It can be one of the following:
        - immediate
        - fifo
        - fifo_relaxed
        - mailbox
        */
        void changePresentMode(std::string presentMode){
            engine.setPresentMode(presentMode);
        }

        std::vector<std::string> getMSAAModes(){
            return engine.getAvailableMSAAModes();
        }

        void changeMSAA(std::string mode){
            engine.changeMSAA(mode);
        }

    private:
        /**
        \brief The backend Vulkan engine, used for rendering the desired shapes and text.s
        */
        Chronos::Engine::Engine engine;
    };
};
};
