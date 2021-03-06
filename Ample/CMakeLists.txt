cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17 -fPIC")

set(OPENGL_gl_LIBRARY /usr/lib/x86_64-linux-gnu/libGL.so)
set(OPENGL_INCLUDE_DIRS /usr/include/GL/)

set(SDL2_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2.so)
set(SDL2_INCLUDE_DIRS /usr/include/SDL2/)

set(GLU_LIBRARY /usr/lib/x86_64-linux-gnu/libGLU.so)

set(${CMAKE_PROJECT_NAME}_SOURCES

    src/Basic/Activity.cpp

    src/Control/EventManager.cpp
    src/Control/EventHandler.cpp
    
    src/Os/WindowActivity.cpp
    src/Os/Clock.cpp
    src/Os/Exception.cpp
    src/Os/Window.cpp

    src/Utils/Utils.cpp

    src/Graphics/GraphicalObject.cpp
    src/Graphics/GraphicalObject2d.cpp
    src/Graphics/ScreenObject.cpp
    src/Graphics/Vector2d.cpp
    src/Graphics/Camera.cpp
    src/Graphics/CameraPerspective.cpp
    src/Graphics/CameraOrtho.cpp
    )

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

include_directories(include)

set(SOURCE_LIB ${${CMAKE_PROJECT_NAME}_SOURCES})

add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCE_LIB})

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLU_LIBRARY})
