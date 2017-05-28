/*
 * cursor.h
 *
 *  Created on: 02.09.2016
 *      Author: Richard
 */

#ifndef LIBS_LIGHTPORTS_LIGHTPORTS_EXTRA_CURSOR_H_
#define LIBS_LIGHTPORTS_LIGHTPORTS_EXTRA_CURSOR_H_

#include <windows.h>
#include <memory>

#include <cpp-utils/memory/ptr_algorithm.h>
#include <lightports/core.h>

#ifndef OCR_HELP
# define OCR_HELP 32651
#endif

namespace Windows {

struct HCURSORDeleter
{
  using pointer = HCURSOR;
  void operator()(HCURSOR ptr) { ::DestroyCursor(ptr); }
};

template<typename Storage = HCURSOR>
class CursorBase
{
public:
  using View = CursorBase<>;
  using Handle = CursorBase<std::unique_ptr<HCURSOR, HCURSORDeleter>>;

  CursorBase()
  : ptr_(nullptr)
  { }

  explicit
  CursorBase(HCURSOR hicon)
  : ptr_(hicon)
  { }

  Handle clone() const
  {
    return Handle(CopyCursor(getHCURSOR()));
  }

  HCURSOR getHCURSOR() const { return cpp::get_ptr(ptr_); }

private:
  Storage ptr_;
};

using CursorView   = CursorBase<>::View;
using CursorHandle = CursorBase<>::Handle;

inline
Point getCursorPosition()
{
  POINT pt;
  win_throw_on_fail(GetCursorPos(&pt));
  return pt;
}

inline
void setCursorPosition(Point pt)
{
  win_throw_on_fail(SetCursorPos(pt.getX(), pt.getY()));
}

template<typename T>
inline
CursorView setCursor(const CursorBase<T>& newCursor)
{
  return CursorView(win_throw_on_fail(
    ::SetCursor(newCursor.getHCURSOR())
  ));
}

enum class StockCursor
{
  AppStarting = OCR_APPSTARTING,
  Normal = OCR_NORMAL,
  Cross = OCR_CROSS,
  Hand = OCR_HAND,
  IBeam = OCR_IBEAM,
  No = OCR_NO,
  Size = OCR_SIZEALL,
  Icon = OCR_ICON,
  IcoCur = OCR_ICOCUR,
  SizeAll = OCR_SIZEALL,
  SizeNeSw = OCR_SIZENESW,
  SizeNS = OCR_SIZENS,
  SizeNwSe = OCR_SIZENWSE,
  SizeWE = OCR_SIZEWE,
  Up = OCR_UP,
  Wait = OCR_WAIT,

  // inofficial
  Help = OCR_HELP,
  Pen = 32631,
  WaitCd = 32663
};

inline
CursorView loadCursor(StockCursor cursor)
{
  return CursorView((HCURSOR)win_throw_on_fail(
    LoadImageW(
      nullptr,
      MAKEINTRESOURCEW(int(cursor)),
      IMAGE_CURSOR,
      0, 0,
      LR_DEFAULTSIZE | LR_SHARED)
  ));
}

inline
CursorHandle loadCursor(
  HINSTANCE hinstance,
  int cursorResource,
  Size size, DWORD flags)
{
  return CursorHandle((HCURSOR)win_throw_on_fail(
    LoadImageW(
      hinstance,
      MAKEINTRESOURCEW(cursorResource),
      IMAGE_CURSOR,
      size.getWidth(), size.getHeight(),
      flags)
  ));
}

inline
CursorHandle loadCursor(
  HINSTANCE hinstance,
  cpp::wstring_view cursorResource,
  Size size, DWORD flags)
{
  return CursorHandle(static_cast<HCURSOR>(win_throw_on_fail(
    LoadImageW(
      hinstance,
      cursorResource.data(),
      IMAGE_CURSOR,
      size.getWidth(), size.getHeight(),
      flags)
  )));
}

}  // namespace Windows

#endif /* LIBS_LIGHTPORTS_LIGHTPORTS_EXTRA_CURSOR_H_ */
