#ifndef ADAPTIX_BEACON_HTTP_TRANSPORT_H_
#define ADAPTIX_BEACON_HTTP_TRANSPORT_H_

#include <memory>
#include <optional>
#include <string>

#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/transport.h"

namespace adaptix {
namespace beacon {

class HttpTransport : public Transport {
 public:
  explicit HttpTransport(std::shared_ptr<BeaconConfig> config);
  ~HttpTransport() override = default;

  std::optional<Task> ReceiveTask() override;
  bool SendResult(const TaskResult& result) override;

 private:
  std::shared_ptr<BeaconConfig> config_;
  uint32_t retry_count_{0};

  std::optional<std::string> MakeHttpRequest(
      const std::string& endpoint,
      const std::string& method,
      const std::optional<std::string>& body);
  
  std::string BuildUrl(const std::string& endpoint) const;
  std::string SerializeTask(const Task& task) const;
  std::optional<Task> DeserializeTask(const std::string& json) const;
  std::string SerializeTaskResult(const TaskResult& result) const;
};

}  // namespace beacon
}  // namespace adaptix

#endif  // ADAPTIX_BEACON_HTTP_TRANSPORT_H_
