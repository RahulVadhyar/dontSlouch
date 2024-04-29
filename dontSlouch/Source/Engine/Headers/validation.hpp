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

/** \file validation.hpp

 \brief Contains the functions needed for initalization of vulkan validation layers.
*/

#pragma once
namespace Chronos {
namespace Engine {
#ifdef ENABLE_VULKAN_VALIDATION_LAYERS

    /**
    \brief Contains the name of the extensions needed to be enabled for validation layers.
    */
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };


    /**
    \brief Checks if the validation layers are supported by the system.
    */
    bool checkValidationLayerSupport();

    /**
    \brief This sets the debug Callback function

    In order to receive the debug messages, we need to register a callback function.
    This function is called when a debug message is generated.
    This function has many parameters, each one having many options. Please read the options carefully before using this function.

    @param messageSeverity: Severity of the message

            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes

    @param messageType: Type of the message

            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan

    @param pCallbackData: Struct containing details about the message itself
            pMessage: The debug message as a null-terminated string
            pObjects: Array of Vulkan object handles related to the message
            objectCount: Number of objects in array
            pMessageIdName: Unique identifier of the message (only for ```VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT```)
            messageIdNumber: Number representation of the message ID
            pQueueLabels: Array of ```VkDebugUtilsLabelEXT``` structs related to the objects in ```pObjects```
            queueLabelCount: Number of elements in ```pQueueLabels```
            pCmdBufLabels: Array of ```VkDebugUtilsLabelEXT``` structs related to the objects in ```pObjects```
            cmdBufLabelCount: Number of elements in ```pCmdBufLabels```

    @param pUserData: The ```pUserData``` value that was specified when calling ```vkCreateDebugUtilsMessengerEXT```

    @return ```VK_FALSE``` indicates that the Vulkan call that triggered validation was aborted and validation layers are not supported.
    */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
#pragma GCC diagnostic pop
    /**
    \brief Creates the debug messenger.
    */
    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger);

    /**
    \brief Destroys the debug messenger.

    OFC, we need to destroy the debug messenger when we are done with it.

    @param instance: The instance that has the debug messenger.
    @param debugMessenger: The debug messenger to be destroyed.
    @param pAllocator: The allocator to be used.
    */
    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator);

    /**
    \brief Populates the debug messenger create info.

    @param createInfo: The create info struct to be populated.
    */
    void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT& createInfo);
#endif
};
};
