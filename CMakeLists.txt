cmake_minimum_required(VERSION 3.5)

project(Sandbox VERSION 1.0
                DESCRIPTION "A sandbox application using the Ambient game engine"
                LANGUAGES CXX)

add_executable(app src/app.cpp)
target_include_directories(app PUBLIC ../Engine/include/)
target_link_directories(app PUBLIC ../Engine/build/)
target_link_libraries(app PUBLIC ambient)
