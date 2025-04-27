cmake_minimum_required(VERSION 3.27)
project(grpc-log-aggregator)

set(CMAKE_CXX_STANDARD 20)

set(PROTO_SRC
        proto/log.pb.cc
        proto/log.grpc.pb.cc
)

set(SERVER_SRC
        src/main.cc
        src/log_service_impl.cc
)

add_executable(grpc_log_aggregator
        ${SERVER_SRC}
        ${PROTO_SRC}
)

target_include_directories(grpc_log_aggregator PRIVATE proto/ include/)

target_link_libraries(grpc_log_aggregator
        grpc++
        grpc
        protobuf
)

