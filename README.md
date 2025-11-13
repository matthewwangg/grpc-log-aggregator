# grpc-log-aggregator

A high-performance log aggregation server built with C++20 and gRPC. This project supports log ingestion, querying, streaming, and real-time subscription using a defined Protobuf schema.

## API Description

- **SendLog**: Send individual log entries
- **StreamLog**: Stream logs in batches
- **QueryLog**: Search logs by source, date, and optional keyword
- **SubscribeLog**: Real-time streaming of new logs via pub/sub

## Protobuf API Definition

Defined in `proto/log.proto`:

- `SendLog(LogEntry) → LogResponse`
- `StreamLog(stream LogEntry) → LogResponse`
- `QueryLog(QueryRequest) → QueryResponse`
- `SubscribeLog(SubscribeRequest) → stream LogEntry`

## Storage
- Logs are saved to disk under `logs/<source>/<YYYY-MM-DD>.log`
- GCS buckets are supported.

## Requirements

- CMake ≥ 3.27
- g++ with C++20 support
- gRPC and Protobuf (C++)

## Build Instructions

```bash
./build.sh
```

This script:
1. Cleans and regenerates Protobuf and gRPC files.
2. Sets up a fresh build directory.
3. Compiles the server using CMake and `make`.

To build the client:

```bash
cd client
./build_client.sh
```

## Project Structure

```
grpc-log-aggregator/
├── proto/                  # Protobuf definitions and generated code
│   └── log.proto
├── include/                # Public headers
│   ├── log_service_impl.h
│   ├── utils/
│   │   ├── log_utils.h
│   │   └── time_utils.h
├── src/                    # Server implementation
│   ├── main.cc
│   ├── log_service_impl.cc
│   └── utils/
│       ├── log_utils.cc
│       └── time_utils.cc
├── client/                 # Example client + tests
│   ├── main.cc             # Tests for SendLog, StreamLog, QueryLog, SubscribeLog
│   └── build_client.sh     # Script to build client binary
├── build.sh                # Build automation script for server
├── CMakeLists.txt          # Build configuration
```

## License

MIT License  
© Matthew Wang 2025
