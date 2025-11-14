# Visual Studio 编译 C++20 Beacon 指南

## 概述

本指南介绍如何使用 Visual Studio 编译 C++20 版本的 Beacon。C++20 版本使用了现代 C++ 特性，包括智能指针、标准容器等，相比原版本更加安全和现代化。

## 前置要求

- Visual Studio 2022 (17.0 或更高版本)
- Windows 10 SDK (10.0.26100.0 或更高版本)
- 支持 C++20 的 MSVC 编译器

## 项目结构

```
project/
├── beacon.sln                    # Visual Studio 解决方案文件
├── beacon/
│   └── beacon.vcxproj           # 原版本项目文件
├── beacon_cpp20/
│   ├── beacon_cpp20.vcxproj     # C++20 项目文件 (新增)
│   ├── Agent.cpp                 # C++20 现代化源文件
│   ├── Agent.h                   # C++20 现代化头文件
│   └── ...                       # 其他 C++20 源文件
└── ...
```

## 编译步骤

### 1. 打开解决方案

双击 `beacon.sln` 文件在 Visual Studio 中打开解决方案。

### 2. 选择 C++20 项目

在解决方案资源管理器中，您会看到两个项目：
- `beacon` - 原版本
- `beacon_cpp20` - C++20 版本

右键点击 `beacon_cpp20` 项目，选择"设为启动项目"。

### 3. 选择编译配置

Visual Studio 提供多种配置选项：

#### 调试配置
- `Debug EXE` - 调试版本可执行文件 (HTTP/WebSocket)
- `Debug_SMB` - 调试版本 SMB Beacon
- `Debug_TCP` - 调试版本 TCP Beacon
- `DebugDLL` - 调试版本 DLL

#### 发布配置
- `Release EXE` - 发布版本可执行文件 (HTTP/WebSocket)
- `Release SVC` - 发布版本服务
- `Release DLL` - 发布版本 DLL

### 4. 选择目标平台

- `x64` - 64位版本
- `x86` - 32位版本

### 5. 编译项目

使用以下任一方式编译：

**方法一：菜单编译**
- 菜单：`生成` → `生成解决方案` (Ctrl+Shift+B)

**方法二：右键编译**
- 在解决方案资源管理器中右键点击 `beacon_cpp20` 项目
- 选择 `生成`

**方法三：工具栏编译**
- 在 Visual Studio 工具栏中选择配置和平台
- 点击绿色"生成"按钮

## C++20 特性配置

项目已配置以下 C++20 特性：

### 编译器设置
```xml
<LanguageStandard>stdcpp20</LanguageStandard>
```

### 主要现代化改进
- **智能指针**: 使用 `std::unique_ptr` 替代原始指针
- **标准容器**: 使用 `std::vector`, `std::map`, `std::array`
- **类型安全**: 使用标准整数类型 (`std::uint8_t`, `std::uint32_t`)
- **常量正确性**: 方法标记 `const` 关键字
- **RAII**: 自动资源管理

## 输出文件

编译完成后，输出文件位于：

```
beacon_cpp20/x64/Debug/          # 调试版本 x64
beacon_cpp20/x64/Release/        # 发布版本 x64
beacon_cpp20/Win32/Debug/         # 调试版本 x86
beacon_cpp20/Win32/Release/       # 发布版本 x86
```

## 常见问题

### Q: 编译时出现 C++20 相关错误
**A**: 确保使用 Visual Studio 2022，并且项目配置中设置了 `<LanguageStandard>stdcpp20</LanguageStandard>`

### Q: 链接错误找不到 ws2_32.lib
**A**: 项目已配置链接 `ws2_32.lib` 和 `winsock2.lib`，如果仍有问题，检查 Windows SDK 是否正确安装

### Q: 预处理器定义错误
**A**: 根据需要的协议类型，确保预处理器定义正确：
- HTTP/WebSocket: `BEACON_WS`
- SMB: `BEACON_SMB` 
- TCP: `BEACON_TCP`
- DLL: `BUILD_DLL`

## 性能对比

C++20 版本相比原版本：
- ✅ 内存安全：使用智能指针避免内存泄漏
- ✅ 类型安全：强类型系统减少运行时错误
- ✅ 现代化：符合现代 C++ 最佳实践
- ✅ 可维护性：代码更清晰，易于理解和维护
- ⚖️ 性能：编译后性能基本相同

## 调试技巧

1. **设置断点**: 在 C++20 代码中设置断点进行调试
2. **内存检查**: 使用 Visual Studio 的内存检查功能验证智能指针的正确使用
3. **性能分析**: 使用性能分析器对比原版本和 C++20 版本的性能

## 其他编译方式

除了 Visual Studio，您还可以使用：

1. **CMake**: 
   ```bash
   cd beacon_cpp20
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   ```

2. **Makefile**:
   ```bash
   make x64-cpp20    # 编译 x64 C++20 版本
   make x86-cpp20    # 编译 x86 C++20 版本
   make all-cpp20    # 编译所有 C++20 版本
   ```

## 技术支持

如遇到编译问题，请检查：
1. Visual Studio 版本是否支持 C++20
2. Windows SDK 是否正确安装
3. 项目配置是否正确
4. 源文件路径是否正确

更多信息请参考：
- `MODERNIZATION.md` - 详细的现代化改进文档
- `README.md` - 项目总体说明
- `beacon_cpp20/README.md` - C++20 版本特定说明