cmake_minimum_required(VERSION 3.31)
project(FloorPlan)

file(GLOB_RECURSE source_files
    "src/*.cpp"
)


add_executable(${PROJECT_NAME} ${source_files})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "FindSFML.cmake")

find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)

target_include_directories(${PROJECT_NAME} PUBLIC include)

set(SFML_LIBS sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})

# Copy res dir to the binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
