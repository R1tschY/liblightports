#include "trayicon.h"

#include <algorithm>
#include <windowsx.h>
#include <cpp-utils/algorithm/length.h>
#include "../base/application.h"
#include <lightports/extra/systeminformation.h>
#include <lightports/base/resources.h>

namespace Windows {

int TrayIcon::LastId = 0;

TrayIcon::TrayIcon():
  added_(false)
{
  trayicon_.uFlags = 0;
}

void TrayIcon::add(HWND hwnd)
{
  if (added_)
    return;

  trayicon_.cbSize           = sizeof(NOTIFYICONDATAW);
#if (WINVER < 0x0600)
  trayicon_.uVersion         = NOTIFYICON_VERSION;
  trayicon_.uFlags           |= NIF_MESSAGE;
#else
  trayicon_.uVersion         = NOTIFYICON_VERSION_4;
  trayicon_.uFlags           |= NIF_MESSAGE;
#endif
  trayicon_.hWnd             = hwnd;
  trayicon_.uID              = ++LastId;
  trayicon_.uCallbackMessage = MessageId;

  Shell_NotifyIconW(NIM_ADD, &trayicon_);
  Shell_NotifyIconW(NIM_SETVERSION, &trayicon_);

  trayicon_.uFlags = 0;
  added_ = true;
}

void
TrayIcon::remove()
{
  if (!added_)
    return;

  Shell_NotifyIconW(NIM_DELETE, &trayicon_);

  trayicon_.uFlags = 0;
  added_ = false;
}

TrayIcon::~TrayIcon()
{
  remove();
}

template<std::size_t N>
static void copy_to(cpp::wstring_view src, wchar_t (&dest)[N])
{
  auto size = src.copy(dest, N - 1);
  dest[size] = '\0';
}

void TrayIcon::setToolTip(cpp::wstring_view src)
{
#if (WINVER < 0x0600)
  trayicon_.uFlags |= NIF_TIP;
#else
  trayicon_.uFlags |= NIF_TIP | NIF_SHOWTIP;
#endif
  copy_to(src, trayicon_.szTip);

  update();
}

void TrayIcon::showBalloon(cpp::wstring_view title, cpp::wstring_view msg, SystemIcon icontype)
{
#if (WINVER < 0x0600)
  trayicon_.uFlags |= NIF_INFO;
#else
  trayicon_.uFlags |= NIF_INFO | NIF_SHOWTIP;
#endif
  trayicon_.dwInfoFlags = static_cast<int>(icontype);
  copy_to(title, trayicon_.szInfoTitle);
  copy_to(msg, trayicon_.szInfo);

  update();
}

void TrayIcon::setIcon(int resource_id)
{
  // TODO: use LoadIconMetric if Mingw supports it
  setIcon(
    Resources::getIcon(
      Application::getInstance(),
      resource_id,
      SystemMetrics::getDefaultSmallIconSize()));
}

void TrayIcon::setIcon(HICON icon)
{
  trayicon_.uFlags |= NIF_ICON;
  trayicon_.hIcon = icon;

  update();
}

void TrayIcon::update()
{
  if (!added_)
    return;

  Shell_NotifyIconW(NIM_MODIFY, &trayicon_);
  trayicon_.uFlags = 0;
}

LRESULT TrayIcon::handleMessage(WPARAM wparam, LPARAM lparam, MessageHandler handler)
{
#if (WINVER < 0x0600)
  int id = wparam;
  if (id != trayicon_.uID)
    return 0;

  UINT msg = lparam;
  Point pt = getCursorPosition();
#else
  int id = HIWORD(lparam);
  if (id != trayicon_.uID)
    return 0;

  UINT msg = LOWORD(lparam);
  Point pt(GET_X_LPARAM(wparam), GET_Y_LPARAM(wparam));
#endif

  return handler(msg, pt);
}

} // namespace Windows
