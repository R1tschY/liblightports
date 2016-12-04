#include "onscreendisplay.h"

#include "../core/debug.h"
#include "gdiplus.h"

using namespace Gdiplus;

namespace Windows {

OnScreenDisplay::OnScreenDisplay() :
  GraphicsControl(
    WS_POPUP,
    WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TRANSPARENT)
{
  // transparent for clicks, ... : WS_EX_TRANSPARENT
}

void OnScreenDisplay::onPaint(Graphics& context)
{
  SolidBrush pen(Color(Color::Black));
  context.FillRectangle(&pen, convert_to<Gdiplus::RectF>(getClientRect()));

  SolidBrush white(Color(Color::White));
  Font font(L"Arial", 32);
  StringFormat format;
  format.SetAlignment(StringAlignmentCenter);

  context.DrawString(L"PowerWin", -1, &font, convert_to<Gdiplus::RectF>(getClientRect()), &format, &white);
}

void OnScreenDisplay::onCreate()
{
  SetLayeredWindowAttributes(getHWND(), RGB(0,0,0), 255*0.5, LWA_ALPHA);
}

} // namespace Windows
