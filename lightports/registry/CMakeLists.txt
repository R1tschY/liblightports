
file(GLOB lightports_shell_SOURCES "*.cpp")

lightport_module(
  NAME lightports-registry
  DEPS lightports-core
  SOURCES ${lightports_shell_SOURCES}
)