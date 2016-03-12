#ifndef TRAYICON_H
#define TRAYICON_H

#include <cpp-utils/strings/string_view.h>
#include <windows.h>
#include <shellapi.h>
#include <functional>

#include "../core/macros.h"
#include "icon.h"

namespace Windows {

class TrayIcon {
  DISALLOW_COPY_AND_ASSIGN(TrayIcon);

public:
  enum class SystemIcon {
    None    = NIIF_NONE,
    Info    = NIIF_INFO,
    Warning = NIIF_WARNING,
    Error   = NIIF_ERROR
  };

  static const unsigned int MessageId = WM_USER + 21;
  using MessageHandler = std::function<int(UINT,int,int)>;

  TrayIcon();
  ~TrayIcon();

  void add(HWND hwnd, const Icon& icon, cpp::wstring_view tooltip);
  void remove();

  void setIcon(const Icon& icon);
  void setToolTip(cpp::wstring_view src);
  void showBalloon(cpp::wstring_view title,
                   cpp::wstring_view msg,
                   SystemIcon icontype);

  LRESULT handleMessage(WPARAM wparam, LPARAM lparam, MessageHandler handler);

private:
  NOTIFYICONDATAW trayicon_;
  bool added_;

  static int LastId;
};

} // namespace Windows

#endif // TRAYICON_H
