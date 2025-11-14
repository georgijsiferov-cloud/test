#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>
#include <span>
#include <thread>

#include "adaptix/beacon/beacon.h"
#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/command_executor.h"
#include "adaptix/beacon/concepts.h"
#include "adaptix/beacon/http_transport.h"

using namespace std::chrono_literals;

namespace {

struct BeaconConfigBuilder {
  std::string server_address = "127.0.0.1";
  uint16_t server_port = 8080;
  std::chrono::milliseconds sleep_interval = 3s;
  uint8_t jitter_percent = 25;
  std::string user_agent = "AdaptixBeacon/2.0";
  std::string beacon_id = "beacon-cpp20";

  [[nodiscard]] auto Build() const -> std::shared_ptr<adaptix::beacon::BeaconConfig> {
    auto config = std::make_shared<adaptix::beacon::BeaconConfig>();
    config->set_server_address(server_address);
    config->set_server_port(server_port);
    config->set_sleep_interval(sleep_interval);
    config->set_jitter_percent(jitter_percent);
    config->set_user_agent(user_agent);
    config->set_beacon_id(beacon_id);
    return config;
  }
};

void PrintBanner() {
  std::cout << std::format("╔═══════════════════════════════════════╗\n");
  std::cout << std::format("║   Adaptix Beacon Agent (C++20)        ║\n");
  std::cout << std::format("║   Modern C++ Implementation           ║\n");
  std::cout << std::format("╚═══════════════════════════════════════╝\n");
}

void PrintConfig(const adaptix::beacon::BeaconConfig& config) {
  std::cout << std::format("\n[Configuration]\n");
  std::cout << std::format("  Server:   {}:{}\n", 
                          config.server_address(), 
                          config.server_port());
  std::cout << std::format("  Sleep:    {}ms (jitter: {}%)\n", 
                          config.sleep_interval().count(), 
                          config.jitter_percent());
  std::cout << std::format("  Agent:    {}\n", config.user_agent());
  std::cout << std::format("  ID:       {}\n", config.beacon_id());
}

template <adaptix::beacon::Transportable T, adaptix::beacon::Executable E>
void RunBeacon(std::shared_ptr<adaptix::beacon::BeaconConfig> config,
               std::unique_ptr<T> transport,
               std::unique_ptr<E> executor) {
  adaptix::beacon::Beacon beacon(config, std::move(transport), std::move(executor));
  
  std::cout << std::format("\n[Status] Starting beacon...\n");
  beacon.Start();
  
  if (beacon.is_running()) {
    std::cout << std::format("[Status] Beacon is running\n");
  }
  
  constexpr auto run_duration = 10s;
  std::cout << std::format("[Status] Running for {} seconds\n", 
                          std::chrono::duration_cast<std::chrono::seconds>(run_duration).count());
  
  std::this_thread::sleep_for(run_duration);
  
  std::cout << std::format("\n[Status] Stopping beacon...\n");
  beacon.Stop();
  std::cout << std::format("[Status] Beacon stopped.\n");
}

}  // namespace

int main() {
  PrintBanner();
  
  BeaconConfigBuilder builder{
      .server_address = "127.0.0.1",
      .server_port = 8080,
      .sleep_interval = 3s,
      .jitter_percent = 25,
      .user_agent = "AdaptixBeacon/2.0-cpp20",
      .beacon_id = "beacon-cpp20-001"
  };
  
  auto config = builder.Build();
  PrintConfig(*config);
  
  auto transport = std::make_unique<adaptix::beacon::HttpTransport>(config);
  auto executor = std::make_unique<adaptix::beacon::DefaultCommandExecutor>();
  
  RunBeacon(config, std::move(transport), std::move(executor));
  
  return 0;
}
