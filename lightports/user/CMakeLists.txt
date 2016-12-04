
file(GLOB lightports_shell_SOURCES "*.cpp")

lightport_module(
  NAME lightports-user
  DEPS lightports-core
  SOURCES ${lightports_shell_SOURCES}
)
target_compile_definitions(lightports-user PUBLIC OEMRESOURCE)