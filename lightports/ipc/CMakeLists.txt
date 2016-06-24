
file(GLOB lightports_ipc_SOURCES "*.cpp")

lightport_module(
  NAME lightports-ipc
  IN_DEPS lightports-core
  SOURCES ${lightports_ipc_SOURCES}
)