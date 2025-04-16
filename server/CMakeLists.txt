find_package(Boost REQUIRED)
find_package(jsoncpp REQUIRED)

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
        src/VarifyGrpcClient.cpp
        src/VarifyGrpcClient.h
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