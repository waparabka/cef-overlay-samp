cmake_minimum_required(VERSION 3.15)

project(cef-sub-process)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CEF_PATH "C:/cef_binary")

include_directories(${CEF_PATH}/Release)
link_directories(${CEF_PATH}/Release)

add_executable(${PROJECT_NAME} source/main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE LINK_FLAGS "/ENTRY:WinMainCRTStartup")

target_link_libraries(${PROJECT_NAME} PUBLIC ${CEF_PATH}/Release/libcef.lib ${CEF_PATH}/Release/libcef_dll_wrapper.lib)