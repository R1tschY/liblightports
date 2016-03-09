
file(GLOB lightports_extra_SOURCES "*.cpp")

add_library(lightports-extra STATIC ${lightports_extra_SOURCES})
target_link_libraries(lightports-extra PUBLIC lightports-base)

set_target_properties(lightports-extra PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)
