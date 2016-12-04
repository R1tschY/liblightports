#ifndef TRAYICON_H
#define TRAYICON_H

#include <cpp-utils/strings/string_view.h>
#include <windows.h>
#include <shellapi.h>
#include <functional>

#include "../core/macros.h"
#include "../user/geometry.h"
#include "../user/icon.h"

namespace Windows {

///
/// \sa
/// https://msdn.microsoft.com/en-us/library/cc144179(v=vs.85).aspx
/// https://msdn.microsoft.com/en-us/library/ee330740(v=vs.85).aspx
class TrayIcon {
  DISALLOW_COPY_AND_ASSIGN(TrayIcon);

public:
  enum class SystemIcon {
    None    = NIIF_NONE,
    Info    = NIIF_INFO,
    Warning = NIIF_WARNING,
    Error   = NIIF_ERROR
  };

  static const unsigned int MessageId = WM_APP + 21;
  using MessageHandler = std::function<int(UINT,Point)>;

  TrayIcon();
  ~TrayIcon();

  void add(HWND hwnd);
  void remove();

  void setIcon(IconView icon) { setIcon(icon.getHICON()); }
  void setIcon(int resource_id);
  void setIcon(HICON icon);
  void setToolTip(cpp::wstring_view src);
  void showBalloon(cpp::wstring_view title,
                   cpp::wstring_view msg,
                   SystemIcon icontype);

  LRESULT handleMessage(WPARAM wparam, LPARAM lparam, MessageHandler handler);

private:
  NOTIFYICONDATAW trayicon_;
  bool added_;

  static int LastId;

  void update();
};

} // namespace Windows

#endif // TRAYICON_H
