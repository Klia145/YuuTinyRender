cmake_minimum_required(VERSION 3.15)
project(SoftwareRasterizer)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加include目录
include_directories(include)
include_directories(imgui)
include_directories(imgui/backends)

# SDL2路径
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/x86_64-w64-mingw32")
include_directories(${SDL2_DIR}/include/SDL2)
link_directories(${SDL2_DIR}/lib)


file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/**/*.cpp")


file(GLOB IMGUI_CORE_FILES "imgui/*.cpp")

# 手动指定需要的ImGui backend（只要SDL2相关的）
set(IMGUI_BACKEND_FILES
    imgui/backends/imgui_impl_sdl2.cpp
    imgui/backends/imgui_impl_sdlrenderer2.cpp
)

# 组合所有源文件
set(SOURCES
    main.cpp
    ${SRC_FILES}
    ${IMGUI_CORE_FILES}
    ${IMGUI_BACKEND_FILES}
)

# 如果用Config_Modern.hpp，排除constants.cpp
list(FILTER SOURCES EXCLUDE REGEX ".*constants\\.cpp$")

# 创建可执行文件
add_executable(viewer ${SOURCES})

# 链接SDL2
target_link_libraries(viewer PRIVATE
    mingw32
    SDL2main
    SDL2
)
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    target_link_libraries(viewer PRIVATE OpenMP::OpenMP_CXX)
    message(STATUS "OpenMP Found! Multi-threading enabled.")
else()
    message(WARNING "OpenMP not found. Running in single thread mode.")
endif()


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    message(STATUS "⚠️  Build Type was empty, FORCING to Release mode!")
endif()

# 再次确认并打印
message(STATUS ">>>>> Current Build Type: ${CMAKE_BUILD_TYPE} <<<<<")


if(MSVC)
    target_compile_options(viewer PRIVATE /W4 /O2)
else()
    target_compile_options(viewer PRIVATE -Wall)

    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_compile_options(viewer PRIVATE -g -O0)
    else()
        target_compile_options(viewer PRIVATE -O3 -ffast-math)
    endif()
endif()


# 输出信息
list(LENGTH SOURCES SOURCE_COUNT)
message(STATUS "Found ${SOURCE_COUNT} source files")