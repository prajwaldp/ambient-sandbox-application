cmake_minimum_required(VERSION 3.5)

project(Sandbox VERSION 1.0
        DESCRIPTION "A sandbox application using the Ambient game engine"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

include(../Engine/build/conanbuildinfo.cmake)  # Includes the contents of the conanbuildinfo.cmake file.

# https://docs.conan.io/en/latest/howtos/manage_shared_libraries/rpaths.html
conan_basic_setup(KEEP_RPATHS)  # This is a macro defined in the above file

add_executable(app
        App.cpp
        demos/PlatformGame2D/PlatformGame2D.cpp
        demos/SimpleSquareAndTriangle.cpp
        demos/RotatingQuad.cpp
        demos/RPGWorld.cpp demos/ParticleSystem.cpp demos/ParticleSystem.h demos/PlatformGame2D/LevelManager.cpp demos/PlatformGame2D/LevelManager.h demos/PlatformGame2D/Player.cpp demos/PlatformGame2D/Player.h)

target_include_directories(app PUBLIC ../Engine/include/ ./demos/)
target_link_directories(app PUBLIC ../Engine/build/lib)
target_link_libraries(app ambient ${CONAN_LIBS})
