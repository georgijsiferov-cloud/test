#include "adaptix/beacon/beacon_config.h"

#include <algorithm>
#include <utility>

namespace adaptix {
namespace beacon {

BeaconConfig::BeaconConfig()
    : server_address_("localhost"),
      server_port_(8080),
      sleep_interval_(std::chrono::seconds(5)),
      jitter_percent_(0),
      protocol_(TransportProtocol::kHttp),
      encryption_(EncryptionType::kNone),
      user_agent_("Mozilla/5.0"),
      beacon_id_(""),
      max_retries_(3),
      profile_data_({}) {}

void BeaconConfig::set_server_address(const std::string& address) {
  server_address_ = address;
}

void BeaconConfig::set_server_port(uint16_t port) {
  server_port_ = port;
}

void BeaconConfig::set_sleep_interval(std::chrono::milliseconds interval) {
  sleep_interval_ = interval;
}

void BeaconConfig::set_jitter_percent(uint8_t jitter) {
  jitter_percent_ = std::min(jitter, static_cast<uint8_t>(100));
}

void BeaconConfig::set_protocol(TransportProtocol protocol) {
  protocol_ = protocol;
}

void BeaconConfig::set_encryption(EncryptionType encryption) {
  encryption_ = encryption;
}

void BeaconConfig::set_user_agent(const std::string& user_agent) {
  user_agent_ = user_agent;
}

void BeaconConfig::set_beacon_id(const std::string& id) {
  beacon_id_ = id;
}

void BeaconConfig::set_max_retries(uint32_t retries) {
  max_retries_ = retries;
}

void BeaconConfig::set_profile_data(std::vector<std::uint8_t> profile) {
  profile_data_ = std::move(profile);
}

bool BeaconConfig::Validate() const {
  if (server_address_.empty()) {
    return false;
  }
  if (server_port_ == 0) {
    return false;
  }
  if (sleep_interval_.count() <= 0) {
    return false;
  }
  if (jitter_percent_ > 100) {
    return false;
  }
  return true;
}

}  // namespace beacon
}  // namespace adaptix
