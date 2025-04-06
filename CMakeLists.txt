cmake_minimum_required(VERSION 3.30)
project(GateServer_CLion)

# 设置控制台子系统
if(WIN32)
    add_compile_options(-mconsole)
    add_link_options(-Wl,-subsystem,console)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GateServer_CLion main.cpp
        CServer.cpp
        CServer.h
        Const.h
        HttpConnection.cpp
        HttpConnection.h
        Singleton.h
        LogicSystem.cpp
        LogicSystem.h)

# 链接系统库和 Boost（如果使用）
target_link_libraries(GateServer_CLion PRIVATE
        ws2_32
        mswsock
        advapi32
        # Boost::system  # 按需添加
)
# 启用 Boost.Asio 的 Windows 特性
add_definitions(-DBOOST_ASIO_HAS_WINDOWS_OVERLAPPED_PTR)
