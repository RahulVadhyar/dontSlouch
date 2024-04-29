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

/** \file Vertex.hpp

 \brief This file includes the structure of a vertex.

 This is used to create the vertices of a shape and define basic parameters(position, texture, color).
*/
#pragma once
namespace Chronos {
namespace Engine {

    /**
    \brief This defines the position  of a vertex. Used when we need a colored vertex. Does not support textures
    */
    struct ColorVertex {
        glm::vec2 pos;

        /**
        \brief Creates a ```VkVertexInputBindingDescription``` for the vertex based on the size of the vertex.
        */
        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(ColorVertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        /**
        \brief Generates the ```VkVertexInputAttributeDescription``` for the vertex based on pos attribute of the vertex.
        */
        static std::array<VkVertexInputAttributeDescription, 1>
        getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions {};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(ColorVertex, pos);
            return attributeDescriptions;
        }
    };

    /**
    \brief This defines the position and texture coordinates of a vertex. Used when we need a textured vertex.
    */
    struct TexturedVertex {
        glm::vec2 pos;
        glm::vec2 texCoord;

        /**
        \brief Creates a ```VkVertexInputBindingDescription``` for the vertex based on the size of the vertex.
        */
        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(TexturedVertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        /**
        \brief Generates the ```VkVertexInputAttributeDescription``` for the vertex based on attributes(pos, texCoord) of the vertex.
        */
        static std::array<VkVertexInputAttributeDescription, 2>
        getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions {};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(TexturedVertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(TexturedVertex, texCoord);

            return attributeDescriptions;
        }
    };
};
};
