
file(GLOB lightports_extra_SOURCES "*.cpp")

lightport_module(
  NAME lightports-extra
  DEPS lightports-core comctl32 cpp-utils
  SOURCES ${lightports_extra_SOURCES}
)