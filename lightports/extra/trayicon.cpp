#include "trayicon.h"

#include <algorithm>
#include <windowsx.h>
#include <cpp-utils/algorithm/length.h>
#include "../base/application.h"

namespace Windows {

int TrayIcon::LastId = 0;

TrayIcon::TrayIcon():
  added_(false)
{}

void
TrayIcon::add(HWND hwnd, const Icon& icon, cpp::wstring_view tooltip) {
    trayicon_.cbSize = sizeof(NOTIFYICONDATAW);
#if (WINVER < 0x0600)
    trayicon_.uVersion         = NOTIFYICON_VERSION;
    trayicon_.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;
#else
    trayicon_.uVersion         = NOTIFYICON_VERSION_4;
    trayicon_.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_SHOWTIP;
#endif
    trayicon_.hWnd             = hwnd;
    trayicon_.uID              = ++LastId;
    trayicon_.uCallbackMessage = MessageId;
    trayicon_.hIcon            = icon.getHICON();

    tooltip.copy(trayicon_.szTip, cpp::length(trayicon_.szTip));

    Shell_NotifyIconW(NIM_ADD, &trayicon_);
    Shell_NotifyIconW(NIM_SETVERSION, &trayicon_);

    added_ = true;
}

void
TrayIcon::remove() {
  added_ = false;
  Shell_NotifyIconW(NIM_DELETE, &trayicon_);
}

TrayIcon::~TrayIcon(void) {
  if (added_) {
    remove();
  }
}

void
TrayIcon::setToolTip(cpp::wstring_view src) {
#if (WINVER < 0x0600)
  trayicon_.uFlags = NIF_TIP;
#else
  trayicon_.uFlags = NIF_TIP | NIF_SHOWTIP;
#endif
  src.copy(trayicon_.szTip, cpp::length(trayicon_.szTip));
  Shell_NotifyIconW(NIM_MODIFY, &trayicon_);
}

void
TrayIcon::showBalloon(cpp::wstring_view title, cpp::wstring_view msg, SystemIcon icontype) {
#if (WINVER < 0x0600)
  trayicon_.uFlags = NIF_INFO;
#else
  trayicon_.uFlags = NIF_INFO | NIF_SHOWTIP;
#endif
  trayicon_.dwInfoFlags = static_cast<int>(icontype);
  title.copy(trayicon_.szInfoTitle, cpp::length(trayicon_.szInfoTitle));
  msg.copy(trayicon_.szInfo, cpp::length(trayicon_.szInfo));

  Shell_NotifyIconW(NIM_MODIFY, &trayicon_);
}

LRESULT TrayIcon::handleMessage(WPARAM wparam, LPARAM lparam, MessageHandler handler)
{
#if (WINVER < 0x0600)
  int id = wparam;
  if (id != trayicon_.uID)
    return 0;

  UINT msg = lparam;
  POINT pt;
  GetCursorPos(&pt);
  int x = pt.x;
  int y = pt.y;
#else
  int id = HIWORD(lparam);
  if (id != trayicon_.uID)
    return 0;

  UINT msg = LOWORD(lparam);
  int x = GET_X_LPARAM(wparam);
  int y = GET_Y_LPARAM(wparam);
#endif

  return handler(msg, x, y);
}

void TrayIcon::setIcon(const Icon& icon)
{
  trayicon_.uFlags = NIF_ICON;
  trayicon_.hIcon = icon.getHICON();
  Shell_NotifyIconW(NIM_MODIFY, &trayicon_);
}

} // namespace Windows
