#ifndef ADAPTIX_BEACON_TASK_H_
#define ADAPTIX_BEACON_TASK_H_

#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>

namespace adaptix {
namespace beacon {

enum class TaskStatus {
  kPending,
  kRunning,
  kCompleted,
  kFailed
};

enum class TaskType {
  kCommand,
  kFileDownload,
  kFileUpload,
  kSleep,
  kScript
};

class Task {
 public:
  Task() = default;
  Task(std::string id, TaskType type, std::string payload);
  ~Task() = default;

  Task(const Task&) = default;
  Task& operator=(const Task&) = default;
  Task(Task&&) noexcept = default;
  Task& operator=(Task&&) noexcept = default;

  const std::string& id() const { return id_; }
  TaskType type() const { return type_; }
  const std::string& payload() const { return payload_; }
  TaskStatus status() const { return status_; }
  std::chrono::system_clock::time_point created_at() const { return created_at_; }
  const std::unordered_map<std::string, std::string>& metadata() const { return metadata_; }

  void set_status(TaskStatus status);
  void AddMetadata(const std::string& key, const std::string& value);

 private:
  std::string id_;
  TaskType type_{TaskType::kCommand};
  std::string payload_;
  TaskStatus status_{TaskStatus::kPending};
  std::chrono::system_clock::time_point created_at_;
  std::unordered_map<std::string, std::string> metadata_;
};

class TaskResult {
 public:
  TaskResult();
  TaskResult(std::string task_id, TaskStatus status, std::string output);
  ~TaskResult() = default;

  TaskResult(const TaskResult&) = default;
  TaskResult& operator=(const TaskResult&) = default;
  TaskResult(TaskResult&&) noexcept = default;
  TaskResult& operator=(TaskResult&&) noexcept = default;

  const std::string& task_id() const { return task_id_; }
  TaskStatus status() const { return status_; }
  const std::string& output() const { return output_; }
  std::chrono::system_clock::time_point finished_at() const { return finished_at_; }
  const std::optional<std::string>& error_message() const { return error_message_; }

  void set_error_message(const std::string& message);

 private:
  std::string task_id_;
  TaskStatus status_{TaskStatus::kPending};
  std::string output_;
  std::chrono::system_clock::time_point finished_at_;
  std::optional<std::string> error_message_;
};

}  // namespace beacon
}  // namespace adaptix

#endif  // ADAPTIX_BEACON_TASK_H_
