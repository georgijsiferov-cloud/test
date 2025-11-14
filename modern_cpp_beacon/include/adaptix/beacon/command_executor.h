#ifndef ADAPTIX_BEACON_COMMAND_EXECUTOR_H_
#define ADAPTIX_BEACON_COMMAND_EXECUTOR_H_

#include <memory>
#include <string>
#include <string_view>

#include "adaptix/beacon/task.h"

namespace adaptix {
namespace beacon {

class CommandExecutor {
 public:
  CommandExecutor() = default;
  virtual ~CommandExecutor() = default;

  CommandExecutor(const CommandExecutor&) = delete;
  CommandExecutor& operator=(const CommandExecutor&) = delete;
  CommandExecutor(CommandExecutor&&) = delete;
  CommandExecutor& operator=(CommandExecutor&&) = delete;

  virtual TaskResult Execute(const Task& task) = 0;

 protected:
  TaskResult ExecuteCommandTask(const Task& task);
  TaskResult ExecuteScriptTask(const Task& task, std::string_view interpreter);

 private:
  TaskResult RunCommand(std::string_view command,
                        const std::string& task_id,
                        std::string_view error_message);
};

class DefaultCommandExecutor : public CommandExecutor {
 public:
  DefaultCommandExecutor() = default;
  ~DefaultCommandExecutor() override = default;

  TaskResult Execute(const Task& task) override;

 private:
  TaskResult HandleFileDownload(const Task& task);
  TaskResult HandleFileUpload(const Task& task);
  TaskResult HandleSleep(const Task& task);
};

}  // namespace beacon
}  // namespace adaptix

#endif  // ADAPTIX_BEACON_COMMAND_EXECUTOR_H_
