#include "adaptix/beacon/task.h"

#include <chrono>
#include <utility>

namespace adaptix {
namespace beacon {

Task::Task(std::string id, TaskType type, std::string payload)
    : id_(std::move(id)),
      type_(type),
      payload_(std::move(payload)),
      status_(TaskStatus::kPending),
      created_at_(std::chrono::system_clock::now()) {}

void Task::set_status(TaskStatus status) {
  status_ = status;
}

void Task::AddMetadata(const std::string& key, const std::string& value) {
  metadata_[key] = value;
}

TaskResult::TaskResult()
    : task_id_(""),
      status_(TaskStatus::kPending),
      output_(""),
      finished_at_(std::chrono::system_clock::now()),
      error_message_(std::nullopt) {}

TaskResult::TaskResult(std::string task_id, TaskStatus status, std::string output)
    : task_id_(std::move(task_id)),
      status_(status),
      output_(std::move(output)),
      finished_at_(std::chrono::system_clock::now()),
      error_message_(std::nullopt) {}

void TaskResult::set_error_message(const std::string& message) {
  error_message_ = message;
}

}  // namespace beacon
}  // namespace adaptix
