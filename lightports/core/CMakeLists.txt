
file(GLOB lightports_core_SOURCES "*.cpp")

lightport_module(
  NAME lightports-core
  DEPS cpp-utils
  SOURCES ${lightports_core_SOURCES}
)

# TODO
target_include_directories(lightports-core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../..")
