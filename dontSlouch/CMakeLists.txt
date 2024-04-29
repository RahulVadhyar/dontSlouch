cmake_minimum_required(VERSION 3.21.1)
message(${CMAKE_SOURCE_DIR}/libtorch-cxx11-abi-shared-with-deps-2.3.0+cu121/libtorch/share/cmake/Torch)
#set(Torch_DIR ${CMAKE_SOURCE_DIR}/libtorch-cxx11-abi-shared-with-deps-2.3.0+cu121/libtorch/share/cmake/Torch)
set(Torch_DIR ${CMAKE_SOURCE_DIR}/libtorch-cxx11-abi-shared-with-deps-2.3.0+cpu/libtorch/share/cmake/Torch)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

project(dontSlouchApp LANGUAGES CXX)

set(CUDA_PATH "/home/abdulamaan/miniconda3/envs/pytorch_stuff/lib/python3.10/site-packages/torch/share/cmake")

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Qml Quick)

if (Qt6_VERSION VERSION_GREATER_EQUAL 6.3)
    qt_standard_project_setup()
endif()

qt_add_executable(dontSlouchApp src/main.cpp)

qt_add_resources(dontSlouchApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(dontSlouchApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    "${TORCH_LIBRARIES}"
)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY}
    CACHE STRING "Import paths for Qt Creator's code model"
    FORCE
)

if (BUILD_QDS_COMPONENTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

if (LINK_INSIGHT)
    include(${CMAKE_CURRENT_SOURCE_DIR}/insight)
endif ()

include(GNUInstallDirs)
install(TARGETS dontSlouchApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# make IDEs aware of the QML import path
set(QML_IMPORT_PATH ${PROJECT_BINARY_DIR}/qml CACHE PATH
    "Path to the custom QML components defined by the project")
