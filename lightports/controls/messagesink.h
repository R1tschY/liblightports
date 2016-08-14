#ifndef MESSAGESINK_H
#define MESSAGESINK_H

#include "control.h"

#include <functional>
#include <windows.h>

namespace Windows {

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

  HWND getNativeHandle() { return Control::getNativeHandle(); }

  using Control::getNativeHandle;
  using Control::destroy;

  static HWND find(cpp::wstring_view class_name, cpp::wstring_view window_name)
  {
    return Control::find(HWND_MESSAGE, class_name, window_name);
  }

  static
  cpp::iterator_range<FindWindowIterator, FindWindowIteratorSentinel>
  findAll(cpp::wstring_view class_name, cpp::wstring_view window_name)
  {
    return Control::findAll(HWND_MESSAGE, class_name, window_name);
  }
};

} // namespace Windows

#endif // MESSAGESINK_H
