# Quick Start Guide

## Installation

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y mingw-w64 make cmake git

# Verify installation
x86_64-w64-mingw32-g++ --version
make --version
cmake --version
```

## Building

### Option 1: Using the Build Script (Recommended)

```bash
# Make script executable
chmod +x build.sh

# Build both original and C++20 variants
./build.sh --all

# Build only C++20 variant
./build.sh --cpp20

# Build only x64 architecture
./build.sh --all --type x64

# Build with verbose output
./build.sh --cpp20 -v

# Show help
./build.sh --help
```

### Option 2: Using Make Directly

```bash
# Build original beacon (all variants)
make all

# Build original beacon (x64 only)
make clean
make pre
make x64

# Build C++20 beacon (all variants)
make all-cpp20

# Build C++20 beacon (x86 only)
make clean-cpp20
make pre-cpp20
make x86-cpp20

# Clean build artifacts
make clean      # Clean original builds
make clean-cpp20  # Clean C++20 builds
```

### Option 3: Using CMake (Modern Approach)

```bash
cd beacon_cpp20
mkdir -p build
cd build

# Configure for HTTP protocol
cmake -DBEACON_TYPE=BEACON_HTTP \
      -DBUILD_MODE=BUILD_DLL \
      -DTARGET_ARCH=x64_64 \
      ..

# Build
make -j$(nproc)

# List outputs
ls -lh
```

## Build Variants

### Protocols

- **BEACON_HTTP**: Standard HTTP/HTTPS communication
- **BEACON_SMB**: SMB/Named Pipe protocol
- **BEACON_TCP**: Raw TCP socket
- **BEACON_WS**: WebSocket protocol

### Build Modes

- **BUILD_DLL**: Dynamic library (.dll)
- **BUILD_SVC**: Windows service executable
- **BUILD_SHELLCODE**: Position-independent shellcode

### Architectures

- **x86**: 32-bit (i686-w64-mingw32)
- **x64**: 64-bit (x86_64-w64-mingw32)

## Output Locations

```
# Original builds
objects_http/          # HTTP variant
objects_smb/           # SMB variant
objects_tcp/           # TCP variant
  ├── *.x64.o         # 64-bit objects
  ├── *.x86.o         # 32-bit objects
  ├── main_dll.o
  ├── main_service.o
  └── main_shellcode.o

# C++20 builds
objects_http_cpp20/
objects_smb_cpp20/
objects_tcp_cpp20/
```

## Common Tasks

### Build for Specific Protocol

```bash
# Original: HTTP only
make clean && make pre
make HTTP_DIST_DIR=objects_http x64

# C++20: SMB protocol
BEACON_TYPE=BEACON_SMB make all-cpp20
```

### Cross-compile for Different Architecture

```bash
# Build for i686 (x86)
ARCH=i686-w64-mingw32 make all

# Build for x86_64 (x64)
ARCH=x86_64-w64-mingw32 make all
```

### Build Single File

```bash
# Compile single source with specific protocol
x86_64-w64-mingw32-g++ -c beacon_cpp20/Agent.cpp \
  -D BEACON_HTTP -I beacon_cpp20/ \
  -std=c++20 -fno-exceptions \
  -o Agent.x64.o
```

## Verification

### Check Build Success

```bash
# List all build artifacts
ls -lh objects_*/ | grep -E "\.o$"

# Count objects per protocol
for dir in objects_*/; do
  echo "$dir: $(ls $dir/*.o 2>/dev/null | wc -l) objects"
done

# Verify x64 and x86 objects exist
file objects_http/*.x64.o objects_http/*.x86.o
```

### Run Static Analysis

```bash
# Install analysis tools
sudo apt-get install -y cppcheck clang-tools

# Run cppcheck
cppcheck --enable=all beacon_cpp20/ --std=c++20

# Check for unsafe functions
grep -r "strcpy\|gets\|sprintf" beacon_cpp20/
```

## Troubleshooting

### Build Fails - No such file or directory

**Problem:** `beacon_cpp20/utils.h: No such file or directory`

**Solution:** Ensure you're building from the project root directory
```bash
cd /path/to/project
make all-cpp20
```

### Build Fails - Command not found

**Problem:** `x86_64-w64-mingw32-g++: command not found`

**Solution:** Install MinGW-w64
```bash
sudo apt-get install mingw-w64
```

### Build Fails - Permission denied

**Problem:** `build.sh: Permission denied`

**Solution:** Make script executable
```bash
chmod +x build.sh
./build.sh
```

### CMake Configuration Error

**Problem:** `CMake Error: Could not find cmake module path`

**Solution:** Ensure CMake is installed and run from correct directory
```bash
cmake --version  # Check if installed
cd beacon_cpp20/build  # Correct directory
cmake ..
```

## Development Workflow

### For C++20 Development

1. **Create feature branch**
   ```bash
   git checkout -b feature/my-enhancement
   ```

2. **Make changes in `beacon_cpp20/`**
   ```bash
   # Edit files following C++20 best practices
   - Use smart pointers (std::unique_ptr, std::shared_ptr)
   - Mark methods const when appropriate
   - Use standard containers (std::vector, std::map, std::array)
   ```

3. **Test compilation**
   ```bash
   ./build.sh --cpp20 --type x64
   ```

4. **Run analysis**
   ```bash
   make clean-cpp20  # Optional cleanup
   # CI/CD will run full analysis
   ```

5. **Commit and push**
   ```bash
   git add beacon_cpp20/
   git commit -m "feat: description of changes"
   git push origin feature/my-enhancement
   ```

## Performance Considerations

- **Build Time**: C++20 may have slightly longer compile times due to additional optimizations
- **Binary Size**: Negligible difference from original
- **Runtime Performance**: Identical to original implementation
- **Memory Usage**: Smart pointers provide same efficiency as manual management

## Next Steps

1. **Read MODERNIZATION.md** for detailed technical information
2. **Review beacon_cpp20/README.md** for C++20-specific details
3. **Check GitHub Actions workflows** to understand CI/CD
4. **Explore beacon_cpp20/Agent.h/cpp** to understand modern C++ patterns

## Support Resources

- **CMake Documentation**: https://cmake.org/documentation/
- **C++20 Reference**: https://en.cppreference.com/w/cpp/20
- **Beacon Framework**: https://deepwiki.com/Adaptix-Framework/AdaptixC2/4-beacon-agent-system
- **MinGW-w64**: http://mingw-w64.org/

## Quick Reference

```bash
# Build everything
make all all-cpp20

# Build x64 only
make clean && make x64 && make clean-cpp20 && make x64-cpp20

# Build x86 only
make clean && make x86 && make clean-cpp20 && make x86-cpp20

# Clean everything
make clean clean-cpp20

# Analyze code quality
cppcheck --enable=all beacon_cpp20/ --std=c++20 2>&1 | head -20

# Count lines of code
wc -l beacon_cpp20/*.{cpp,h} | tail -1

# Compare builds
diff <(ls objects_http/) <(ls objects_http_cpp20/)
```
