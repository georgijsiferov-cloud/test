#ifndef ADAPTIX_BEACON_BEACON_H_
#define ADAPTIX_BEACON_BEACON_H_

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <random>
#include <thread>

#include "adaptix/beacon/beacon_config.h"
#include "adaptix/beacon/command_executor.h"
#include "adaptix/beacon/transport.h"

namespace adaptix {
namespace beacon {

class Beacon {
 public:
  Beacon(std::shared_ptr<BeaconConfig> config,
         std::unique_ptr<Transport> transport,
         std::unique_ptr<CommandExecutor> command_executor);
  ~Beacon();

  Beacon(const Beacon&) = delete;
  Beacon& operator=(const Beacon&) = delete;
  Beacon(Beacon&&) = delete;
  Beacon& operator=(Beacon&&) = delete;

  void Start();
  void Stop();

  bool is_running() const { return is_running_.load(); }

 private:
  void RunLoop();
  std::chrono::milliseconds CalculateSleepInterval();

  std::shared_ptr<BeaconConfig> config_;
  std::unique_ptr<Transport> transport_;
  std::unique_ptr<CommandExecutor> command_executor_;

  std::atomic<bool> is_running_{false};
  std::thread worker_thread_;

  mutable std::mutex mutex_;
  std::condition_variable stop_condition_;

  std::mt19937 random_generator_;
  std::uniform_int_distribution<int> jitter_distribution_;
};

}  // namespace beacon
}  // namespace adaptix

#endif  // ADAPTIX_BEACON_BEACON_H_
