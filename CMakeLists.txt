# CMakeList.txt : CMake project for Quasar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Quasar")

add_subdirectory(GraphicCore)

# Optional (only for show)
add_subdirectory(Example)

# Add source to this project's executable.
add_executable (Quasar "Quasar.cpp" "Quasar.h" )

target_link_libraries(Quasar VulkanCore)

set_property(TARGET Quasar PROPERTY CXX_STANDARD 17)