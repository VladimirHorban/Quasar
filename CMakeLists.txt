# CMakeList.txt : CMake project for Quasar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Quasar")

add_subdirectory(GraphicCore)

add_subdirectory(API)

set(HEADERS API/Headers/CoreMinimal.h)
set(SOURCES API/Sources/CoreMinimal.cpp)

# Add source to this project's executable.
#add_executable (Quasar "Quasar.cpp" "Quasar.h" ${HEADERS} ${SOURCES})
add_library(Quasar STATIC ${HEADERS} ${SOURCES})

target_link_libraries(Quasar VulkanCore)

set_property(TARGET Quasar PROPERTY CXX_STANDARD 17)