cmake_minimum_required(VERSION 2.8)

project(Frangi_filter1)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})


add_executable(Frangi_filter1 Frangi_filter1.cxx)
target_link_libraries(Frangi_filter1  ${ITK_LIBRARIES})


