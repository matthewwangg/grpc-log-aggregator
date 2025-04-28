#!/bin/bash
set -e

echo "[-] Cleaning old client build..."
rm -rf build
mkdir build
cd build

echo "[+] Running CMake for client..."
cmake ..
echo "[+] Building client..."
make -j$(nproc)

echo "[✓] Client build complete."
