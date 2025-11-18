# Lamport's One-Way Hash Chain Authentication Protocol

## Overview

This project implements Lamport's one-way hash chain authentication protocol using Qt6 and Crypto++

### Features

* GUI-based application with real-time protocol visualization
* Network communication between Alice (Verifier) and Bob (Prover)
* SHA-256 based hash chain generation
* Configurable parameters via config.ini
* Progress tracking and detailed logging

## Protocol Flow

The protocol is a challenge-response system where identity is proven using a one-way hash chain. The process involves two key phases:

1. **Setup Phase**: Bob generates hash chain h₀ → h₁ → ... → hₓ and provides hₓ to Alice.
2. **Challenge-Response Phase**:

   * Alice sends challenge c (counter value)
   * Bob computes response r = H^(n-c)(h₀)
   * Alice verifies: Hash(r) = last valid hash
   * This repeats for n-1 rounds

This ensures forward security: each hash is used only once and is irreversible.

## Configuration

Edit `config.ini` to modify:

* Network settings (IP addresses and ports)
* Chain length (default: 100)
* Sleep duration between rounds (default: 1000ms)

## Security Notes

* One-way hash provides authentication
* Each hash value is used only once
* Forward security maintained

## Build Requirements

* Qt 6.9.2 (or compatible Qt6 version)
* CMake
* Crypto++
* MinGW compiler (for Windows) or g++ (for Linux)

---

## CMakeLists Information  
#### Paste one of this in CMakeLists.txt based on your OS (Also make sure you edit the appropriate fallback path according to your system)
Depending on your platform, you should use the appropriate CMakeLists file  

* Windows: `info/CMakeLists_windows.txt`
* Linux (Ubuntu): `info/CMakeLists_linux.txt`

Ensure you consult the correct file for platform-specific build configurations.

---

## Building and Running the Project

### Common Steps

1. Clone the repository.
2. Configure `config.ini` as needed.

---

### Windows Instructions

#### Step 1: Install vcpkg and Crypto++

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install cryptopp:x64-mingw-static
```

#### Step 2: Configure CMake (Change path according to your system)

```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.9.2/mingw_64/lib/cmake" -DCMAKE_TOOLCHAIN_FILE="C:/Users/anubh/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

#### Step 3: Build and Run

```bash
cmake -G "MinGW Makefiles" -S . -B build
cmake --build build
cmake --build build --target run
```

---

### Linux (Ubuntu) Instructions

#### Step 1: Install Dependencies

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-tools-dev libcrypto++-dev
```

#### Step 2: Configure and Build

```bash
mkdir build
cd build
cmake ./..
cmake --build .
```

#### Step 3: Running the Application
#### Double click on the executable file inside build folder or:

#### For Windows (For Linux simply change .exe to .out)
* Launch Alice (Verifier):

  ```bash
  cd build
  ./LamportAuthProtocol.exe
  ```

  Click "Start as Alice (Verifier)" in GUI.

* Launch Bob (Prover):

  ```bash
  cd build
  ./LamportAuthProtocol.exe
  ```

  Click "Start as Bob (Prover)" in GUI.

---

## Application Screenshots

### Alice (Verifier) Screen

![Alice Screen](info/alice_screenshot.png)

### Bob (Prover) Screen

![Bob Screen](info/bob_screenshot.png)

---

## Notes

* On both platforms, two separate instances are required (Alice and Bob).
* The protocol automatically performs 99 rounds with 1-second intervals.

## Contact

For questions or issues, please contact the project maintainer.
