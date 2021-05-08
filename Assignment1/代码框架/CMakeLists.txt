cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

message("cmake_module_path: " ${CMAKE_MODULE_PATH})
find_package(OpenCV REQUIRED)


message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/Cellar/eigen/3.3.9/include)
include_directories(/usr/local/include)

add_executable(Rasterizer main.cpp rasterizer.hpp rasterizer.cpp Triangle.hpp Triangle.cpp)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
