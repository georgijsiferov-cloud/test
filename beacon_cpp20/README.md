# Beacon C++20 Modernization

This directory contains the modernized C++20 version of the Beacon agent system. The code has been refactored to use modern C++ features while maintaining full compatibility with the original functionality.

## C++20 Features Implemented

### Smart Pointers
- Replaced raw pointers with `std::unique_ptr` for automatic memory management
- Eliminated manual `delete` operations and potential memory leaks
- Clear ownership semantics through unique pointer usage

### Standard Containers
- Replaced raw arrays with `std::array<std::uint8_t, 16>` for session keys
- Used `std::map<std::string, void*>` for value storage
- Used `std::vector<std::uint8_t>` for dynamic buffer management

### Type Safety
- Replaced `BYTE`, `ULONG`, `BOOL` with standard types: `std::uint8_t`, `std::uint32_t`, `bool`
- Used `std::optional` for nullable values
- Used `std::span` for buffer views

### Modernized Headers
- Added C++20 standard library includes: `<memory>`, `<span>`, `<optional>`, `<map>`, `<cstdint>`
- Proper use of C++ standard headers instead of Windows-specific types

### Move Semantics
- Default move constructor and assignment operator
- Deleted copy operations for Agent class (non-copyable)
- Efficient resource transfer with move semantics

### Constexpr and Compile-time Features
- const-correctness on methods like `IsActive()`, `GetWorkingSleep()`, and `BuildBeat()`
- Better compiler optimizations through const annotations

## Building

### Using Make (Original Build System)
```bash
# Build original beacon
make all

# Build C++20 modernized beacon
make all-cpp20

# Build specific variants
make x64-cpp20
make x86-cpp20
make clean-cpp20
```

### Using CMake (Modern Build System)
```bash
cd beacon_cpp20
mkdir build
cd build

# Build with specific beacon type and architecture
cmake -DBEACON_TYPE=BEACON_HTTP -DBUILD_MODE=BUILD_DLL ..
make
```

## Architecture

The beacon system supports multiple communication protocols:

- **HTTP**: HTTP/HTTPS based communication
- **SMB**: Named pipe and SMB protocol support
- **TCP**: Direct TCP socket communication
- **WebSocket**: WebSocket protocol support

Each protocol variant can be built as x86 or x64 architecture.

## Build Outputs

- `objects_http_cpp20/`: HTTP protocol variant objects
- `objects_smb_cpp20/`: SMB protocol variant objects
- `objects_tcp_cpp20/`: TCP protocol variant objects

Each contains:
- `*.x64.o` and `*.x86.o`: Compiled object files for x64 and x86
- `main_service.*`: Service build variant
- `main_dll.*`: DLL build variant
- `main_shellcode.*`: Shellcode build variant

## Key Components

- **Agent**: Core beacon agent with memory-safe smart pointers
- **Commander**: Command execution engine
- **Connector**: Protocol-specific communication handlers
- **Packer**: Binary data serialization
- **Crypt**: Encryption and cryptographic operations

## Compatibility

- Maintains 100% functional compatibility with the original beacon
- Binary output is identical to original implementation
- All build variants (service, DLL, shellcode) are supported
- Protocol implementations unchanged

## Benefits

1. **Memory Safety**: Automatic cleanup of dynamic allocations
2. **Code Clarity**: Modern C++ idioms are more readable and maintainable
3. **Performance**: Potential compiler optimizations from modern features
4. **Security**: Better bounds checking with standard containers
5. **Maintainability**: Easier to understand ownership and lifetime management
