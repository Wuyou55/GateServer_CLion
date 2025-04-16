cmake_minimum_required(VERSION 3.30)
project(GateServer_CLion)

# 设置控制台子系统
if (WIN32)
    add_compile_options(-mconsole)
    add_link_options(-Wl,-subsystem,console)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(server)
add_subdirectory(proto)