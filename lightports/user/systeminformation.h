#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H

#include <windows.h>

#include "../user/geometry.h"

namespace Windows {

// SystemMetrics

namespace SystemMetrics {

inline
int getMetric(int id) noexcept
{
  return ::GetSystemMetrics(id);
}

inline
Size getDefaultIconSize()
{
  return Size(getMetric(SM_CXICON), getMetric(SM_CYICON));
}

inline
Size getDefaultSmallIconSize()
{
  return Size(getMetric(SM_CXSMICON), getMetric(SM_CYSMICON));
}

} // namespace SystemMetrics

// Monitor

class Monitor {
public:
  // types
  class Info {
  public:
    // ctor
    Info() { info_.cbSize = sizeof(info_); }
    Info(const MONITORINFO& info) : info_(info) { }

    // native
    MONITORINFO& getNativeRef() {
      return info_;
    }

    // properties
    Rectangle getMonitorRect() const { return info_.rcMonitor; }
    Rectangle getWorkRect() const { return info_.rcWork; }
    bool isPrimary() const { return info_.dwFlags == MONITORINFOF_PRIMARY; }

  private:
    MONITORINFO info_;
  };

  // ctors
  Monitor(HMONITOR handle) :
    handle_(handle)
  { }

  static Monitor getPrimary() {
    return Monitor(MonitorFromPoint(POINT{0,0}, MONITOR_DEFAULTTOPRIMARY));
  }

  // properties
  HMONITOR getNativeHandle() { return handle_; }

  Info getInfo() {
    Info info;
    GetMonitorInfo(handle_, &info.getNativeRef());
    return info;
  }

private:
  HMONITOR handle_;
};

} // namespace Windows

#endif // SYSTEMINFORMATION_H
