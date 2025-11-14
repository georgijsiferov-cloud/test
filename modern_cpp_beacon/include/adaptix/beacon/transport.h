#ifndef ADAPTIX_BEACON_TRANSPORT_H_
#define ADAPTIX_BEACON_TRANSPORT_H_

#include <optional>
#include <string>

#include "adaptix/beacon/task.h"

namespace adaptix {
namespace beacon {

class Transport {
 public:
  Transport() = default;
  virtual ~Transport() = default;

  Transport(const Transport&) = delete;
  Transport& operator=(const Transport&) = delete;
  Transport(Transport&&) = delete;
  Transport& operator=(Transport&&) = delete;

  virtual std::optional<Task> ReceiveTask() = 0;
  virtual bool SendResult(const TaskResult& result) = 0;
};

}  // namespace beacon
}  // namespace adaptix

#endif  // ADAPTIX_BEACON_TRANSPORT_H_
