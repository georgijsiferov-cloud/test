#include "adaptix/beacon/command_executor.h"

#include <array>
#include <cstdio>
#include <memory>
#include <sstream>
#include <string>

namespace adaptix {
namespace beacon {
namespace {

struct FileCloser {
  void operator()(FILE* file) const noexcept {
    if (file == nullptr) {
      return;
    }
#ifdef _WIN32
    _pclose(file);
#else
    pclose(file);
#endif
  }
};

}  // namespace

TaskResult CommandExecutor::RunCommand(std::string_view command,
                                        const std::string& task_id,
                                        std::string_view error_message) {
  std::array<char, 128> buffer;
  std::string output;
  const std::string command_string(command);

#ifdef _WIN32
  std::unique_ptr<FILE, FileCloser> pipe(_popen(command_string.c_str(), "r"));
#else
  std::unique_ptr<FILE, FileCloser> pipe(popen(command_string.c_str(), "r"));
#endif

  if (!pipe) {
    TaskResult result(task_id, TaskStatus::kFailed, "");
    result.set_error_message(std::string(error_message));
    return result;
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    output += buffer.data();
  }

  return TaskResult(task_id, TaskStatus::kCompleted, output);
}

TaskResult CommandExecutor::ExecuteCommandTask(const Task& task) {
  return RunCommand(task.payload(), task.id(), "Failed to execute command");
}

TaskResult CommandExecutor::ExecuteScriptTask(const Task& task,
                                               std::string_view interpreter) {
  std::ostringstream command_stream;
  command_stream << interpreter << " -c \"" << task.payload() << "\"";
  return RunCommand(command_stream.str(), task.id(), "Failed to execute script");
}

TaskResult DefaultCommandExecutor::Execute(const Task& task) {
  switch (task.type()) {
    case TaskType::kCommand:
      return ExecuteCommandTask(task);
    case TaskType::kScript:
      return ExecuteScriptTask(task, "bash");
    case TaskType::kFileDownload:
      return HandleFileDownload(task);
    case TaskType::kFileUpload:
      return HandleFileUpload(task);
    case TaskType::kSleep:
      return HandleSleep(task);
    default: {
      TaskResult result(task.id(), TaskStatus::kFailed, "");
      result.set_error_message("Unknown task type");
      return result;
    }
  }
}

TaskResult DefaultCommandExecutor::HandleFileDownload(const Task& task) {
  TaskResult result(task.id(), TaskStatus::kCompleted, "");
  return result;
}

TaskResult DefaultCommandExecutor::HandleFileUpload(const Task& task) {
  TaskResult result(task.id(), TaskStatus::kCompleted, "");
  return result;
}

TaskResult DefaultCommandExecutor::HandleSleep(const Task& task) {
  TaskResult result(task.id(), TaskStatus::kCompleted, "Sleep command acknowledged");
  return result;
}

}  // namespace beacon
}  // namespace adaptix
