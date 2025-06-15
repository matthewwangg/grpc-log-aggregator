FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y \
    g++ cmake make \
    libprotobuf-dev protobuf-compiler \
    libgrpc++-dev protobuf-compiler-grpc \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /grpc_log_aggregator

COPY . .

RUN ./build.sh

FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    libprotobuf-dev \
    libgrpc++-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /grpc_log_aggregator

COPY --from=builder /grpc_log_aggregator/build/grpc_log_aggregator .

EXPOSE 50051

CMD ["./grpc_log_aggregator"]