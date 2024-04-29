cmake_minimum_required(VERSION 3.13)
project(Chronos VERSION 0.0.0)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O3 -march=native")

set(GLFW_BUILD_WAYLAND ON)

set(X11_X11_LIB who_cares)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_VULKAN_STATIC ON)

#GLFW
add_subdirectory(ThirdParty/glfw)
set(GLFW_DIR ThirdParty/glfw/include)

#source files
set(SOURCE_DIR Source/Engine/Source)
set(API_DIR Source/API/Source)
file(GLOB MAIN ${SOURCE_DIR}/*.cpp)
file(GLOB API ${API_DIR}/*.cpp)

#get all the cpp files
file(GLOB CHRONOS_CPP ${MAIN} ${API} )

#create a fucntion to include all the directories, also link vulkan and glfw
function(IncludeDirectories projectName)
    target_include_directories(${projectName} PUBLIC Source/Engine/Headers)
    target_include_directories(${projectName} PUBLIC Source/Engine/Templates)
    target_include_directories(${projectName} PUBLIC Source/API/Headers)
    target_include_directories(${projectName} PUBLIC Source/Common/Headers)
    target_include_directories(${projectName} PUBLIC ThirdParty/stb)
    target_include_directories(${projectName} PUBLIC include)
    target_include_directories(${projectName} PUBLIC ${GLFW_DIR})  
    target_link_libraries(${projectName} vulkan)   
    target_link_libraries(${projectName} glfw)

endfunction()

#editor executable
set(EDITOR_DIR Source/Editor/Source)
set(IMGUI_DIR ThirdParty/imgui) 
set(IMGUI_BACKENDS ${IMGUI_DIR}/backends)

file(GLOB EDITOR ${EDITOR_DIR}/*.cpp)
file(GLOB IMGUI ${IMGUI_DIR}/*.cpp)
file(GLOB EDITOR_FILES 
${EDITOR} 
${IMGUI} 
${IMGUI_BACKENDS}/imgui_impl_glfw.cpp 
${IMGUI_BACKENDS}/imgui_impl_vulkan.cpp
)

# linking
add_executable(ChronosEditor ${CHRONOS_CPP} ${EDITOR_FILES})

#vulkan sdk if using windows
if(WIN32)
    target_include_directories(ChronosEditor PUBLIC ${VULKAN_SDK_INCLUDE})     
    target_link_directories(ChronosEditor PUBLIC ${VULKAN_SDK_LIB})
    message("Vulkan SDK include found: ${VULKAN_SDK_INCLUDE}")
endif(WIN32) 

target_compile_definitions(ChronosEditor PUBLIC ENABLE_EDITOR)
target_compile_definitions(ChronosEditor PUBLIC ENABLE_VULKAN_VALIDATION_LAYERS)

target_compile_definitions(ChronosEditor PUBLIC GAME_NAME="dontSlouch")
target_compile_definitions(ChronosEditor PUBLIC GAME_VERSION_MAJOR=0)
target_compile_definitions(ChronosEditor PUBLIC GAME_VERSION_MINOR=0)
target_compile_definitions(ChronosEditor PUBLIC GAME_VERSION_PATCH=0)
target_compile_definitions(ChronosEditor PUBLIC CHRONOS_VERSION_MAJOR=0)
target_compile_definitions(ChronosEditor PUBLIC CHRONOS_VERSION_MINOR=0)
target_compile_definitions(ChronosEditor PUBLIC CHRONOS_VERSION_PATCH=0)

#header files
target_include_directories(ChronosEditor PUBLIC Source/Editor/Headers)
target_include_directories(ChronosEditor PUBLIC ${IMGUI_DIR})               
target_include_directories(ChronosEditor PUBLIC ${IMGUI_BACKENDS})
IncludeDirectories(ChronosEditor)