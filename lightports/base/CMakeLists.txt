
file(GLOB lightports_base_SOURCES "*.cpp")

lightport_module(
  NAME lightports-base 
  DEPS lightports-core 
  SOURCES ${lightports_base_SOURCES}
)

