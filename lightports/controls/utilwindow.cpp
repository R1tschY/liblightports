#include "utilwindow.h"

#include <memory>

#include "../extra/charcodecs.h"
#include "messagesink.h"
#include "../core/debug.h"

namespace Windows {

namespace  {

class UtilWindow : public MessageSink
{
public:
  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override
  {
    switch (msg) {
    case WM_CALLBACK:
      if (lparam) {
        auto pfunc = std::unique_ptr<Callback>((Callback*)lparam);
        try {
          (*pfunc)();
        } catch (std::bad_function_call &error) {
          WIN_CRITICAL(L"callback failed: %s",
                       to_wstring(error.what()).c_str());
        }
      }
    }

    return Control::onMessage(msg, wparam, lparam);
  }
};

UtilWindow createCallbackWindow()
{
  UtilWindow message_sink;
  message_sink.create();
  return message_sink;
}

}  // namespace

HWND getCallbackWindow() {
  static UtilWindow window = createCallbackWindow();
  cpp_assert(window.getHWND());
  return window.getHWND();
}

} // namespace Windows
