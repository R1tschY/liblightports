
function(lightport_lib)

  # options
  
  set(options )
  set(oneValueArgs NAME BITS)
  set(multiValueArgs SOURCES IN_DEPS EX_DEPS)
  set(prefix lightport_lib)
  cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (${prefix}_BITS)
    # set name
    set(${prefix}_NAME ${${prefix}_NAME}${${prefix}_BITS})
    
    # set compile options
    set(_extra_properties COMPILE_OPTIONS "-m${${prefix}_BITS}" LINK_FLAGS "-m${${prefix}_BITS}")
    
    # set right deps
    set(_orig_deps ${${prefix}_IN_DEPS})
    unset(${prefix}_IN_DEPS)
    foreach(_dep ${_orig_deps})
      list(APPEND ${prefix}_IN_DEPS ${_dep}${${prefix}_BITS})
    endforeach()    
  endif()

  add_library(${${prefix}_NAME} STATIC EXCLUDE_FROM_ALL ${${prefix}_SOURCES})
  target_link_libraries(${${prefix}_NAME} PUBLIC ${${prefix}_IN_DEPS} ${${prefix}_EX_DEPS})
  
  set_target_properties(${${prefix}_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    ${_extra_properties}
  )

endfunction()

function(lightport_module)

  # options
  
  set(oneValueArgs NAME)
  set(prefix lightport_module)
  cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  lightport_lib(${ARGN} BITS 32)
  lightport_lib(${ARGN} BITS 64)
  
  if (NOT(CMAKE_SIZEOF_VOID_P EQUAL 8))
    add_library(${${prefix}_NAME} ALIAS ${${prefix}_NAME}64)
  else()
    add_library(${${prefix}_NAME} ALIAS ${${prefix}_NAME}32)
  endif()

endfunction()

add_subdirectory(base)
add_subdirectory(com)
add_subdirectory(controls)
add_subdirectory(core)
add_subdirectory(extra)
add_subdirectory(ipc)
add_subdirectory(dll)
add_subdirectory(shell)
