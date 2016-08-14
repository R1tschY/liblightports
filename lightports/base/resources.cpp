/*
 * resources.cpp
 *
 *  Created on: 09.08.2015
 *      Author: Richard
 */

#include "resources.h"
#include <lightports/core/debug.h>
#include <commctrl.h>

namespace Windows {

std::wstring Resources::getString(HINSTANCE hinstance, unsigned id)
{
  const wchar_t* p = nullptr;
  ::SetLastError(0);
  int len = ::LoadStringW(hinstance, id, reinterpret_cast<LPWSTR>(&p), 0);
  if (len <= 0)
  {
    win_throw_on_fail((HRESULT)::GetLastError());
    return std::wstring();
  }

  return std::wstring(p, static_cast<size_t>(len));
}

HICON Resources::getIcon(HINSTANCE hinstance, int id)
{
  return win_throw_on_fail(LoadIcon(hinstance, MAKEINTRESOURCE(id)));
}

HICON Resources::getIcon(HINSTANCE hinstance, int id, Size size)
{
  return (HICON)win_throw_on_fail(
      LoadImage(
          hinstance,
          MAKEINTRESOURCE(id),
          IMAGE_ICON,
          size.getWidth(), size.getHeight(),
          LR_SHARED)
      );
}

//Icon Resources::getIconMetric(HINSTANCE hinstance, int id, int lim)
//{
//  HICON result;
//  win_throw_on_fail(
//    ::LoadIconMetric(
//        hinstance,
//        MAKEINTRESOURCE(id),
//        lim,
//        &result)
//    );
//  return result;
//}

} /* namespace Windows */

