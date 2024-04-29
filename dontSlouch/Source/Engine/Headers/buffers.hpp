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
/** \file buffers.hpp

 \brief Contains the [Buffer](#Chronos::Engine::Buffer) and [UniformBuffer](#Chronos::Engine::UniformBuffer) class.
*/
namespace Chronos {
namespace Engine {

    /**
     \brief Used to store buffers in GPU.

    It contains the buffer, the size and the memory variables needed for this,
    along with the appropriate functions to use them.
    */
    class Buffer {
    public:
        /**
        \brief This is used to initialize the buffer.

        It is calls the [createBuffer](#Chronos::Engine::createBuffer) function for this purpose
        and stores the results.

        @param device The device on which the buffer should be stored.
        @param flags The [flags](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkBufferUsageFlagBits.html) to use for the buffer.
        @params properties The [memory properties](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkMemoryPropertyFlagBits.html) to use for the buffer.
        */
        void create(Chronos::Engine::Device device, VkBufferUsageFlags flags,
            VkMemoryPropertyFlags properties);

        /**
        \brief Copies the data to the buffer.

        It does this by coping the data into a temporary staging buffer and then copying it to the final buffer.

        @param data The data to copy.
        @param commandPool The command pool to use for the staging buffer and commands during the copy.
        */
        void copy(void* data, VkCommandPool commandPool);

        /**
        \brief Destroys the buffer and frees the memory.
        */
        void destroy();

        /**
        \brief The buffer.
        */
        VkBuffer buffer;

        /**
        \brief The size of the buffer.
        */
        VkDeviceSize size;

        /**
        \brief The device memory of the buffer.
        */
        VkDeviceMemory memory;

    private:
        /**
        \brief The device on which the buffer is stored.
        */
        Chronos::Engine::Device device;
    };

    /**
        \brief This is a Uniform buffer for storing the uniform variables in the shaders

    Currently used only for shapes properties like postition, size etc.
    Can be updated on the fly using the update function.
    */
    class UniformBuffer : public Buffer {
    public:
        /**
        \brief Creates the buffer and maps the memory.

        It uses the inherited function [create](#Chronos::Engine::Buffer::create) to create the buffer

        @param device The device on which the buffer should be stored.
        */
        void create(Chronos::Engine::Device device);

        /**
        \brief Updates the buffer with the given information.

        It creates a [UniformBufferObject](#Chronos::Engine::UniformBufferObject).
        The entires in this struct are calculated using glm functions.
        We need the swapChainExtent to calculate the projection matrix so that
        the scale is correct even when resizing windows.

        @param swapChainExtent The extent of the swap chain.
        @param x The x coordinate of the shape.
        @param y The y coordinate of the shape.
        @param rotation The rotation of the shape.
        @param x_size The x size of the shape.
        @param y_size The y size of the shape.
        */

        void update(VkExtent2D swapChainExtent, float x, float y, float rotation,
            float x_size, float y_size);

    private:
        /**
        \brief Temporary storage used to store the data before copying to the buffer.
        */
        void* data;
    };

    /**
    \brief This is a buffer for storing and updating the color of the shape.

    It is used to store the color of the shape and update it on the fly.
    */
    class ColorBuffer : public Buffer {
    public:
        /**
        \brief Creates the buffer and maps the memory.

        It uses the inherited function [create](#Chronos::Engine::Buffer::create) to create the buffer

        @param device The device on which the buffer should be stored.
        */
        void create(Chronos::Engine::Device device);
        
        /**
        \brief Updates the buffer with the new color.

        @param color The new color of the shape.
        */
        void update(glm::vec3 color);

    private:
        void* data;
    };
}; // namespace Engine
}; // namespace Chronos
