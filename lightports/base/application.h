#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <functional>
#include <string>
#include <atomic>
#include "../core/macros.h"
#include "../core/memory.h"
#include "../base/path.h"
#include <cpp-utils/strings/string_view.h>
#include "../core/version.h"
#include <cpp-utils/assert.h>

namespace Windows {

class Application final {
  DISALLOW_COPY_AND_ASSIGN(Application);

public:
  // types
  typedef std::function<int()> ExecuteFunc;

  // ctor
  Application(cpp::wstring_view name, HINSTANCE instance);

  // run
  int run(ExecuteFunc unique_entry, ExecuteFunc duplicate_entry = ExecuteFunc());

  // application properties
  static HINSTANCE getInstance();
  static cpp::wstring_view getName() { return self().name_; }

  // paths
  static std::wstring getExecutablePath();
  static std::wstring getConfigPath();

  // system
  //static WindowsVersion getWindowsVersion() { return winversion; }
  // TODO: static string getWindowsVersionString();

  /// \todo move to Process
  static bool is64BitWindows();

  // messages
  static int processMessages();

  template<typename Func>
  static void runSafe(Func&& func)
  {
    try
    {
      func();
    }
    catch (...)
    {
      handleException();
    }
  }

  template<typename Func, typename T>
  static T runSafe(Func&& func, T valueIfCatched)
  {
    try
    {
      return func();
    }
    catch (...)
    {
      handleException();
      return std::move(valueIfCatched);
    }
  }

  static void handleException();

private:
  Handle mutex_;
  std::atomic<bool> is_running_;

  const HINSTANCE appinstance_;
  const std::wstring name_;

  static Application* instance_;

  static Application& self() { cpp_assert(instance_ != nullptr); return *instance_; }
};

} // namespace Windows

#endif // APPLICATION_H
