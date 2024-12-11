cmake_minimum_required(VERSION 3.10)

project(Julia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra")

set(SOURCE_FILES
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/shader.cpp
    src/glad.c
)

add_executable(main src/main.cpp ${SOURCE_FILES})

#target_compile_options(main PRIVATE
#    -std=c++17        
#    -g                
#    -Wall              
#    -Wextra           
#)

add_executable(animation src/animation.cpp ${SOURCE_FILES})

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(OpenGL REQUIRED)

find_package(glfw3 REQUIRED)

find_package(Threads REQUIRED)

find_package(X11 REQUIRED)

target_link_libraries(main PRIVATE
    glfw
    OpenGL::GL
    Threads::Threads
    X11
    dl
    m 
)

target_link_libraries(animation PRIVATE
    glfw
    OpenGL::GL
    Threads::Threads
    X11
    dl
    m 
)

