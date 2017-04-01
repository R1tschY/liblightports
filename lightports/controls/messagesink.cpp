#include "messagesink.h"

#include <cpp-utils/preprocessor.h>
#include <cpp-utils/strings/string_literal.h>
#include <cpp-utils/strings/string_view.h>
#include <minwindef.h>
#include <windef.h>
#include <winuser.h>
#include <experimental/string_view>

namespace Windows {

const Window MessageOnlyHWnd = Window(HWND_MESSAGE);

static ATOM getMessageSinkClass() {
  static ATOM window_class = Control::registerClass(
                               L"MessageSink",
                               0,
                               0,
                               0,
                               0,
                               cpp::wstring_view(),
                               0);
  return window_class;
}

MessageSink::MessageSink() :
  Control(getMessageSinkClass(), WS_POPUP, WS_EX_TOOLWINDOW)
{ }

void MessageSink::create(cpp::wstring_view name)
{
  Control::create(HWND_MESSAGE, name, 0, 0, 0, 0);
}

} // namespace Windows
