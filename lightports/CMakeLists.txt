include(CMakeParseArguments)

function(lightport_module)
 
  # options
  
  set(options )
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES DEPS)
  set(prefix lightport_lib)
  cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_library(${${prefix}_NAME} STATIC EXCLUDE_FROM_ALL ${${prefix}_SOURCES})
  target_link_libraries(${${prefix}_NAME} PUBLIC ${${prefix}_DEPS})
  
  set_target_properties(${${prefix}_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    ${_extra_properties}
  ) 

endfunction()

add_subdirectory(base)
add_subdirectory(com)
add_subdirectory(controls)
add_subdirectory(core)
add_subdirectory(extra)
add_subdirectory(ipc)
add_subdirectory(dll)
add_subdirectory(shell)
