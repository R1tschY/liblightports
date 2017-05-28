#ifndef ICON_H
#define ICON_H

#include <windows.h>
#include <memory>

#include <cpp-utils/memory/ptr_algorithm.h>
#include <lightports/core/memory.h>

namespace Windows {

struct HICONDeleter
{
  using pointer = HICON;
  void operator()(HICON ptr) { ::DestroyIcon(ptr); }
};

template<typename Storage>
class IconBase
{
public:
  IconBase()
  : hicon_(nullptr)
  { }

  explicit
  IconBase(HICON hicon)
  : hicon_(hicon)
  { }

  HICON getHICON() const { return cpp::get_ptr(hicon_); }

private:
  Storage hicon_;
};

using IconView   = IconBase<HICON>;
using IconHandle = IconBase<std::unique_ptr<HICON, HICONDeleter>>;

}

#endif // ICON_H
