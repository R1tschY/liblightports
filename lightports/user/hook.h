#pragma once

#include <windows.h>
#include <lightports/core/memory.h>
#include <lightports/core/debug.h>

namespace Windows {

struct HHOOKDeleter {
  typedef HHOOK pointer;

  void operator()(HHOOK ptr)
  { ::UnhookWindowsHookEx(ptr); }
};
using HookHandle = ManagedHandle<HHOOKDeleter>;


class Hook {
public:
  static const DWORD AllThreads = 0;

  enum {
    Cbt = WH_CBT,
    MouseLowLevel = WH_MOUSE_LL,
    Mouse = WH_MOUSE,
  };

  void create(HINSTANCE hinstance, int hook_id, DWORD thread_id, HOOKPROC func)
  {
    hook_ = HookHandle(win_throw_on_fail(
      ::SetWindowsHookExW(hook_id, func, hinstance, thread_id)
    ));
  }

  void destroy()
  {
    hook_ = nullptr;
  }

  bool isActive() const { return hook_ != nullptr; }
  HHOOK getHHOOK() const { return hook_.get(); }

  LRESULT callNext(int code, WPARAM wparam, LPARAM lparam) const {
    return ::CallNextHookEx(hook_.get(), code, wparam, lparam);
  }

private:
  HookHandle hook_;
};

using GetHookProc = Hook& (*)();
using ManagedHookProc = LRESULT (*)(Hook& hook, int code, WPARAM wparam, LPARAM lparam);

template<GetHookProc HookGetter, ManagedHookProc Proc>
LRESULT CALLBACK hookProc(int code, WPARAM wparam, LPARAM lparam)
{
  Hook& hook = HookGetter();
  if (code >= 0)
  {
    try
    {
      return Proc(hook, code, wparam, lparam);
    }
    catch (...)
    {
      ::OutputDebugStringW(L"exception in hook function");
    }
  }

  return hook.callNext(code, wparam, lparam);
}

} // namespace Windows


