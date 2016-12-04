#ifndef ICON_H
#define ICON_H

#include <windows.h>
#include <memory>

#include <cpp-utils/memory/unmanaged.h>
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

  HICON getHICON() const { return hicon_.get(); }

private:
  Storage hicon_;
};

using IconView   = IconBase<std::unique_ptr<HICON, cpp::no_deleter<HICON>>>;
using IconHandle = IconBase<std::unique_ptr<HICON, HICONDeleter>>;

}

#endif // ICON_H
