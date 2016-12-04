#include "toplevelwindow.h"

#include <cpp-utils/assert.h>

namespace Windows {

void TopLevelWindow::setOpacity(float value) {
  cpp_assert(getHWND());

  win_throw_on_fail(
    ::SetLayeredWindowAttributes(getHWND(), RGB(0,0,0), value * 255, LWA_ALPHA));
}

float TopLevelWindow::getOpacity() const {
  cpp_assert(getHWND());

  BYTE result;
  DWORD flags;
  win_throw_on_fail(
    ::GetLayeredWindowAttributes(getHWND(), nullptr, &result, &flags));
  return (flags & LWA_ALPHA) ? (result / 255.f) : 1.0f;
}

} // namespace Windows
