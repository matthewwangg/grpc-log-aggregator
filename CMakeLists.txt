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
        src/utils/time_utils.cc
        src/utils/log_utils.cc
        src/utils/auth_utils.cc
        src/utils/pubsub_utils.cc
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
        gpr
        absl_synchronization
)

