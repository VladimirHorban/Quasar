cmake_minimum_required (VERSION 3.8)

set(PROJECT_NAME "VulkanCore")

set(HEADERS Headers/Instance.h
		    Headers/Validation.h
			Headers/CommonHeaders.h
			Headers/PhysicalDevice.h
			Headers/LogicalDevice.h
			Headers/WindowSurface.h
			Headers/Window.h
			Headers/SwapChain.h
			Headers/Common.h
			Headers/Pipeline.h
			Headers/FrameBuffer.h
			Headers/CommandBuffer.h
			Headers/Synchronization.h
			)

set(SOURCES Sources/Instance.cpp
            Sources/Validation.cpp
			Sources/PhysicalDevice.cpp
			Sources/LogicalDevice.cpp
			Sources/WindowSurface.cpp
			Sources/Window.cpp
			Sources/SwapChain.cpp
			Sources/Pipeline.cpp	
			Sources/FrameBuffer.cpp
			Sources/CommandBuffer.cpp
			Sources/Synchronization.cpp
			)

project(${PROJECT_NAME} CXX)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

# Add Vulkan to project
target_include_directories(${PROJECT_NAME} PRIVATE include ../ThirdParty/vulkan)

find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS "../ThirdParty/vulkan/libs/windows/x64")

target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARY})

# Add SPIRV-Tools static lib

target_link_libraries(${PROJECT_NAME} SPIRV-TOOLS)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)