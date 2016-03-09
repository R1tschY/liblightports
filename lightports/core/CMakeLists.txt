
file(GLOB lightports_core_SOURCES "*.cpp")

add_library(lightports-core STATIC ${lightports_core_SOURCES})
target_link_libraries(lightports-core PUBLIC cpp-utils)
target_include_directories(lightports-core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")

set_target_properties(lightports-core PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)
