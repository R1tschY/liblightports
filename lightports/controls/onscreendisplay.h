#ifndef ONSCREENDISPLAY_H
#define ONSCREENDISPLAY_H

#include "graphicscontrol.h"

#include <cpp-utils/strings/string_literal.h>

namespace Windows {

class OnScreenDisplay : public GraphicsControl
{
public:
  OnScreenDisplay();

  void onPaint(Gdiplus::Graphics& context) override;
  void onCreate() override;
};

} // namespace Windows

#endif // ONSCREENDISPLAY_H
