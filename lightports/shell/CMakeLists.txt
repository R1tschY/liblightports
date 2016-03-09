
file(GLOB lightports_shell_SOURCES "*.cpp")

add_library(lightports-shell STATIC ${lightports_shell_SOURCES})
target_link_libraries(lightports-shell PUBLIC lightports-core)

set_target_properties(lightports-shell PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)
