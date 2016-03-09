
file(GLOB lightports_com_SOURCES "*.cpp")

add_library(lightports-com STATIC ${lightports_com_SOURCES})
target_link_libraries(lightports-com PUBLIC lightports-core)

set_target_properties(lightports-com PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)
