
file(GLOB lightports_com_SOURCES "*.cpp")

lightport_module(
  NAME lightports-com
  DEPS lightports-core 
  SOURCES ${lightports_com_SOURCES}
)