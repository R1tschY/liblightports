
file(GLOB lightports_base_SOURCES "*.cpp")

add_library(lightports-base STATIC ${lightports_base_SOURCES})
target_link_libraries(lightports-base PUBLIC lightports-core)

set_target_properties(lightports-base PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)
