#ifndef ADAPTIX_BEACON_CONCEPTS_H_
#define ADAPTIX_BEACON_CONCEPTS_H_

#include <concepts>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "adaptix/beacon/task.h"

namespace adaptix {
namespace beacon {

template <typename T>
concept StringLike = std::convertible_to<T, std::string_view>;

template <typename T>
concept Transportable = requires(T t) {
  { t.ReceiveTask() } -> std::same_as<std::optional<Task>>;
  { t.SendResult(std::declval<TaskResult>()) } -> std::same_as<bool>;
};

template <typename T>
concept Executable = requires(T t, Task task) {
  { t.Execute(task) } -> std::same_as<TaskResult>;
};

}  // namespace beacon
}  // namespace adaptix

#endif  // ADAPTIX_BEACON_CONCEPTS_H_
