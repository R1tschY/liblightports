#ifndef MESSAGESINK_H
#define MESSAGESINK_H

#include "control.h"

#include <functional>
#include <windows.h>

namespace Windows {

constexpr Window MessageOnlyHWnd = Window(HWND_MESSAGE);

class MessageSink : protected Control
{
public:
  explicit MessageSink();

  explicit MessageSink(const wchar_t* window_class) :
    Control(window_class, WS_POPUP, WS_EX_TOOLWINDOW)
  { }

  explicit MessageSink(ATOM window_class) :
    Control(window_class, WS_POPUP, WS_EX_TOOLWINDOW)
  { }

  void create(cpp::wstring_view name = L"MessageSink");

  HWND getHWND() { return Control::getHWND(); }

  using Control::destroy;

  static Window find(cpp::wstring_view class_name, cpp::wstring_view window_name)
  {
    return MessageOnlyHWnd.findChild(class_name, window_name);
  }

  static
  Internal::FindWindowRange
  findAll(cpp::wstring_view class_name, cpp::wstring_view window_name)
  {
    return MessageOnlyHWnd.findChilds(class_name, window_name);
  }
};

} // namespace Windows

#endif // MESSAGESINK_H
