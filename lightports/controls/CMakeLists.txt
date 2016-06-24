lightport_module(
  NAME lightports-controls
  IN_DEPS lightports-base
  EX_DEPS ole32 shell32 gdiplus
  SOURCES control.cpp window.cpp utilwindow.cpp messagesink.cpp
)

lightport_module(
  NAME lightports-gdi-controls
  IN_DEPS lightports-controls
  EX_DEPS gdiplus
  SOURCES gdipluscontext.cpp graphicscontrol.cpp onscreendisplay.cpp
)