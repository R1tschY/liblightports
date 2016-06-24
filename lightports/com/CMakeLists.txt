
file(GLOB lightports_com_SOURCES "*.cpp")

lightport_module(
  NAME lightports-com
  IN_DEPS lightports-core 
  SOURCES ${lightports_com_SOURCES}
)