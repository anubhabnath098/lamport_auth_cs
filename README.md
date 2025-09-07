# Lamport's One-Way Hash Chain Authentication Protocol

## Overview
This project implements Lamport's one-way hash chain authentication protocol using Qt6 and Crypto++.

## Features
- GUI-based application with real-time protocol visualization
- Network communication between Alice (verifier) and Bob (prover)
- SHA-256 based hash chain generation
- Configurable parameters via config.ini
- Progress tracking and detailed logging

## Build Requirements
- Qt 6.9.2
- MinGW compiler
- CMake
- Crypto++ (via vcpkg)

## Building the Project

### Step 1: Install vcpkg and Crypto++
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install cryptopp:x64-mingw-static
```

### Step 2: Configure CMake
```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.9.2/mingw_64/lib/cmake" -DCMAKE_TOOLCHAIN_FILE="C:/Users/anubh/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

### Step 3: Build
```bash
cmake -G "MinGW Makefiles" -S . -B build
cmake --build build
cmake --build build --target run
```

## Running the Application

1. Start one instance as Alice (Verifier):
   - Click "Start as Alice (Verifier)"
   - The application will listen on the configured port

2. Start another instance as Bob (Prover):
   - Click "Start as Bob (Prover)"
   - The application will connect to Alice

3. The protocol will automatically execute 99 rounds with 1-second intervals

## Configuration
Edit `config.ini` to modify:
- Network settings (IP addresses and ports)
- Chain length (default: 100)
- Sleep duration between rounds (default: 1000ms)

## Protocol Flow
1. **Setup Phase**: Bob generates hash chain h₀ → h₁ → ... → hₙ
2. **Challenge-Response**: 
   - Alice sends challenge c (counter value)
   - Bob computes response r = H^(n-c)(h₀)
   - Alice verifies: Hash(r) = last valid hash
   - Process repeats for n-1 rounds

## Security Notes
- The hash chain provides one-way authentication
- Each hash value can only be used once
- Forward security: compromising current value doesn't reveal previous values