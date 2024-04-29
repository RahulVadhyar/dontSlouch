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

/** \file vulkanHeaders.hpp

 \brief Contains the common headers for the vulkan API.

More specifically it includes the following packages:
- GLFW
- GLM
- Vulkan(included by GLFW)
*/
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


/**
\brief The number of frames in flight.

During rendering, we can have multiple frames in flight.
This ensures that the GPU and CPU are busy at the same time and not waiting for each other.
Hence we create multiple frames and while one is rendering, the other is being prepared.
This ensures maximum performance.
*/
#define MAX_FRAMES_IN_FLIGHT 2
#define SPIV_SHADER_PATH "Shaders/"
