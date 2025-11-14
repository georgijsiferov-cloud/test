# C++20 特性应用指南

本文档说明Adaptix Beacon项目中使用的C++20特性。

## 编译要求

- **C++标准**: C++20
- **CMake版本**: 3.14+
- **编译器要求**:
  - GCC 10+ (推荐 GCC 11+)
  - Clang 10+ (推荐 Clang 12+)
  - MSVC 19.29+ (Visual Studio 2019 16.11+)

## 已应用的C++20特性

### 1. Concepts（概念）

**文件**: `include/adaptix/beacon/concepts.h`

Concepts提供了编译时的类型约束，使模板代码更安全、更易读。

```cpp
template <typename T>
concept StringLike = std::convertible_to<T, std::string_view>;

template <typename T>
concept Transportable = requires(T t) {
  { t.ReceiveTask() } -> std::same_as<std::optional<Task>>;
  { t.SendResult(std::declval<TaskResult>()) } -> std::same_as<bool>;
};

template <typename T>
concept Executable = requires(T t, Task task) {
  { t.Execute(task) } -> std::same_as<TaskResult>;
};
```

**使用示例**:
```cpp
template <Transportable T, Executable E>
void RunBeacon(std::shared_ptr<BeaconConfig> config,
               std::unique_ptr<T> transport,
               std::unique_ptr<E> executor) {
  // 只接受满足Transportable和Executable概念的类型
  Beacon beacon(config, std::move(transport), std::move(executor));
  beacon.Start();
}
```

**优势**:
- ✅ 编译时类型检查
- ✅ 更清晰的错误消息
- ✅ 替代SFINAE，代码更易读
- ✅ 明确接口要求

### 2. std::format（格式化输出）

**文件**: `src/main_cpp20.cc`

替代传统的`printf`和`iostream`，提供类型安全的格式化。

```cpp
std::cout << std::format("Server: {}:{}\n", 
                        config.server_address(), 
                        config.server_port());

std::cout << std::format("Sleep: {}ms (jitter: {}%)\n", 
                        config.sleep_interval().count(), 
                        config.jitter_percent());
```

**优势**:
- ✅ 类型安全
- ✅ Python风格的格式化语法
- ✅ 比`iostream`更快
- ✅ 支持自定义类型格式化

### 3. Designated Initializers（指定初始化器）

**文件**: `src/main_cpp20.cc`

允许按名称初始化结构体成员。

```cpp
BeaconConfigBuilder builder{
    .server_address = "127.0.0.1",
    .server_port = 8080,
    .sleep_interval = 3s,
    .jitter_percent = 25,
    .user_agent = "AdaptixBeacon/2.0-cpp20",
    .beacon_id = "beacon-cpp20-001"
};
```

**优势**:
- ✅ 代码更易读
- ✅ 可以跳过某些成员（使用默认值）
- ✅ 成员顺序必须匹配，提高安全性
- ✅ 避免构造函数参数过多

### 4. User-defined Literals for Chrono（时间字面量）

**文件**: `src/main_cpp20.cc`

```cpp
using namespace std::chrono_literals;

auto sleep_interval = 3s;          // 3秒
auto run_duration = 10s;           // 10秒
auto short_sleep = 500ms;          // 500毫秒
```

**优势**:
- ✅ 代码更简洁
- ✅ 类型安全
- ✅ 避免单位转换错误

### 5. [[nodiscard]] 属性增强

**文件**: `src/main_cpp20.cc`

```cpp
[[nodiscard]] auto Build() const -> std::shared_ptr<BeaconConfig> {
  // 编译器会警告未使用返回值
  return config;
}
```

**优势**:
- ✅ 防止忽略重要返回值
- ✅ 提高代码安全性
- ✅ 在C++20中可以添加原因字符串

### 6. Ranges（范围库）

虽然当前实现中未完全使用，但C++20的ranges为未来扩展提供了可能：

```cpp
#include <ranges>

// 示例：处理任务列表
auto active_tasks = tasks 
    | std::views::filter([](const Task& t) { 
        return t.status() == TaskStatus::kRunning; 
      })
    | std::views::transform([](const Task& t) { 
        return t.id(); 
      });
```

**优势**:
- ✅ 延迟求值
- ✅ 组合式操作
- ✅ 更函数式的代码风格
- ✅ 避免临时容器

## C++20 vs C++17 对比

### 代码可读性

**C++17**:
```cpp
template <typename T>
typename std::enable_if_t<
    std::is_base_of_v<Transport, T>, void>
RunBeacon(std::unique_ptr<T> transport) {
    // ...
}
```

**C++20**:
```cpp
template <Transportable T>
void RunBeacon(std::unique_ptr<T> transport) {
    // ...
}
```

### 输出格式化

**C++17**:
```cpp
std::cout << "Server: " << address << ":" << port << "\n";
// 或
printf("Server: %s:%d\n", address.c_str(), port);
```

**C++20**:
```cpp
std::cout << std::format("Server: {}:{}\n", address, port);
```

### 结构体初始化

**C++17**:
```cpp
Config config;
config.address = "127.0.0.1";
config.port = 8080;
config.timeout = 30s;
```

**C++20**:
```cpp
Config config{
    .address = "127.0.0.1",
    .port = 8080,
    .timeout = 30s
};
```

## 编译示例

### 标准编译
```bash
cd modern_cpp_beacon
mkdir build && cd build
cmake ..
make
```

### 运行C++17版本
```bash
./beacon_agent
```

### 运行C++20版本
```bash
./beacon_agent_cpp20
```

输出对比：

**C++17版本** (beacon_agent):
```
Beacon stopped.
```

**C++20版本** (beacon_agent_cpp20):
```
╔═══════════════════════════════════════╗
║   Adaptix Beacon Agent (C++20)        ║
║   Modern C++ Implementation           ║
╚═══════════════════════════════════════╝

[Configuration]
  Server:   127.0.0.1:8080
  Sleep:    3000ms (jitter: 25%)
  Agent:    AdaptixBeacon/2.0-cpp20
  ID:       beacon-cpp20-001

[Status] Starting beacon...
[Status] Beacon is running
[Status] Running for 10 seconds

[Status] Stopping beacon...
[Status] Beacon stopped.
```

## 编译器兼容性

### GCC

| 版本 | C++20支持 | Concepts | std::format | 推荐 |
|------|----------|----------|-------------|------|
| GCC 10 | 部分 | ✅ | ❌ | ⚠️ |
| GCC 11 | 大部分 | ✅ | ❌ | ⚠️ |
| GCC 12 | 几乎完整 | ✅ | ❌ | ⚠️ |
| GCC 13 | 完整 | ✅ | ✅ | ✅ |

### Clang

| 版本 | C++20支持 | Concepts | std::format | 推荐 |
|------|----------|----------|-------------|------|
| Clang 10 | 部分 | ✅ | ❌ | ❌ |
| Clang 12 | 大部分 | ✅ | ❌ | ⚠️ |
| Clang 14 | 几乎完整 | ✅ | ❌ | ⚠️ |
| Clang 15+ | 完整 | ✅ | ✅ | ✅ |

### MSVC

| 版本 | C++20支持 | Concepts | std::format | 推荐 |
|------|----------|----------|-------------|------|
| VS 2019 16.11 | 大部分 | ✅ | ❌ | ⚠️ |
| VS 2022 17.0+ | 完整 | ✅ | ✅ | ✅ |

## 注意事项

### std::format 可用性

`std::format` 在一些编译器中尚未完全实现。如果编译失败，可以：

1. **使用 {fmt} 库**（std::format的基础）:
```cmake
find_package(fmt REQUIRED)
target_link_libraries(beacon_agent_cpp20 PRIVATE fmt::fmt)
```

2. **条件编译**:
```cpp
#if __has_include(<format>)
#include <format>
namespace output = std;
#else
#include <fmt/format.h>
namespace output = fmt;
#endif
```

3. **回退到iostream**:
```cpp
#ifdef __cpp_lib_format
  std::cout << std::format("Server: {}:{}\n", addr, port);
#else
  std::cout << "Server: " << addr << ":" << port << "\n";
#endif
```

### Concepts 编译错误

如果遇到concepts相关的编译错误：

1. 确保使用 `-std=c++20` 或 `-std=c++2a`
2. 检查编译器版本是否支持
3. 可以使用传统的SFINAE作为后备

## 未来C++20特性集成计划

### 1. Coroutines（协程）
```cpp
Task<TaskResult> ExecuteAsync(const Task& task) {
  co_await transport_->SendTask(task);
  auto result = co_await WaitForResult();
  co_return result;
}
```

### 2. Modules（模块）
```cpp
// beacon.cppm
export module adaptix.beacon;
export class Beacon { ... };
```

### 3. std::span
```cpp
void ProcessTasks(std::span<const Task> tasks) {
  for (const auto& task : tasks) {
    executor_->Execute(task);
  }
}
```

### 4. constexpr 增强
```cpp
constexpr auto ComputeJitter(int base, int percent) {
  return base * percent / 100;
}
```

## CI/CD 配置

### GitHub Actions 示例

```yaml
name: C++20 Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install GCC 13
      run: |
        sudo add-apt-repository ppa:ubuntu-toolchain-r/test
        sudo apt-get update
        sudo apt-get install -y gcc-13 g++-13
    
    - name: Configure
      run: |
        mkdir build && cd build
        cmake .. -DCMAKE_CXX_COMPILER=g++-13
    
    - name: Build
      run: cmake --build build -j$(nproc)
    
    - name: Test
      run: |
        cd build/modern_cpp_beacon
        ./beacon_agent_cpp20
```

### Docker 示例

```dockerfile
FROM gcc:13

WORKDIR /app
COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc)

CMD ["./build/modern_cpp_beacon/beacon_agent_cpp20"]
```

## 总结

C++20带来了显著的改进：

✅ **Concepts**: 更好的模板约束
✅ **std::format**: 现代化的格式化输出
✅ **Designated Initializers**: 更清晰的初始化
✅ **Ranges**: 强大的数据处理管道
✅ **更好的编译时计算**: consteval, constinit

项目已经成功升级到C++20，所有代码都能在支持C++20的编译器上正常编译和运行！

## 参考资源

- [C++20 Reference](https://en.cppreference.com/w/cpp/20)
- [C++ Compiler Support](https://en.cppreference.com/w/cpp/compiler_support/20)
- [{fmt} Library](https://github.com/fmtlib/fmt)
- [C++20 Concepts](https://en.cppreference.com/w/cpp/language/constraints)
