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

#include <cpp-utils/memory/unmanaged.h>
#include <lightports/core.h>

namespace Windows {

struct HCURSORDeleter
{
  using pointer = HCURSOR;
  void operator()(HCURSOR ptr) { ::DestroyCursor(ptr); }
};

template<typename Storage>
class CursorBase : public Storage
{
public:
  CursorBase()
  : Storage(nullptr)
  { }

  explicit
  CursorBase(HCURSOR hicon)
  : Storage(hicon)
  { }

  CursorBase<std::unique_ptr<HCURSOR, HCURSORDeleter>> copy() const
  {
    return CopyCursor(getHCURSOR());
  }

  HCURSOR getHCURSOR() const { return Storage::get(); }
};

using CursorView   = CursorBase<std::unique_ptr<HCURSOR, cpp::no_deleter<HCURSOR>>>;
using CursorHandle = CursorBase<std::unique_ptr<HCURSOR, HCURSORDeleter>>;

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

inline
CursorView setCursor(CursorView newCursor)
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
  Wait = OCR_WAIT
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
