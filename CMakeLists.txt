project(ProtoDAQ)

set(CMAKE_CXX_STANDARD 14)

# TODO (Khalid): If user does not have glfw3_FOUND, then install it for them
find_package(glfw3 CONFIG REQUIRED) # sudo apt-get install libglfw3 libglfw3-dev

# By default, linux should already has this
find_package(SDL2 REQUIRED)

# TODO (Khalid): Help the user install by checking if it exists or not
find_package(OpenGL REQUIRED)

set(PROTODAQ_MODULES_BASE_DIR 
    ${PROJECT_SOURCE_DIR}/external/modules
)

# Build static library (libglad.a) for imgui
add_library(glad STATIC ${PROTODAQ_MODULES_BASE_DIR}/glad/glad.c)
target_include_directories(glad PUBLIC ${PROTODAQ_MODULES_BASE_DIR}/glad)

set(exec_name ${PROJECT_NAME})

set(IMGUI_SRC
    imgui/src/implot_demo.cpp
    imgui/src/implot_items.cpp
    imgui/src/implot.cpp
    imgui/src/imgui.cpp
    imgui/src/imgui_draw.cpp
    imgui/src/imgui_impl_glfw.cpp
    imgui/src/imgui_impl_opengl3.cpp
    imgui/src/imgui_impl_sdl2.cpp
    imgui/src/imgui_tables.cpp
    imgui/src/imgui_widgets.cpp
    imgui/src/imgui_demo.cpp
)

set(FULL_IMGUI_SRC_PATH)
foreach(src ${IMGUI_SRC})
    list(APPEND FULL_IMGUI_SRC_PATH "${PROTODAQ_MODULES_BASE_DIR}/${src}")
endforeach()


add_executable(${exec_name} main.cpp ${FULL_IMGUI_SRC_PATH})

target_include_directories(${exec_name} PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${PROTODAQ_MODULES_BASE_DIR}/imgui/include
    ${PROTODAQ_MODULES_BASE_DIR}/imgui/src
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${exec_name} PRIVATE glfw glad SDL2::SDL2 OpenGL::GL)
