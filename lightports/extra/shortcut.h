/// \file shortcut.h

#ifndef HOOKLIB_WINDOWS_SHORTCUT_H_
#define HOOKLIB_WINDOWS_SHORTCUT_H_

namespace Windows {

struct ShortCut
{
  unsigned modifiers = 0;
  unsigned key = 0;

  bool isValid() const noexcept { return key != 0; }
  explicit operator bool() const noexcept
  {
    return isValid();
  }
};

} // namespace Windows

#endif /* HOOKLIB_WINDOWS_SHORTCUT_H_ */
