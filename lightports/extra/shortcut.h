/// \file shortcut.h

#ifndef HOOKLIB_WINDOWS_SHORTCUT_H_
#define HOOKLIB_WINDOWS_SHORTCUT_H_

#include <tuple>

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


  bool operator==(const ShortCut& rhs)
  {
    return std::tie(modifiers, key) == std::tie(rhs.modifiers, rhs.key);
  }
};

} // namespace Windows

#endif /* HOOKLIB_WINDOWS_SHORTCUT_H_ */
