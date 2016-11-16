#pragma once

#include <functional>
#include <vector>
#include <cassert>
#include <cstdio>
#include <windows.h>

namespace Windows {

class Hook {
public:
  static const DWORD AllThreads = 0;

  bool create(int hook_id, DWORD thread_id, HOOKPROC func);
  bool destroy();

  bool isActive() const { return hook_ != nullptr; }
  HHOOK getHHOOK() const { return hook_; }

  Hook() : hook_(nullptr) { }
  ~Hook() { destroy(); }

  LRESULT callNext(int code, WPARAM wparam, LPARAM lparam) const {
    return ::CallNextHookEx(hook_, code, wparam, lparam);
  }

private:
  HHOOK hook_;
};

} // namespace Windows


