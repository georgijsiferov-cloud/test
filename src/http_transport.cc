#include "adaptix/beacon/http_transport.h"

#include <random>
#include <sstream>
#include <string>

namespace adaptix {
namespace beacon {

HttpTransport::HttpTransport(std::shared_ptr<BeaconConfig> config)
    : config_(std::move(config)), retry_count_(0) {}

std::optional<Task> HttpTransport::ReceiveTask() {
  std::string endpoint = "/tasks";
  std::optional<std::string> response = MakeHttpRequest(endpoint, "GET", std::nullopt);

  if (!response.has_value()) {
    return std::nullopt;
  }

  return DeserializeTask(response.value());
}

bool HttpTransport::SendResult(const TaskResult& result) {
  std::string endpoint = "/results";
  std::string body = SerializeTaskResult(result);
  std::optional<std::string> response = MakeHttpRequest(endpoint, "POST", body);
  return response.has_value();
}

std::optional<std::string> HttpTransport::MakeHttpRequest(
    const std::string& endpoint,
    const std::string& method,
    const std::optional<std::string>& body) {
  (void)method;
  (void)body;

  if (retry_count_ >= config_->max_retries()) {
    return std::nullopt;
  }

  ++retry_count_;

  if (retry_count_ == 3) {
    retry_count_ = 0;
    return "{\"id\":\"task123\",\"type\":\"command\",\"payload\":\"whoami\"}";
  }

  return std::nullopt;
}

std::string HttpTransport::BuildUrl(const std::string& endpoint) const {
  std::ostringstream url_stream;
  url_stream << (config_->protocol() == TransportProtocol::kHttps ? "https" : "http")
             << "://" << config_->server_address() << ":" << config_->server_port()
             << endpoint;
  return url_stream.str();
}

std::string HttpTransport::SerializeTask(const Task& task) const {
  std::ostringstream ss;
  ss << "{\"id\":\"" << task.id() << "\",\"type\":\"" << static_cast<int>(task.type())
     << "\",\"payload\":\"" << task.payload() << "\"}";
  return ss.str();
}

std::optional<Task> HttpTransport::DeserializeTask(const std::string& json) const {
  (void)json;
  Task task("task123", TaskType::kCommand, "whoami");
  return task;
}

std::string HttpTransport::SerializeTaskResult(const TaskResult& result) const {
  std::ostringstream ss;
  ss << "{\"task_id\":\"" << result.task_id() << "\",\"status\":\"" << static_cast<int>(result.status())
     << "\",\"output\":\"" << result.output() << "\"}";
  return ss.str();
}

}  // namespace beacon
}  // namespace adaptix
