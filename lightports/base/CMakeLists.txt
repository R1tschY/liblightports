
file(GLOB lightports_base_SOURCES "*.cpp")

lightport_module(
  NAME lightports-base 
  IN_DEPS lightports-core 
  SOURCES ${lightports_base_SOURCES}
)

