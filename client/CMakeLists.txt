cmake_minimum_required(VERSION 3.27)
project(grpc-log-client)

set(CMAKE_CXX_STANDARD 20)

set(PROTO_SRC
        ../proto/log.pb.cc
        ../proto/log.grpc.pb.cc
)

set(CLIENT_SRC
        main.cc
)

add_executable(grpc_log_client
        ${CLIENT_SRC}
        ${PROTO_SRC}
)

target_include_directories(grpc_log_client PRIVATE ../proto)
target_link_libraries(grpc_log_client PRIVATE grpc++ grpc protobuf)
