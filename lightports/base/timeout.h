#pragma once

#include <functional>

#include "../core/macros.h"
#include <windows.h>

namespace Windows {

////////////////////////////////////////////////////////////////////////////////
//     Timeout
////////////////////////////////////////////////////////////////////////////////
class Timeout {
  DISALLOW_COPY_AND_ASSIGN(Timeout);

public:
  typedef std::function<void()> Callback;

  Timeout(const Callback& callback, int milliseconds);
  ~Timeout();
  void setInterval(int milliseconds);
  void setCallback(const Callback& callback);

  static void execute(const Callback& callback, int milliseconds);

private:
  Callback callback_;
  bool enabled_;

  static void CALLBACK ccallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
  static void CALLBACK cexecallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
};

} // namespace Windows

