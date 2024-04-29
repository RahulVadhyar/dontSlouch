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
/** \file engineStructs.hpp

 \brief Contains ShapeParams and UniformBufferObject structs.
*/
namespace Chronos {
namespace Engine {
    /**
    \brief Parameters for rendering text

    This is the struct for the parameters of a ```Font```.
    This is needed to create and modify a ```Font```.

    ## Note:
    In Vulkan y axis is inverted. So the y axis of the shape is inverted.
    This means that -1 is the top of the screen and 1 is the bottom of the screen.

    @param x x coordinate of the text(Range : 0 and 1).
    @param y y coordinate of the text(Range : 0 and 1).
    @param rotation Rotation of the text in degrees(Range: any float).
    @param scale Scale of the text(Range : any float).
    @param text The text to render.
    @param color The color of the text(Range : 0 and 1).
    */
    struct TextParams {
        std::string text;
        float x = 0;
        float y = 0;
        float rotation = 0;
        float scale = 1.0f;
        std::array<float, 3> color = {1.0f, 1.0f, 1.0f};
    };
    /**
    \brief Uniform struct passed to shader

    This is the uniform of the shape. This is passed to the shaders for generating the vertices in the correct positons.
    */
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    /**
    \brief Uniform struct for color passed to shader

    This is used for passing the color to the shaders. Used with colored shapes.
    */
    struct UniformColorBufferObject {
        glm::vec3 color;
    };
};
};
