
file(GLOB lightports_extra_SOURCES "*.cpp")

lightport_module(
  NAME lightports-extra
  IN_DEPS lightports-base
  EX_DEPS comctl32
  SOURCES ${lightports_extra_SOURCES}
)