cmake_minimum_required(VERSION 3.31)
project(FloorPlanner VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "FindSFML.cmake")
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
set(SFML_LIBS sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

include_directories(include)
add_library(src SHARED src/Wall.cpp src/Structure.cpp src/Inputs.cpp)
target_link_libraries(src ${SFML_LIBS})


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} src)


target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})
# Copy res dir to the binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
