#ifndef ADAPTIX_BEACON_WEBSOCKET_TRANSPORT_H_
#define ADAPTIX_BEACON_WEBSOCKET_TRANSPORT_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/transport.h"

namespace adaptix {
namespace beacon {

class WebSocketTransport : public Transport {
 public:
  explicit WebSocketTransport(std::shared_ptr<BeaconConfig> config);
  ~WebSocketTransport() override = default;

  std::optional<Task> ReceiveTask() override;
  bool SendResult(const TaskResult& result) override;

 private:
  bool EnsureHandshake();
  bool PerformHandshake();

  std::shared_ptr<BeaconConfig> config_;
  bool handshake_completed_{false};
  uint32_t retry_count_{0};
};

}  // namespace beacon
}  // namespace adaptix

#endif  // ADAPTIX_BEACON_WEBSOCKET_TRANSPORT_H_
