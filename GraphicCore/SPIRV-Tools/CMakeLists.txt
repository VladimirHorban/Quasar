cmake_minimum_required (VERSION 3.8)

set(PROJECT_NAME "SPIRV-TOOLS")

set(HEADERS Headers/ShaderManager.h)

set(SOURCES Sources/ShaderManager.cpp)

project(${PROJECT_NAME} CXX)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

# Add Vulkan to project
target_include_directories(${PROJECT_NAME} PRIVATE include ../ThirdParty/vulkan)

find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS "../ThirdParty/vulkan/libs/windows/x64")

target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARY})



