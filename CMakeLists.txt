cmake_minimum_required(VERSION 3.10)
project(F1CarDesigner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/imgui ${CMAKE_SOURCE_DIR}/imgui/backends ${CMAKE_SOURCE_DIR}/src)

set(IMGUI_SRC
    ${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

set(SOURCES
    src/main.cpp
    src/CarDesign.cpp
    src/ConfigurationManager.cpp
)

add_executable(F1CarDesigner ${SOURCES} ${IMGUI_SRC})

target_compile_definitions(F1CarDesigner PRIVATE IMGUI_ENABLE_DOCKING)

target_link_libraries(F1CarDesigner glfw ${OPENGL_LIBRARIES})

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/designs)
execute_process(COMMAND chmod 755 ${CMAKE_SOURCE_DIR}/designs)
