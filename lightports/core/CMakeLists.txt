
file(GLOB lightports_core_SOURCES "*.cpp")

lightport_module(
  NAME lightports-core
  EX_DEPS cpp-utils
  SOURCES ${lightports_core_SOURCES}
)

# TODO
target_include_directories(lightports-core32 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")
target_include_directories(lightports-core64 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")
