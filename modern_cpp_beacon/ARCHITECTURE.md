# Beacon Agent 架构设计

## 概述

本文档描述了现代C++实现的Beacon Agent系统的架构设计和实现细节。

## 系统架构

```
┌─────────────────────────────────────────────────────────────┐
│                         Beacon                              │
│  (主协调器 - 管理整体生命周期)                                │
└────────────┬────────────────────────────────┬────────────────┘
             │                                │
             │ 使用                            │ 使用
             ▼                                ▼
┌────────────────────────┐       ┌──────────────────────────┐
│      Transport         │       │   CommandExecutor        │
│  (抽象传输层接口)        │       │   (命令执行器接口)          │
└────────────┬───────────┘       └────────────┬─────────────┘
             │                                │
             │ 实现                            │ 实现
             ▼                                ▼
┌────────────────────────┐       ┌──────────────────────────┐
│   HttpTransport        │       │ DefaultCommandExecutor   │
│  (HTTP传输实现)         │       │   (默认执行器实现)          │
└────────────────────────┘       └──────────────────────────┘
```

## 核心组件详解

### 1. Beacon (beacon.h/beacon.cc)

**职责**：
- 管理beacon的启动和停止
- 控制主事件循环
- 协调Transport和CommandExecutor
- 实现睡眠间隔和抖动（jitter）机制

**关键特性**：
- 使用 `std::thread` 实现后台运行
- 使用 `std::atomic<bool>` 实现线程安全的状态管理
- 使用 `std::condition_variable` 实现优雅的停止机制
- 使用 `std::mt19937` 随机数生成器实现抖动

**主要方法**：
```cpp
void Start();           // 启动beacon
void Stop();            // 停止beacon
bool is_running();      // 检查运行状态
```

### 2. BeaconConfig (beacon_config.h/beacon_config.cc)

**职责**：
- 存储和管理所有配置参数
- 提供配置验证功能

**配置参数**：
- 服务器地址和端口
- 睡眠间隔和抖动百分比
- 传输协议（HTTP/HTTPS/TCP/命名管道）
- 加密类型
- User-Agent字符串
- Beacon标识符
- 最大重试次数

**特点**：
- 使用 `std::chrono::milliseconds` 表示时间
- 使用 `enum class` 提供类型安全的枚举
- 完全支持拷贝和移动语义

### 3. Task & TaskResult (task.h/task.cc)

**Task类职责**：
- 表示一个待执行的任务
- 包含任务ID、类型、负载和状态
- 支持元数据附加

**TaskResult类职责**：
- 表示任务执行的结果
- 包含任务ID、状态、输出和错误信息
- 使用 `std::optional` 表示可选的错误信息

**任务类型**：
```cpp
enum class TaskType {
  kCommand,        // 系统命令
  kFileDownload,   // 文件下载
  kFileUpload,     // 文件上传
  kSleep,          // 休眠控制
  kScript          // 脚本执行
};
```

**任务状态**：
```cpp
enum class TaskStatus {
  kPending,    // 待处理
  kRunning,    // 运行中
  kCompleted,  // 已完成
  kFailed      // 失败
};
```

### 4. Transport接口 (transport.h)

**职责**：
- 定义通信层的抽象接口
- 从C2服务器接收任务
- 向服务器发送任务结果

**接口方法**：
```cpp
virtual std::optional<Task> ReceiveTask() = 0;
virtual bool SendResult(const TaskResult& result) = 0;
```

**设计特点**：
- 纯虚接口，支持多种传输实现
- 使用 `std::optional` 表示可能失败的接收操作
- 删除拷贝和移动操作（接口类不应被复制）

### 5. HttpTransport (http_transport.h/http_transport.cc)

**职责**：
- 实现基于HTTP/HTTPS的传输
- 处理请求重试逻辑
- 序列化和反序列化任务数据

**当前状态**：
- 模拟实现（生产环境需要实际HTTP库）
- 提供重试机制框架
- 定义序列化/反序列化接口

**未来扩展**：
- 集成libcurl或类似HTTP库
- 实现SSL/TLS证书验证
- 添加请求签名和认证

### 6. CommandExecutor接口 (command_executor.h)

**职责**：
- 定义命令执行的抽象接口
- 提供命令和脚本执行的基础实现

**关键方法**：
```cpp
virtual TaskResult Execute(const Task& task) = 0;
```

**基类提供的辅助方法**：
```cpp
TaskResult ExecuteCommandTask(const Task& task);
TaskResult ExecuteScriptTask(const Task& task, std::string_view interpreter);
```

### 7. DefaultCommandExecutor (command_executor.h/command_executor.cc)

**职责**：
- 实现默认的命令执行逻辑
- 支持多种任务类型

**实现细节**：
- 使用 `popen`/`_popen` 执行系统命令
- 自定义 `FileCloser` 作为 `std::unique_ptr` 的删除器
- 跨平台支持（Windows和Unix/Linux）
- 使用 `std::string_view` 优化字符串传递

## 线程模型

```
主线程                        工作线程
  │                             │
  │ Start()                     │
  ├─────────────────────────────┤
  │                             │ while (running)
  │                             │   ├─ ReceiveTask()
  │                             │   ├─ Execute()
  │                             │   ├─ SendResult()
  │                             │   └─ Sleep (with jitter)
  │                             │
  │ Stop()                      │
  ├─────────────────────────────┤
  │                             │ 退出循环
  │ join()                      │
  └─────────────────────────────┴─ 线程结束
```

## 现代C++特性应用

### 智能指针
```cpp
std::shared_ptr<BeaconConfig> config_;      // 共享配置
std::unique_ptr<Transport> transport_;       // 独占传输层
std::unique_ptr<CommandExecutor> executor_;  // 独占执行器
```

### RAII与自定义删除器
```cpp
struct FileCloser {
  void operator()(FILE* file) const noexcept {
    if (file) pclose(file);
  }
};
std::unique_ptr<FILE, FileCloser> pipe(popen(...));
```

### 可选值
```cpp
std::optional<Task> ReceiveTask();           // 可能没有任务
std::optional<std::string> error_message_;   // 可能没有错误
```

### 强类型枚举
```cpp
enum class TaskType { kCommand, kScript, ... };  // 类型安全
```

### 时间类型
```cpp
std::chrono::milliseconds sleep_interval_;   // 类型安全的时间
std::chrono::system_clock::time_point created_at_;
```

### 线程同步
```cpp
std::atomic<bool> is_running_;               // 原子标志
std::mutex mutex_;                           // 互斥锁
std::condition_variable stop_condition_;     // 条件变量
```

## 错误处理策略

1. **配置验证**：
   - `BeaconConfig::Validate()` 在启动前检查配置
   - 无效配置会阻止beacon启动

2. **任务执行失败**：
   - 返回 `TaskResult` 与 `TaskStatus::kFailed`
   - 使用 `std::optional<std::string>` 提供错误详情

3. **传输失败**：
   - `ReceiveTask()` 返回 `std::nullopt`
   - 支持重试机制（在HttpTransport中实现）

4. **资源管理**：
   - RAII确保资源自动释放
   - 智能指针防止内存泄漏

## 扩展点

### 添加新的传输协议
```cpp
class TcpTransport : public Transport {
 public:
  std::optional<Task> ReceiveTask() override;
  bool SendResult(const TaskResult& result) override;
};
```

### 添加新的任务类型
1. 在 `TaskType` 枚举中添加新类型
2. 在 `DefaultCommandExecutor::Execute()` 中添加处理逻辑
3. 或创建新的 `CommandExecutor` 实现

### 添加加密支持
```cpp
class EncryptedTransport : public Transport {
 private:
  std::unique_ptr<Cipher> cipher_;
};
```

## 性能考虑

1. **移动语义**：避免不必要的拷贝
2. **字符串视图**：使用 `std::string_view` 避免字符串拷贝
3. **预留容器空间**：在适当时使用 `reserve()`
4. **引用传递**：大对象使用 `const&` 传递

## 安全考虑

1. **输入验证**：在执行命令前需要验证（TODO）
2. **权限控制**：需要实现身份认证（TODO）
3. **加密传输**：生产环境必须使用加密（TODO）
4. **审计日志**：添加日志记录执行操作（TODO）

## 测试策略

1. **单元测试**：测试各个组件的独立功能
2. **集成测试**：测试组件间的交互
3. **模拟测试**：使用mock对象测试Transport和Executor
4. **端到端测试**：测试完整的beacon工作流程

## 未来改进

1. **日志系统**：集成结构化日志库（如spdlog）
2. **配置文件**：支持JSON/YAML配置文件
3. **插件系统**：动态加载任务处理器
4. **性能监控**：添加性能指标收集
5. **持久化**：任务队列持久化到磁盘
6. **负载均衡**：支持多C2服务器

## 参考文档

- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [C++17 Standard](https://en.cppreference.com/w/cpp/17)
- [Modern C++ Best Practices](https://github.com/cpp-best-practices/cppbestpractices)
