#!/bin/bash
set -e

echo "[-] Cleaning old generated proto files..."
rm -f proto/*.pb.cc proto/*.pb.h proto/*.grpc.pb.cc proto/*.grpc.pb.h

echo "[+] Regenerating proto and gRPC files..."
protoc --proto_path=proto --cpp_out=proto --experimental_allow_proto3_optional proto/log.proto
protoc --proto_path=proto --grpc_out=proto --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin --experimental_allow_proto3_optional proto/log.proto

echo "[+] Setting up build directory..."
rm -rf build
mkdir build
cd build

echo "[+] Running CMake..."
cmake ..

echo "[+] Building project..."
make -j$(nproc)

echo "[✓] Build complete."
