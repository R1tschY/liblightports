#include "hook.h"

#include "../core/macros.h"
#include "../base/module.h"

#include <cpp-utils/algorithm/erase.h>

namespace Windows {

///////////////////////////////////////////////////////////////////////////////
// Hook

bool Hook::create(int hook_id, DWORD thread_id, HOOKPROC func) {
  if (!hook_) {
    hook_ = SetWindowsHookEx(hook_id, func, Module::getInstance(), thread_id);
    // TODO: log if (hook_ != nullptr)
    return (hook_ != nullptr);
  } else {
    return true;
  }
}

bool Hook::destroy() {
  if (hook_ && UnhookWindowsHookEx(hook_)) {
    hook_ = nullptr;
    return true;
  } else {
    return (hook_ == nullptr);
  }
}

///////////////////////////////////////////////////////////////////////////////
// MouseHook

MouseHook& MouseHook::get() {
  static MouseHook instance;
  return instance;
}

void MouseHook::addHandler(MouseHook::WheelHandler* proc) {
  wheel_hooks_.push_back(proc);
  hook_.create(WH_MOUSE_LL, Hook::AllThreads, hookProc);
}

void MouseHook::removeHandler(MouseHook::WheelHandler* proc) {
  cpp::erase(wheel_hooks_, proc);
  if (empty()) {
    if (!hook_.destroy()) {
      // TODO: log MessageBeep(MB_ICONERROR);
    }
  }
}

void MouseHook::addHandler(MouseHook::ClickHandler* proc) {
  click_hooks_.push_back(proc);
  hook_.create(WH_MOUSE_LL, Hook::AllThreads, hookProc);
}

void MouseHook::removeHandler(MouseHook::ClickHandler* proc) {
  cpp::erase(click_hooks_, proc);
  if (empty()) {
    if (!hook_.destroy()) {
      // TODO: log MessageBeep(MB_ICONERROR);
    }
  }
}

__attribute__((hot))
LRESULT CALLBACK MouseHook::hookProc(int code, WPARAM wparam, LPARAM lparam) {
  if (code != HC_ACTION) {
    return CallNextHookEx(get().hook_.getHandle(), code, wparam, lparam);
  }

  bool processed = false;
  MSLLHOOKSTRUCT* data = reinterpret_cast<MSLLHOOKSTRUCT*>(lparam);

  switch (wparam) {
  case WM_MOUSEWHEEL:
    for (auto& hook : get().wheel_hooks_) {
      if ((*hook)(data->pt, HIWORD(data->mouseData))) processed = true;
    }
    break;

  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    for (auto& hook : get().click_hooks_) {
      if ((*hook)(wparam, data->pt)) processed = true;
    }
    break;
  }

  if (!processed) {
    return CallNextHookEx(get().hook_.getHandle(), code, wparam, lparam);
  } else {
    return 1;
  }
}

} // namespace Windows


