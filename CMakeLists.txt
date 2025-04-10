cmake_minimum_required(VERSION 3.30)
project(GateServer_CLion)

# 设置控制台子系统
if (WIN32)
    add_compile_options(-mconsole)
    add_link_options(-Wl,-subsystem,console)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(jsoncpp REQUIRED)

add_executable(GateServer_CLion main.cpp
        CServer.cpp
        CServer.h
        Const.h
        HttpConnection.cpp
        HttpConnection.h
        Singleton.h
        LogicSystem.cpp
        LogicSystem.h)

# 链接系统库和 Boost
target_link_libraries(GateServer_CLion PRIVATE
        ws2_32
        mswsock
        advapi32
)
