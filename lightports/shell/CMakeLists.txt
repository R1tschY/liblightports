
file(GLOB lightports_shell_SOURCES "*.cpp")

lightport_module(
  NAME lightports-shell
  DEPS lightports-core
  SOURCES ${lightports_shell_SOURCES}
)