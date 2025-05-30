
cmake_minimum_required(VERSION 3.10)
project(Xiangqi)
set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system network REQUIRED)

# make sure the include/ root is on your include path
include_directories(${CMAKE_SOURCE_DIR}/include)

# recurse into subfolders of src
file(GLOB_RECURSE SRC_FILES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(Xiangqi ${SRC_FILES})
target_link_libraries(Xiangqi
  sfml-graphics sfml-window sfml-system sfml-network
)