
add_library(lightports-controls STATIC control.cpp window.cpp utilwindow.cpp messagesink.cpp)
target_link_libraries(lightports-controls PUBLIC lightports-base ole32 shell32 gdiplus)
set_target_properties(lightports-controls PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)

#add_library(lightports-gdi-controls STATIC gdipluscontext.cpp graphicscontrol.cpp onscreendisplay.cpp)
#target_link_libraries(lightports-gdi-controls PUBLIC lightports-controls gdiplus)
#set_target_properties(lightports-gdi-controls PROPERTIES
#	CXX_STANDARD 14
#	CXX_STANDARD_REQUIRED ON
#)
