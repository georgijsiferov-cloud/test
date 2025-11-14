# Adaptix Modern C++ Beacon Agent System

现代C++20实现的Beacon代理系统，遵循Google C++代码规范。

## 概述

Adaptix Beacon Agent是一个C2（命令与控制）beacon实现，支持远程命令执行和任务管理。此实现使用现代C++特性，遵循业界最佳实践。

## 现代C++特性

本实现采用现代C++20特性：

### C++17特性
- **智能指针**：使用 `std::unique_ptr` 和 `std::shared_ptr` 实现自动内存管理
- **移动语义**：实现高效的移动构造函数和移动赋值运算符
- **强类型枚举**：使用 `enum class` 提供类型安全的枚举
- **可选值**：使用 `std::optional` 表示可选的返回类型
- **范围循环**：在适用场景使用基于范围的for循环
- **RAII**：资源获取即初始化，确保资源正确管理
- **原子操作**：使用 `std::atomic` 实现线程安全操作
- **标准线程库**：使用 `std::thread`、`std::mutex` 和 `std::condition_variable`
- **Chrono库**：使用 `std::chrono` 实现类型安全的时间操作

### C++20新增特性
- **Concepts（概念）**：编译时类型约束，替代SFINAE
- **std::format**：类型安全的格式化输出
- **Designated Initializers**：指定初始化器，代码更清晰
- **Three-way comparison**：统一的比较运算符
- **Ranges库**：强大的数据处理管道（预留扩展）

## Google C++代码规范遵循

代码库遵循Google C++代码规范：

### 命名约定
- **类名**：大驼峰命名（PascalCase），例如 `BeaconConfig`、`TaskResult`
- **函数和变量**：小写下划线命名（snake_case），例如 `set_server_address`、`is_running`
- **枚举值**：k前缀驼峰命名（kCamelCase），例如 `kHttp`、`kPending`
- **私有成员变量**：尾部下划线，例如 `server_address_`

### 其他规范
- **头文件保护**：使用 `#ifndef/#define` 模式，包含完整路径
- **文件扩展名**：头文件使用 `.h`，实现文件使用 `.cc`
- **包含顺序**：项目头文件优先
- **禁用C风格转换**：使用C++转换运算符
- **const正确性**：正确使用 `const` 限定符
- **显式构造函数**：适当使用 `explicit` 关键字
- **删除拷贝/移动**：显式删除不需要的拷贝/移动操作

## 架构设计

### 核心组件

1. **Beacon**：主协调器，管理beacon生命周期
   - 启动和停止beacon代理
   - 管理主事件循环
   - 处理睡眠间隔与抖动

2. **BeaconConfig**：配置管理
   - 服务器地址和端口
   - 睡眠间隔和抖动
   - 协议和加密设置
   - User Agent和beacon标识

3. **Task**：任务表示
   - 任务类型：Command（命令）、FileDownload（文件下载）、FileUpload（文件上传）、Sleep（休眠）、Script（脚本）
   - 任务状态：Pending（待处理）、Running（运行中）、Completed（已完成）、Failed（失败）
   - 元数据支持

4. **Transport**：抽象通信层
   - 从C2服务器接收任务
   - 向服务器发送任务结果
   - 通过 `HttpTransport` 实现

5. **CommandExecutor**：任务执行引擎
   - 执行命令和脚本
   - 处理文件操作
   - 返回结构化的任务结果

## 项目结构

```
modern_cpp_beacon/
├── CMakeLists.txt          # CMake构建配置
├── README.md               # 本文档
├── include/                # 头文件目录
│   └── adaptix/
│       └── beacon/
│           ├── beacon.h                # Beacon主类
│           ├── beacon_config.h         # 配置类
│           ├── command_executor.h      # 命令执行器
│           ├── http_transport.h        # HTTP传输实现
│           ├── task.h                  # 任务类
│           └── transport.h             # 传输抽象接口
└── src/                    # 源文件目录
    ├── beacon.cc
    ├── beacon_config.cc
    ├── command_executor.cc
    ├── http_transport.cc
    ├── main.cc             # 主程序入口
    └── task.cc
```

## 构建方法

```bash
cd modern_cpp_beacon
mkdir build && cd build
cmake ..
make
```

### 构建要求
- CMake 3.14 或更高版本
- 支持C++20的编译器（GCC 10+, Clang 10+, MSVC 2019 16.11+）
  - 推荐：GCC 13+, Clang 15+, MSVC 2022+
- Linux/Unix系统需要pthread库

## 文档资源

- [ARCHITECTURE.md](ARCHITECTURE.md): 架构设计与实现细节
- [EXAMPLES.md](EXAMPLES.md): 使用示例和扩展参考
- [CPP20_FEATURES.md](CPP20_FEATURES.md): C++20特性应用指南

## 使用示例

```cpp
#include "adaptix/beacon/beacon.h"
#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/command_executor.h"
#include "adaptix/beacon/http_transport.h"

int main() {
  using namespace adaptix::beacon;

  // 配置beacon
  auto config = std::make_shared<BeaconConfig>();
  config->set_server_address("127.0.0.1");
  config->set_server_port(8080);
  config->set_sleep_interval(std::chrono::seconds(5));
  config->set_jitter_percent(20);

  // 创建传输层和执行器
  auto transport = std::make_unique<HttpTransport>(config);
  auto executor = std::make_unique<DefaultCommandExecutor>();

  // 创建并启动beacon
  Beacon beacon(config, std::move(transport), std::move(executor));
  beacon.Start();

  // Beacon在后台线程运行
  // ...

  beacon.Stop();
  return 0;
}
```

## 相比C/C++混合编程的改进

1. **类型安全**：不使用 `void*` 指针，使用模板和正确的类型
2. **内存安全**：RAII和智能指针防止内存泄漏
3. **线程安全**：现代并发原语替代原始pthread
4. **错误处理**：`std::optional` 用于可能出错的操作
5. **接口设计**：纯虚类实现清晰的抽象
6. **无手动内存管理**：不使用 `malloc`/`free` 或 `new`/`delete`
7. **编译时检查**：`constexpr`、`noexcept` 等现代关键字
8. **标准库**：使用STL容器和算法

## 安全注意事项

这是一个参考实现，用于教育目的。生产环境使用应包括：
- 完整的加密实现
- HTTPS证书验证
- 身份认证机制
- 输入验证和清理
- 安全的凭证存储
- 日志和监控

## 运行示例

```bash
# 编译
cd modern_cpp_beacon/build
make

# 运行beacon代理
./beacon_agent
```

程序将运行10秒钟，模拟beacon操作，然后优雅退出。

## 许可证

MIT License
