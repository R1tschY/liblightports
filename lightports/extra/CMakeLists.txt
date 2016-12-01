
file(GLOB lightports_extra_SOURCES "*.cpp")

lightport_module(
  NAME lightports-extra
  DEPS lightports-base comctl32
  SOURCES ${lightports_extra_SOURCES}
)