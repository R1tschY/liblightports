
file(GLOB lightports_extra_SOURCES "*.cpp")

lightport_module(
  NAME lightports-extra
  IN_DEPS lightports-base
  SOURCES ${lightports_extra_SOURCES}
)