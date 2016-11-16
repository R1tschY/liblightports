#include "hook.h"

#include "../core/macros.h"
#include "../base/module.h"

#include <cpp-utils/algorithm/erase.h>

namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// Hook

bool Hook::create(int hook_id, DWORD thread_id, HOOKPROC func) {
  if (!hook_) {
    hook_ = ::SetWindowsHookExW(hook_id, func, Module::getInstance(), thread_id);
    // TODO: log if (hook_ != nullptr)
    return (hook_ != nullptr);
  } else {
    return true;
  }
}

bool Hook::destroy() {
  if (hook_ && ::UnhookWindowsHookEx(hook_)) {
    hook_ = nullptr;
    return true;
  } else {
    return (hook_ == nullptr);
  }
}

} // namespace Windows


