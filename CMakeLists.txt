cmake_minimum_required(VERSION 3.23)
project(three_d_renderer)

set(CMAKE_CXX_STANDARD 17)
set(PRJ_SHADERS_COMPILER "lib/bgfx/.build/win64_mingw-gcc/bin/shadercRelease.exe")
set(PRJ_TEXTURE_COMPILER "lib/bgfx/.build/win64_mingw-gcc/bin/texturecRelease.exe")
set(PRJ_BIN2C               "lib/bin2c/bin2c.exe")
set(PRJ_TARGET_WINDOWS            1)
set(PRJ_SHADERS_PLATFORM        "windows")
set(PRJ_SHADERS_ARGS_VS            -p vs_4_0 -O 3)
set(PRJ_SHADERS_ARGS_FS            -p ps_4_0 -O 3)
set(PRJ_BUILD_EXECUTABLE        1)

set(GENERATED_SHADERS src/dbg_colors.h)
macro(compileShaders SRCS)
    foreach(FILE ${SRCS})
        get_filename_component(ARRAY_NAME "${FILE}" NAME_WE)

        if    ("${FILE}" MATCHES ".*\\.sc")
            continue()
        endif()

        if    ("${FILE}" MATCHES ".*\\.fs")
            set(SHADER_TYPE "fragment")
            set(SHADER_ARGS ${PRJ_SHADERS_ARGS_FS})
            set(ARRAY_NAME "${ARRAY_NAME}_fs")
        elseif("${FILE}" MATCHES ".*\\.vs")
            set(SHADER_TYPE "vertex")
            set(SHADER_ARGS ${PRJ_SHADERS_ARGS_VS})
            set(ARRAY_NAME "${ARRAY_NAME}_vs")
        elseif("${FILE}" MATCHES ".*\\.cs")
            set(SHADER_TYPE "compute")
            set(ARRAY_NAME "${ARRAY_NAME}_cs")
        endif()

        list(APPEND GENERATED_SHADERS ${FILE}.h)
        add_custom_command(
                OUTPUT ${FILE}.h
                COMMAND ${PRJ_SHADERS_COMPILER}
                -i lib/bgfx/src --type ${SHADER_TYPE} --platform ${PRJ_SHADERS_PLATFORM} ${SHADER_ARGS}
                -f ${FILE} -o ${FILE}.h --bin2c ${ARRAY_NAME}
                DEPENDS ${FILE} ${PRJ_SHADERS_COMPILER}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endforeach()
endmacro()

file(GLOB ALL_SHADERS
        ${PROJECT_SOURCE_DIR}/res/shaders/*/*.vs
        ${PROJECT_SOURCE_DIR}/res/shaders/*/*.fs
        ${PROJECT_SOURCE_DIR}/res/shaders/*/*.cs
        ${PROJECT_SOURCE_DIR}/res/shaders/*/*.sc
        )
compileShaders("${ALL_SHADERS}")

include_directories(three_d_renderer PRIVATE "res/")
include_directories(three_d_renderer PRIVATE "src/")
include_directories(three_d_renderer PRIVATE "lib/bgfx/include")
include_directories(three_d_renderer PRIVATE "lib/bx/include")
include_directories(three_d_renderer PRIVATE "lib/glfw/include")

link_directories(three_d_renderer PRIVATE "lib/bgfx/.build/win64_mingw-gcc/bin/")
link_directories(three_d_renderer PRIVATE "lib/glfw/.build/src/")

add_executable(three_d_renderer src/main.cpp ${GENERATED_SHADERS} src/shader/shader.cpp src/shader/shader.h src/main.h src/mesh/surface.h src/pipeline/renderer.cpp src/pipeline/renderer.h)
target_link_libraries(three_d_renderer bgfxRelease bxRelease bimgRelease bimg_decodeRelease glfw3)