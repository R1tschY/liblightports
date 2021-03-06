#include "control.h"

#include <lightports/user/application.h>
#include <cpp-utils/likely.h>
#include <algorithm>


namespace Windows {

//
// Window class

ATOM Control::registerClass(
    cpp::wstring_view class_name,
    UINT style,
    HICON icon,
    HCURSOR cursor,
    HBRUSH background,
    cpp::wstring_view menu,
    HICON small_icon)
{
  WNDCLASSEX wcex = { };

  wcex.cbSize        = sizeof(WNDCLASSEX);
  wcex.style		     = style;
  wcex.lpfnWndProc	 = &Control::MessageEntry;
  wcex.cbClsExtra	   = 0;
  wcex.cbWndExtra	   = 0;
  wcex.hInstance	   = Application::getHINSTANCE();
  wcex.hIcon		     = icon;
  wcex.hIconSm		   = small_icon;
  wcex.hCursor		   = cursor;
  wcex.hbrBackground = background;
  wcex.lpszClassName = class_name.data();
  wcex.lpszMenuName	 = menu.data();

  return win_throw_on_fail(RegisterClassEx(&wcex));
}

ATOM Control::getWindowClass() {
  static ATOM window_class = registerClass(
                               getWindowClassName().c_str(),
                               CS_HREDRAW | CS_VREDRAW,
                               0,
                               LoadCursor(nullptr, IDC_ARROW),
                               (HBRUSH)(COLOR_WINDOW+1),
                               cpp::wstring_view(),
                               0);
  return window_class;
}

const std::wstring& Control::getWindowClassName() {
  static const std::wstring window_class_name = Application::getName().to_string() + L" Window";
  return window_class_name;
}

//
// ctors/dtor

Control::Control(Control&& other) :
  handle_(std::move(other.handle_)), parent_(other.parent_), class_(other.class_),
  style_(other.style_), exstyle_(other.exstyle_)
{
  if (handle_) {
    SetWindowLongPtr(handle_.get(), GWLP_USERDATA, (LONG_PTR)this);
  }
}

Control::~Control()
{ }

Control& Control::operator=(Control&& other)
{
  handle_ = std::move(other.handle_);
  parent_ = other.parent_;
  class_ = other.class_;
  style_ = other.style_;
  exstyle_ = other.exstyle_;

  if (handle_) {
    SetWindowLongPtr(handle_.get(), GWLP_USERDATA, (LONG_PTR)this);
  }
  return *this;
}

// base functions

void Control::create(
    HWND parent,
    cpp::wstring_view title,
    int x, int y, int width, int height)
{
  cpp_assert(!handle_);

  // handle_ is set in Control::MessageEntry
  win_throw_on_fail(CreateWindowExW(
        // Optional window styles
        exstyle_,

        // Window class
        class_,

        // Window text
        title.data(),

        // Window style
        style_,

        // Size and position
        x, y,
        width, height,

        // Parent window
        parent,

        // Menu
        nullptr,

        // Instance handle
        Application::getHINSTANCE(),

        // Additional application data
        this));
}

void Control::destroy()
{
  handle_.reset();
}

void Control::setTopmost() {
  exstyle_ |= WS_EX_TOPMOST;
  if (handle_) {
    win_print_on_fail(SetWindowPos(getHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE));
  }
}

namespace Internal {

void ControlCommonBase::setPosition(HWND hwnd, const Rectangle& rect)
{
  win_throw_on_fail(::SetWindowPos(
    hwnd,
    nullptr,
    rect.getX(), rect.getY(),
    rect.getWidth(), rect.getHeight(),
    SWP_NOACTIVATE | SWP_NOZORDER));
}

std::wstring ControlCommonBase::getClassName(HWND hwnd)
{
  wchar_t buffer[1024]; // TODO
  ::SetLastError(0);
  int chars = ::GetClassNameW(hwnd, buffer, cpp::length(buffer));
  win_print_on_fail(chars > 0 || ::GetLastError() == 0);
  return std::wstring(buffer, chars);
}

std::wstring ControlCommonBase::getWindowText(HWND hwnd)
{
  wchar_t buffer[1024]; // TODO
  ::SetLastError(0);
  int chars = ::GetWindowTextW(hwnd, buffer, cpp::length(buffer)); // TODO: SendMessage(MSG_GETTEXT, ...
  win_print_on_fail(chars > 0 || ::GetLastError() == 0);
  return std::wstring(buffer, chars);
}

} // namespace Internal

//
// message handling

LRESULT Control::onMessage(UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_NCCREATE:
    return TRUE;

  case WM_CREATE:
    onCreate();
    return 0;

  case WM_DESTROY:
    onDestroy();
    return 0;

  case WM_NCDESTROY:
    handle_.release();
    return 0;

  default:
    return DefWindowProc(handle_.get(), msg, wparam, lparam);
  }
}

void Control::onCreate()
{ }

void Control::onDestroy()
{ }

LRESULT CALLBACK Control::MessageEntry(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
  auto window = reinterpret_cast<Control*>(GetWindowLongPtr(handle, GWLP_USERDATA));
  if (cpp_unlikely(!window))
  {
    if (cpp_likely(msg == WM_NCCREATE))
    {
      auto create_struct = reinterpret_cast<CREATESTRUCT*>(lparam);
      window = static_cast<Control*>(create_struct->lpCreateParams);
      cpp_assert(window);

      SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
      window->handle_.reset(handle);
    }

    if (cpp_unlikely(!window))
    {
      return DefWindowProc(handle, msg, wparam, lparam);
    }
  }

  return Application::runSafe([=](){ return window->onMessage(msg, wparam, lparam); }, 0);
}

} // namespace Windows
