lightport_module(
  NAME lightports-controls
  DEPS lightports-base ole32 shell32 gdiplus
  SOURCES control.cpp window.cpp utilwindow.cpp messagesink.cpp
)

lightport_module(
  NAME lightports-gdi-controls
  DEPS lightports-controls gdiplus
  SOURCES gdipluscontext.cpp graphicscontrol.cpp onscreendisplay.cpp
)