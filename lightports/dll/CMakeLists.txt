
file(GLOB lightports_dll_SOURCES "*.cpp")

lightport_module(
  NAME lightports-dll
  DEPS lightports-extra
  SOURCES ${lightports_dll_SOURCES}
)