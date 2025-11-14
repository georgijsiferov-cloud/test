#include "adaptix/beacon/websocket_transport.h"

#include <iostream>
#include <sstream>
#include <string>

namespace adaptix {
namespace beacon {

WebSocketTransport::WebSocketTransport(std::shared_ptr<BeaconConfig> config)
    : config_(std::move(config)), 
      handshake_completed_(false),
      retry_count_(0) {}

bool WebSocketTransport::EnsureHandshake() {
  if (handshake_completed_) {
    return true;
  }

  if (PerformHandshake()) {
    handshake_completed_ = true;
    return true;
  }

  return false;
}

bool WebSocketTransport::PerformHandshake() {
  std::cout << "[WebSocket] Performing handshake with " 
            << config_->server_address() << ":" << config_->server_port() 
            << std::endl;

  if (!config_->profile_data().empty()) {
    std::cout << "[WebSocket] Using profile data (" 
              << config_->profile_data().size() << " bytes)" << std::endl;
  }

  return true;
}

std::optional<Task> WebSocketTransport::ReceiveTask() {
  if (!EnsureHandshake()) {
    std::cerr << "[WebSocket] Handshake failed, cannot receive tasks" << std::endl;
    return std::nullopt;
  }

  if (retry_count_ >= config_->max_retries()) {
    return std::nullopt;
  }

  ++retry_count_;

  if (retry_count_ == 3) {
    retry_count_ = 0;
    Task task("ws-task-001", TaskType::kCommand, "echo 'WebSocket task'");
    return task;
  }

  return std::nullopt;
}

bool WebSocketTransport::SendResult(const TaskResult& result) {
  if (!EnsureHandshake()) {
    std::cerr << "[WebSocket] Handshake failed, cannot send result" << std::endl;
    return false;
  }

  std::ostringstream ss;
  ss << "{\"task_id\":\"" << result.task_id() 
     << "\",\"status\":\"" << static_cast<int>(result.status())
     << "\",\"output\":\"" << result.output() << "\"}";

  std::cout << "[WebSocket] Sending result: " << ss.str() << std::endl;

  return true;
}

}  // namespace beacon
}  // namespace adaptix
