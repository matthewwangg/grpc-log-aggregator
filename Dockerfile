FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y \
    g++ cmake make \
    libprotobuf-dev protobuf-compiler \
    libgrpc++-dev protobuf-compiler-grpc \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /grpc_log_aggregator

COPY . .

RUN ./build.sh

WORKDIR /grpc_log_aggregator/build

EXPOSE 50051

CMD ["./grpc_log_aggregator"]