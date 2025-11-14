# 更新日志

## [1.1.0] - 2024-11-14

### 新增功能

#### WebSocket传输支持
- ✅ 添加 `WebSocketTransport` 类实现WebSocket协议通信
- ✅ 支持WebSocket握手流程
- ✅ 在 `TransportProtocol` 枚举中添加 `kWebSocket` 选项
- ✅ 创建独立的WebSocket示例程序 `beacon_agent_websocket`

#### Profile二进制配置
- ✅ 在 `BeaconConfig` 中添加 `profile_data_` 成员变量
- ✅ 添加 `set_profile_data()` 和 `profile_data()` 方法
- ✅ 支持从配置获取二进制数组（`std::vector<std::uint8_t>`）
- ✅ WebSocket握手时可以发送profile数据

#### 文档
- ✅ 创建 `WEBSOCKET.md` 详细说明WebSocket使用方法
- ✅ 提供多种Profile数据加载方式示例
- ✅ 更新主README添加WebSocket文档链接

### 技术细节

**新增文件：**
- `include/adaptix/beacon/websocket_transport.h`
- `src/websocket_transport.cc`
- `src/main_websocket.cc`
- `WEBSOCKET.md`

**修改文件：**
- `include/adaptix/beacon/beacon_config.h`：添加profile支持和WebSocket协议
- `src/beacon_config.cc`：实现profile相关方法
- `CMakeLists.txt`：添加WebSocket相关源文件和可执行文件

### 使用示例

```cpp
// 配置WebSocket
auto config = std::make_shared<BeaconConfig>();
config->set_protocol(TransportProtocol::kWebSocket);
config->set_server_address("127.0.0.1");
config->set_server_port(9000);

// 设置profile数据
std::vector<std::uint8_t> profile_data = {
    0x50, 0x4B, 0x03, 0x04,  // 二进制数据
    // ...
};
config->set_profile_data(std::move(profile_data));

// 使用WebSocket传输
auto transport = std::make_unique<WebSocketTransport>(config);
```

### 编译输出

现在项目生成三个可执行文件：
1. `beacon_agent` - 标准HTTP版本
2. `beacon_agent_cpp20` - C++20特性展示版本
3. `beacon_agent_websocket` - WebSocket版本（新增）

---

## [1.0.0] - 2024-11-14

### 初始版本

#### C++20现代化重构
- ✅ 从C/C++混合编程重构为纯现代C++20
- ✅ 完全遵循Google C++代码规范
- ✅ 使用现代C++特性（智能指针、移动语义、enum class等）

#### 核心功能
- ✅ Beacon主控制器
- ✅ HTTP传输实现
- ✅ 命令执行器
- ✅ 任务管理系统
- ✅ 配置管理

#### C++20特性应用
- ✅ Concepts（概念）
- ✅ std::format（部分编译器）
- ✅ Designated Initializers（指定初始化器）
- ✅ 增强的constexpr

#### 文档
- ✅ ARCHITECTURE.md - 架构设计文档
- ✅ EXAMPLES.md - 使用示例
- ✅ CPP20_FEATURES.md - C++20特性说明
- ✅ README.md - 项目说明
- ✅ SUMMARY.md - 重构总结

### 支持的传输协议
- HTTP/HTTPS（示例实现）
- WebSocket（v1.1.0新增）
- TCP（预留）
- Named Pipe（预留）

### 支持的加密类型
- None（无加密）
- AES256（预留）
- RSA2048（预留）
