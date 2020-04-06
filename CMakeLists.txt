cmake_minimum_required(VERSION 3.15)
project(NeuralNetwork)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(NeuralNetwork
        header.hpp
        json.hpp
        nn.hpp
        Matrix.hpp
        main.cpp)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window network graphics audio)
if (SFML_FOUND)
    include_directories(NeuralNetwork)
    target_link_libraries(NeuralNetwork sfml-system sfml-window sfml-network sfml-graphics sfml-audio)
endif()
