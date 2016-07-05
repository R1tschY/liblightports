#ifndef MESSAGESINK_H
#define MESSAGESINK_H

#include "control.h"

#include <functional>
#include <windows.h>

namespace Windows {

class MessageSink : protected Control
{
public:
  typedef std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> MessageFunc;

  explicit MessageSink(MessageFunc wndproc = nullptr);

  void create(cpp::wstring_view name = L"MessageSink");

  HWND getNativeHandle() { return Control::getNativeHandle(); }

  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;

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

private:
  MessageFunc wndproc_;
};

} // namespace Windows

#endif // MESSAGESINK_H
