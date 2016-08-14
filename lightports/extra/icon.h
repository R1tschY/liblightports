#ifndef ICON_H
#define ICON_H

#include <windows.h>
#include <memory>

namespace Windows {

class Icon
{
public:
  Icon() { }
  Icon(HICON hicon) : hmenu_(hicon) { }

  HICON getHICON() const { return hmenu_.get(); }

private:
  struct HIconDeleter {
    typedef HICON pointer;
    void operator()(HICON ptr) { ::DestroyIcon(ptr); }
  };
  typedef std::unique_ptr<HICON, HIconDeleter> HIcon;

  HIcon hmenu_;
};

}

#endif // ICON_H
