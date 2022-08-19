cmake_minimum_required(VERSION 3.23)
project(three_d_renderer)

include_directories(three_d_renderer PRIVATE "res/")
include_directories(three_d_renderer PRIVATE "src/")
include_directories(three_d_renderer PRIVATE "lib/bgfx/include")
include_directories(three_d_renderer PRIVATE "lib/bx/include")
include_directories(three_d_renderer PRIVATE "lib/glfw/include")
include_directories(three_d_renderer PRIVATE "lib/glm")

set(CMAKE_CXX_STANDARD 17)
set(PRJ_SHADERS_COMPILER "lib/bgfx/.build/win64_mingw-gcc/bin/shadercRelease.exe")
set(PRJ_TEXTURE_COMPILER "lib/bgfx/.build/win64_mingw-gcc/bin/texturecRelease.exe")
set(PRJ_BIN2C               "lib/bin2c/bin2c.exe")
set(PRJ_SHADERS_PLATFORM        "windows")
set(PRJ_SHADERS_ARGS_VS            -p vs_4_0 -O 3)
set(PRJ_SHADERS_ARGS_FS            -p ps_4_0 -O 3)

set(GEN_SRC)
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

        list(APPEND GEN_SRC ${FILE}.hpp)
        add_custom_command(
                OUTPUT ${FILE}.hpp
                COMMAND ${PRJ_SHADERS_COMPILER}
                -i lib/bgfx/src --type ${SHADER_TYPE} --platform ${PRJ_SHADERS_PLATFORM} ${SHADER_ARGS}
                -f ${FILE} -o ${FILE}.hpp --bin2c ${ARRAY_NAME}
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

file(GLOB ALL_SOURCE
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.c

        ${PROJECT_SOURCE_DIR}/src/*/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*/*.c

        ${PROJECT_SOURCE_DIR}/src/*/*/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*/*/*.c

        ${PROJECT_SOURCE_DIR}/src/*/*/*/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*/*/*/*.c
        )

list(APPEND ALL_SOURCE ${GEN_SRC})

file(GLOB ALL_HEADER
        ${PROJECT_SOURCE_DIR}/src/*.hpp
        ${PROJECT_SOURCE_DIR}/src/*.h

        ${PROJECT_SOURCE_DIR}/src/*/*.hpp
        ${PROJECT_SOURCE_DIR}/src/*/*.h

        ${PROJECT_SOURCE_DIR}/src/*/*/*.hpp
        ${PROJECT_SOURCE_DIR}/src/*/*/*.h

        ${PROJECT_SOURCE_DIR}/src/*/*/*/*.hpp
        ${PROJECT_SOURCE_DIR}/src/*/*/*/*.h
        )

# Process file groups
message("Grouping source files...")
macro(addGroup SRCS)
    foreach(FILE ${SRCS})
        # Get the directory of the source file
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

        # Remove common directory prefix to make the group
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

        # Make sure we are using windows slashes
        string(REPLACE "/" "\\" GROUP "${GROUP}")

        # Group into "Source Files" and "Header Files"
        if    ("${FILE}" MATCHES ".*\\.cpp")
            set(GROUP "Source Files${GROUP}")

        elseif("${FILE}" MATCHES ".*\\.mm")
            set(GROUP "Source Files${GROUP}")

        elseif("${FILE}" MATCHES ".*\\.c")
            set(GROUP "Source Files${GROUP}")

        elseif("${FILE}" MATCHES ".*\\.sc")
            set(GROUP "Source Files${GROUP}")

        elseif("${FILE}" MATCHES ".*\\.h")
            set(GROUP "Header Files${GROUP}")

        elseif("${FILE}" MATCHES ".*\\.hpp")
            set(GROUP "Header Files${GROUP}")

        elseif("${FILE}" MATCHES ".*\\.vs")
            set(GROUP "Shader Files${GROUP}")
        elseif("${FILE}" MATCHES ".*\\.fs")
            set(GROUP "Shader Files${GROUP}")
        elseif("${FILE}" MATCHES ".*\\.cs")
            set(GROUP "Shader Files${GROUP}")
        endif()

        source_group("${GROUP}" FILES "${FILE}")
    endforeach()
endmacro()

addGroup("${ALL_HEADER}")
addGroup("${ALL_SOURCE}")
addGroup("${ALL_SHADERS}")




add_executable(three_d_renderer ${ALL_HEADER} ${ALL_SOURCE} ${ALL_SHADERS})

target_link_directories(three_d_renderer PRIVATE "lib/bgfx/.build/win64_mingw-gcc/bin/")
target_link_directories(three_d_renderer PRIVATE "lib/glfw/.build/src/")
target_link_libraries(three_d_renderer bgfxDebug bxDebug bimgDebug bimg_decodeDebug glfw3)