find_package(Boost REQUIRED)
find_package(jsoncpp REQUIRED)

add_compile_options("-Wl,--stack,16777216")  # 设置堆栈大小为 16MB
add_compile_options("-O1")  # 启用基本优化

set(SOURCES
        src/main.cpp
        src/Singleton.h
        src/Const.h
        src/CServer.cpp
        src/CServer.h
        src/HttpConnection.h
        src/HttpConnection.cpp
        src/LogicSystem.cpp
        src/LogicSystem.h
        src/VerifyGrpcClient.cpp
        src/VerifyGrpcClient.h
        src/ConfigMgr.cpp
        src/ConfigMgr.h
)

source_group("server/src" ROOT ${CMAKE_SOURCE_DIR} FILES ${SOURCE_FILES})

add_executable(server ${SOURCES})
target_link_libraries(server
        PRIVATE
        myproto
)
target_link_libraries(server PRIVATE
        ws2_32
        mswsock
        advapi32
        JsonCpp::JsonCpp
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
        ${CMAKE_CURRENT_BINARY_DIR}/config.ini
        COPYONLY
)