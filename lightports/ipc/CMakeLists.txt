
file(GLOB lightports_ipc_SOURCES "*.cpp")

add_library(lightports-ipc STATIC ${lightports_ipc_SOURCES})
target_link_libraries(lightports-ipc PUBLIC lightports-core)

set_target_properties(lightports-ipc PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)
