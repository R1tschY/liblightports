
file(GLOB lightports_shell_SOURCES "*.cpp")

lightport_module(
  NAME lightports-shell
  DEPS lightports-core lightports-user
  SOURCES ${lightports_shell_SOURCES}
)