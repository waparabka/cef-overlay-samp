cmake_minimum_required(VERSION 3.15)

project(cef-overlay-samp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(DIRECTX_SDK_PATH "C:/Program Files (x86)/DirectX SDK")
set(CEF_PATH "C:/cef_binary")
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x86-windows-static" CACHE STRING "CMake prefix path")

include_directories(${DIRECTX_SDK_PATH}/Include)
link_directories(${DIRECTX_SDK_PATH}/Lib/x86)

include_directories(${CEF_PATH}/Release)
link_directories(${CEF_PATH}/Release)

include(FetchContent)

FetchContent_Declare(rakhook GIT_REPOSITORY https://github.com/waparabka/rakhook.git)
FetchContent_MakeAvailable(rakhook)

add_library(${PROJECT_NAME} SHARED)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC rakhook imgui::imgui d3d9 d3dx9 dxguid
					${CEF_PATH}/Release/libcef.lib
					${CEF_PATH}/Release/libcef_dll_wrapper.lib)

target_sources(${PROJECT_NAME} PRIVATE source/main.cpp source/plugin.cpp source/overlay.cpp source/render.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} SUFFIX ".asi")
target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)