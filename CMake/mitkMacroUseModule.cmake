macro(MITK_USE_MODULE)
  set(DEPENDS "")
  set(DEPENDS_BEFORE "not initialized")
  # check for each parameter if it is a package (3rd party)
  foreach(package ${ARGN})
    set(is_package)
    foreach(dir ${MODULES_PACKAGE_DEPENDS_DIRS})
      if(EXISTS "${dir}/MITK_${package}_Config.cmake")
        list(APPEND PACKAGE_DEPENDS ${package})
        set(is_package 1)
        break()
      endif()
    endforeach()
    if(NOT is_package)
      list(APPEND DEPENDS ${package})
    endif()
  endforeach(package)
  while(NOT "${DEPENDS}" STREQUAL "${DEPENDS_BEFORE}")
    set(DEPENDS_BEFORE ${DEPENDS})
    foreach(dependency ${DEPENDS})
      if(NOT ${dependency}_CONFIG_FILE)
        message(SEND_ERROR "Missing module: ${dependency}")
      endif()
      include(${${dependency}_CONFIG_FILE})
      list(APPEND DEPENDS ${${dependency}_DEPENDS})
      list(APPEND PACKAGE_DEPENDS ${${dependency}_PACKAGE_DEPENDS})
    endforeach(dependency)
    if(DEPENDS)
      list(REMOVE_DUPLICATES DEPENDS)
      list(SORT DEPENDS)
    endif(DEPENDS)
    if(PACKAGE_DEPENDS)
      list(REMOVE_DUPLICATES PACKAGE_DEPENDS)
      list(SORT PACKAGE_DEPENDS)
    endif(PACKAGE_DEPENDS)
  endwhile()
  # CMake Debug
  set(ALL_DEPENDENCIES ${DEPENDS})
  foreach(dependency ${DEPENDS} ${MODULE_DEPENDS_INTERNAL})
    if(NOT ${dependency}_CONFIG_FILE)
      message(SEND_ERROR "Missing module ${dependency}")
    endif()
    include(${${dependency}_CONFIG_FILE})
    set(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${${dependency}_INCLUDE_DIRS})
    set(ALL_LIBRARIES ${ALL_LIBRARIES} ${${dependency}_PROVIDES})
    set(ALL_LIBRARY_DIRS ${ALL_LIBRARY_DIRS} ${${dependency}_LIBRARY_DIRS})
    if(TARGET ${dependency}-autoload)
      list(APPEND ALL_META_DEPENDENCIES ${dependency}-autoload)
    endif()
  endforeach(dependency)

  foreach(package ${PACKAGE_DEPENDS})
    foreach(dir ${MODULES_PACKAGE_DEPENDS_DIRS})
      if(EXISTS "${dir}/MITK_${package}_Config.cmake")
        include("${dir}/MITK_${package}_Config.cmake")
        break()
      endif()
    endforeach()
    #set(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${${package}_INCLUDE_DIRS})
    # set(ALL_LIBRARIES ${ALL_LIBRARIES} ${${package}_LIBRARIES})
  endforeach(package)
  set(ALL_LIBRARIES ${ALL_LIBRARIES} ${MODULE_ADDITIONAL_LIBS})
  set(ALL_INCLUDE_DIRECTORIES ${MITK_BINARY_DIR} ${ALL_INCLUDE_DIRECTORIES} ${MODULE_INCLUDE_DIRS} ${MODULE_INTERNAL_INCLUDE_DIRS} ${MODULES_CONF_DIRS})
  if(ALL_INCLUDE_DIRECTORIES)
    list(REMOVE_DUPLICATES ALL_INCLUDE_DIRECTORIES)
  endif()
  if(ALL_LIBRARY_DIRS)
    list(REMOVE_DUPLICATES ALL_LIBRARY_DIRS)
  endif(ALL_LIBRARY_DIRS)

endmacro(MITK_USE_MODULE)
