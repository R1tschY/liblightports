#include "timeout.h"

#include <memory>

#include "../core/debug.h"
#include "../base/charcodecs.h"
#include "../controls/utilwindow.h"

namespace Windows {

Timeout::Timeout(const Callback& callback, int milliseconds):
  callback_(callback), enabled_(false)
{
  if (milliseconds <= 0) return;

  win_throw_on_fail(SetTimer(getCallbackWindow(), (UINT_PTR)this, milliseconds, ccallback));
  enabled_ = true;
}

Timeout::~Timeout() {
  if (enabled_) {
    KillTimer(getCallbackWindow(), (UINT_PTR)&callback_);
  }
}

void Timeout::setInterval(int milliseconds) {
  if (milliseconds <= 0) {
    if (enabled_) {
      KillTimer(getCallbackWindow(), (UINT_PTR)this);
      enabled_ = false;
    }
    return;
  }

  win_throw_on_fail(
        SetTimer(getCallbackWindow(), (UINT_PTR)this, milliseconds, ccallback));
  enabled_ = true;
}

void Timeout::setCallback(const Callback& callback) {
  callback_ = callback;
}

void Timeout::execute(const Callback& callback, int milliseconds) {
  UINT_PTR cb = reinterpret_cast<UINT_PTR>(new Callback(callback));
  win_throw_on_fail(SetTimer(getCallbackWindow(), cb, milliseconds, cexecallback));
}

void CALLBACK Timeout::ccallback(HWND, UINT msg, UINT_PTR callback, DWORD) {
  if (msg == WM_TIMER) {
    auto* timeout = reinterpret_cast<Timeout*>(callback);

    if (!timeout) {
       print(L"timeout callback call failed: callback == NULL\n");
       return;
    }

    try {
      timeout->callback_();
    } catch (const std::bad_function_call& error) {
      print(L"timeout callback call failed: %s\n",
            to_wstring(error.what()).c_str());
    }
  }
}

void CALLBACK Timeout::cexecallback(HWND, UINT msg, UINT_PTR callback, DWORD) {
  if (msg == WM_TIMER) {
    auto cb = std::unique_ptr<Callback>(reinterpret_cast<Callback*>(callback));

    if (!cb) {
       print(L"timeout callback call failed: callback == NULL\n");
       return;
    }

    try {
      (*cb)();
    } catch (const std::bad_function_call& error) {
      print(L"timeout callback call failed: %s\n",
            to_wstring(error.what()).c_str());
    }

    KillTimer(getCallbackWindow(), callback);
  }
}

} // namespace Windows
