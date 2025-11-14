#ifndef ADAPTIX_BEACON_BEACON_CONFIG_H_
#define ADAPTIX_BEACON_BEACON_CONFIG_H_

#include <chrono>
#include <cstdint>
#include <string>
#include <optional>

namespace adaptix {
namespace beacon {

enum class TransportProtocol {
  kHttp,
  kHttps,
  kTcp,
  kNamedPipe
};

enum class EncryptionType {
  kNone,
  kAes256,
  kRsa2048
};

class BeaconConfig {
 public:
  BeaconConfig();
  ~BeaconConfig() = default;

  BeaconConfig(const BeaconConfig&) = default;
  BeaconConfig& operator=(const BeaconConfig&) = default;
  BeaconConfig(BeaconConfig&&) noexcept = default;
  BeaconConfig& operator=(BeaconConfig&&) noexcept = default;

  void set_server_address(const std::string& address);
  std::string server_address() const { return server_address_; }

  void set_server_port(uint16_t port);
  uint16_t server_port() const { return server_port_; }

  void set_sleep_interval(std::chrono::milliseconds interval);
  std::chrono::milliseconds sleep_interval() const { return sleep_interval_; }

  void set_jitter_percent(uint8_t jitter);
  uint8_t jitter_percent() const { return jitter_percent_; }

  void set_protocol(TransportProtocol protocol);
  TransportProtocol protocol() const { return protocol_; }

  void set_encryption(EncryptionType encryption);
  EncryptionType encryption() const { return encryption_; }

  void set_user_agent(const std::string& user_agent);
  std::string user_agent() const { return user_agent_; }

  void set_beacon_id(const std::string& id);
  std::string beacon_id() const { return beacon_id_; }

  void set_max_retries(uint32_t retries);
  uint32_t max_retries() const { return max_retries_; }

  bool Validate() const;

 private:
  std::string server_address_;
  uint16_t server_port_;
  std::chrono::milliseconds sleep_interval_;
  uint8_t jitter_percent_;
  TransportProtocol protocol_;
  EncryptionType encryption_;
  std::string user_agent_;
  std::string beacon_id_;
  uint32_t max_retries_;
};

}  // namespace beacon
}  // namespace adaptix

#endif  // ADAPTIX_BEACON_BEACON_CONFIG_H_
