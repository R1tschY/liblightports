#include "gdipluscontext.h"

#include <gdiplus.h>

namespace Windows {

GdiplusContext::GdiplusContext()
{
  Gdiplus::GdiplusStartupInput input;
  input.GdiplusVersion = 1;
  input.DebugEventCallback = NULL; // TODO
  input.SuppressBackgroundThread = false;

  Gdiplus::GdiplusStartupOutput output; // TODO
  GdiplusStartup(&token_, &input, &output);
}

GdiplusContext::~GdiplusContext()
{
  Gdiplus::GdiplusShutdown(token_);
}

} // namespace Windows

