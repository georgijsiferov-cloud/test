# Beacon Agent System

A modernized C2 (Command & Control) beacon agent framework with both original and C++20 implementations.

## Overview

This repository contains the Beacon Agent System from the Adaptix Framework, a sophisticated C2 agent designed for command and control operations. The project includes:

1. **Original Implementation** (`beacon/`): Classic C++ implementation maintaining compatibility with Windows x86/x64 architectures
2. **C++20 Modernized Version** (`beacon_cpp20/`): Refactored using modern C++20 features for improved maintainability and safety

## Quick Start

### Building the Original Beacon

```bash
# Build all variants (HTTP, SMB, TCP for x86/x64)
make all

# Build specific architecture
make x64        # x64 architecture
make x86        # x86 architecture

# Clean build artifacts
make clean
```

### Building the C++20 Beacon

```bash
# Build all C++20 variants
make all-cpp20

# Build specific C++20 variants
make x64-cpp20
make x86-cpp20

# Clean C++20 artifacts
make clean-cpp20
```

### Building with Visual Studio (C++20 version)

#### 方法一：使用 Visual Studio IDE

1. 打开 `beacon.sln` 解决方案文件
2. 选择 `beacon_cpp20` 项目
3. 选择配置（Debug/Release）和平台（x86/x64）
4. 选择协议类型：
   - `Debug EXE` / `Release EXE` - HTTP/WebSocket Beacon
   - `Debug_SMB` / `Release SVC` - SMB Beacon  
   - `Debug_TCP` - TCP Beacon
   - `DebugDLL` / `Release DLL` - DLL 版本
5. 点击"生成" → "生成解决方案"

#### 方法二：使用批处理脚本

```bash
# 运行自动编译脚本
build_vs_cpp20.bat
```

#### 方法三：使用 MSBuild 命令行

```bash
# 编译 x64 Release 版本
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="Release EXE" /p:Platform=x64 /p:LanguageStandard=stdcpp20

# 编译 x86 Debug DLL 版本
MSBuild beacon_cpp20\beacon_cpp20.vcxproj /p:Configuration="DebugDLL" /p:Platform=Win32 /p:LanguageStandard=stdcpp20
```

> 📖 **详细指南**: 查看 `VISUAL_STUDIO_CPP20_GUIDE.md` 获取完整的 Visual Studio 编译指南

### Building with CMake (C++20 version)

```bash
cd beacon_cpp20
mkdir build && cd build

cmake -DBEACON_TYPE=BEACON_HTTP \
      -DBUILD_MODE=BUILD_DLL \
      -DTARGET_ARCH=x64 \
      ..

make -j$(nproc)
```

## Features

### Supported Protocols
- **HTTP/HTTPS**: Direct HTTP communication
- **SMB**: Named pipe and SMB protocol
- **TCP**: Raw TCP socket communication
- **WebSocket**: WebSocket protocol support

### Build Variants
- **Service**: Windows service deployment
- **DLL**: Dynamic link library deployment
- **Shellcode**: Position-independent shellcode

### Architectures
- x86 (32-bit)
- x86_64 (64-bit)

## Project Structure

```
.
├── beacon/                  # Original beacon implementation
├── beacon_cpp20/            # C++20 modernized beacon
├── files/                   # Configuration templates and stubs
├── Makefile                 # Build configuration for both versions
├── .github/workflows/       # CI/CD pipelines
├── MODERNIZATION.md         # Detailed modernization documentation
└── README.md               # This file
```

## Architecture

### Core Components

- **Agent**: Main beacon agent with configuration and state management
- **Commander**: Command execution and task handling
- **Connectors**: Protocol-specific communication handlers
- **Crypto**: Encryption and cryptographic operations
- **Packer**: Binary data serialization

### Memory Model

**Original Version:**
- Manual memory management with custom allocators
- Traditional pointer-based object ownership

**C++20 Version:**
- Automatic memory management via smart pointers (`std::unique_ptr`)
- Standard containers (`std::vector`, `std::map`, `std::array`)
- RAII (Resource Acquisition Is Initialization) principles

## CI/CD Pipeline

The project includes comprehensive GitHub Actions workflows:

### 1. Build Pipeline (`build-beacon.yml`)
- Compiles both original and C++20 variants
- Tests all protocol types
- Validates both x86 and x64 architectures
- Produces build artifacts

### 2. CMake Build (`build-cmake.yml`)
- Tests modern CMake-based build system
- Validates multiple configurations
- Cross-compilation support

### 3. Code Analysis (`code-analysis.yml`)
- Static code analysis with cppcheck
- Security scanning with flawfinder
- Code formatting validation
- Dependency validation
- Compiler compatibility checks

## Documentation

### Key Documents
- [MODERNIZATION.md](./MODERNIZATION.md) - Detailed modernization guide and technical changes
- [beacon_cpp20/README.md](./beacon_cpp20/README.md) - C++20 specific features and building instructions
- [beacon_cpp20/CMakeLists.txt](./beacon_cpp20/CMakeLists.txt) - CMake configuration

## Requirements

### Build Requirements
- MinGW-w64 cross-compiler toolchain
- Make (version 4.0+)
- CMake (version 3.20+) for C++20 builds
- Linux/Unix environment

### Optional Tools
- cppcheck (for code analysis)
- clang-format (for formatting checks)
- flawfinder (for security scanning)

## Installation

```bash
# Ubuntu/Debian
sudo apt-get install mingw-w64 make cmake

# CentOS/RHEL
sudo yum install mingw64-gcc mingw32-gcc make cmake

# macOS (via Homebrew)
brew install mingw-w64 cmake
```

## Building

### Complete Build

```bash
# Build both original and C++20 variants
make all all-cpp20
```

### Specific Builds

```bash
# Original beacon - all protocols, all architectures
make all

# C++20 beacon - all protocols, all architectures
make all-cpp20

# Original beacon - HTTP protocol only
make HTTP_DIST_DIR=objects_http_final

# C++20 beacon - specific protocol
BEACON_TYPE=BEACON_SMB make all-cpp20
```

## Output

Build artifacts are organized by protocol and architecture:

```
objects_http/                # Original HTTP variant
  ├── *.x64.o               # x64 object files
  ├── *.x86.o               # x86 object files
  └── main_*.o              # Main build variants

objects_http_cpp20/         # C++20 HTTP variant
objects_smb/                # Original SMB variant
objects_smb_cpp20/          # C++20 SMB variant
objects_tcp/                # Original TCP variant
objects_tcp_cpp20/          # C++20 TCP variant
```

## Configuration

Configuration is managed through:
- Compiler preprocessor definitions (BEACON_HTTP, BEACON_SMB, etc.)
- Build mode flags (BUILD_DLL, BUILD_SVC, BUILD_SHELLCODE)
- Target architecture selection (x86, x64)

## Compatibility

| Aspect | Original | C++20 |
|--------|----------|-------|
| Functionality | ✓ | ✓ (100% compatible) |
| Binary Output | ✓ | ✓ (functionally identical) |
| Protocol Support | ✓ | ✓ |
| Architecture Support | ✓ | ✓ |
| Backward Compatibility | N/A | ✓ |

## Security Considerations

### Memory Safety
- C++20 version provides automatic memory management
- No buffer overflow risks from smart pointers
- Bounds checking with standard containers

### Build Security
- CI/CD validates code quality
- Static analysis on each commit
- Security scanning for vulnerable patterns

## Performance

- **Original**: Baseline performance
- **C++20**: Equivalent performance with potential optimization opportunities

Binary sizes and execution speed are functionally identical between versions.

## Development

### Adding Features

To add new features:

1. Implement in `beacon_cpp20/` using modern C++ practices
2. Maintain compatibility with original protocol
3. Update tests and documentation
4. Run CI/CD validation

### Contributing

Guidelines:
1. Use C++20 features in the modern version
2. Maintain RAII principles
3. Keep methods const-correct
4. Follow existing code organization

## Troubleshooting

### Build Failures

```bash
# Verify MinGW installation
x86_64-w64-mingw32-g++ --version

# Check file permissions
chmod +x files/*.bin

# Clean and rebuild
make clean all
```

### Compilation Errors

Most errors relate to:
1. Missing MinGW-w64 toolchain
2. Incorrect Windows SDK path
3. Header file path issues

## References

- [Adaptix Framework Documentation](https://deepwiki.com/Adaptix-Framework/AdaptixC2/4-beacon-agent-system)
- [C++20 Standard Reference](https://en.cppreference.com/w/cpp/20)
- [CMake Documentation](https://cmake.org/documentation/)

## License

Refer to the original Adaptix Framework license.

## Support

For issues and questions:
1. Check existing documentation
2. Review CI/CD logs for build errors
3. Consult MODERNIZATION.md for technical details

---

**Note:** This is a modernized reference implementation. For production use, ensure proper validation and security hardening.
