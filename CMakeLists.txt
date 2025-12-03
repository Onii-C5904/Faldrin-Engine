cmake_minimum_required(VERSION 3.31)
project(Faldrin_Engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 20)



add_executable(Faldrin_Engine.exe
        src/main.cpp
        src/helper.h
        src/helper.cpp
        src/engine/Player.cpp
        src/engine/Player.h
        src/engine/Engine.cpp
        src/rendering/Sprite.cpp
        src/rendering/Sprite.h
        src/engine/temp.h
        src/engine/Physics.h
        src/engine/Physics.cpp
)


target_include_directories(Faldrin_Engine.exe PUBLIC "/usr/include")
target_link_directories(Faldrin_Engine.exe PUBLIC "/usr/lib")
target_include_directories(Faldrin_Engine.exe PUBLIC "/usr/local/include")
target_link_directories(Faldrin_Engine.exe PUBLIC "/usr/local/lib")

target_link_libraries(Faldrin_Engine.exe PUBLIC raylib)
target_link_libraries(Faldrin_Engine.exe PUBLIC tmxlite)

