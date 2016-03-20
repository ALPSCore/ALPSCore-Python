#
# Provide common definitions for building alps modules 
#

include(CMakeParseArguments)

# Disable in-source builds
if (${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    message(FATAL_ERROR "In source builds are disabled. Please use a separate build directory")
    # FIXME: augment with the advice to remove CMakeCache from ${CMAKE_SOURCE_DIR}
endif()

# RPATH fix
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
 set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
else()
 set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#policy update CMP0042
if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

# FIXME: If ALPSCore is built statically, can we still do a dynamic build?
# FIXME: If user requested a static build, shall we abort?
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

## Some macros

# macro(add_boost) # usage: add_boost(component1 component2...)
#   #set(Boost_USE_STATIC_LIBS        ON)
#   #set(Boost_USE_STATIC_RUNTIME    OFF)
#   find_package (Boost 1.54.0 COMPONENTS ${ARGV} REQUIRED)
#   message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}" )
#   message(STATUS "Boost libs: ${Boost_LIBRARIES}" )
#   target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
#   target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
# endmacro(add_boost)


## Usage: add_this_package(srcs...)
## The `srcs` are source file names in directory "src/"
## Defines ${PROJECT_NAME} target
## Exports alps::${PROJECT_NAME} target
#function(add_this_package)
#   # This is needed to compile tests:
#   include_directories(
#     ${PROJECT_SOURCE_DIR}/include
#     ${PROJECT_BINARY_DIR}/include
#   )
#  
#  set(src_list_ "")
#  foreach(src_ ${ARGV})
#    list(APPEND src_list_ "src/${src_}.cpp")
#  endforeach()
#  add_library(${PROJECT_NAME} ${ALPS_BUILD_TYPE} ${src_list_})
#  set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
#
#  install(TARGETS ${PROJECT_NAME} 
#          EXPORT ${PROJECT_NAME} 
#          LIBRARY DESTINATION lib
#          ARCHIVE DESTINATION lib
#          INCLUDES DESTINATION include)
#  install(EXPORT ${PROJECT_NAME} NAMESPACE alps:: DESTINATION share/${PROJECT_NAME})
#  target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)
#
#  install(DIRECTORY include DESTINATION .
#          FILES_MATCHING PATTERN "*.hpp" PATTERN "*.hxx"
#         )
#endfunction(add_this_package)

macro(add_testing)
  option(Testing "Enable testing" ON)
  option(TestXMLOutput "Generate XML-formatted test results" OFF)
  if (Testing)
    find_file(PYTEST "py.test" DOC "Location of py.test executable")
    if(NOT PYTEST)
      message(SEND_ERROR "
        The testing is enabled,
        but Python testing utility `py.test` is not found.
        Please specify the location of the `py.test` utility using PYTEST variable
        or disable testing (NOT RECOMMENDED).")
    endif()
    enable_testing()
    add_subdirectory(test)
  endif (Testing)
endmacro(add_testing)

# add Python test, searching for modules in the specified dir
function(add_pytest name dir)
  set(flags_)
  if(TestXMLOutput)
    set(flags_ "--junit-xml" "${name}.xml")
  endif()
  # add_test(NAME ${name}
  #          COMMAND ${CMAKE_BINARY_DIR}/python/run_pytest.sh ${flags_} ${CMAKE_CURRENT_SOURCE_DIR}/${name}_test.py)
  add_test(NAME ${name}
           COMMAND ${PYTEST} ${flags_} ${CMAKE_CURRENT_SOURCE_DIR}/${name}_test.py)
  set_property(TEST ${name} 
               PROPERTY ENVIRONMENT
               "PYTHONPATH=${dir}")
endfunction()


#macro(gen_documentation)
#  set(DOXYFILE_EXTRA_SOURCES "${DOXYFILE_EXTRA_SOURCES} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src" PARENT_SCOPE)
#  option(Documentation "Build documentation" OFF)
#  if (Documentation)
#    set(DOXYFILE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/include")
#    set(DOXYFILE_IN "${PROJECT_SOURCE_DIR}/../common/doc/Doxyfile.in") 
#    include(UseDoxygen)
#  endif(Documentation)
#endmacro(gen_documentation)

# macro(gen_hpp_config)
#   configure_file("${PROJECT_SOURCE_DIR}/include/config.hpp.in" "${PROJECT_BINARY_DIR}/include/alps/config.hpp")
#   install(FILES "${PROJECT_BINARY_DIR}/include/alps/config.hpp" DESTINATION include/alps) 
# endmacro(gen_hpp_config)

# macro(gen_pkg_config)
#   # Generate pkg-config file
#   configure_file("${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.pc.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc")
#   install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION "lib/pkgconfig")
# endmacro(gen_pkg_config)


# # Function: generates package-specific CMake configs
# # Arguments: [DEPENDS <list-of-dependencies>] [EXPORTS <list-of-exported-targets>]
# # If no <list-of-dependencies> are present, the contents of ${PROJECT_NAME}_DEPENDS is used
# # If no exported targets are present, alps::${PROJECT_NAME} is assumed.
# function(gen_cfg_module)
#     include(CMakeParseArguments) # arg parsing helper
#     cmake_parse_arguments(gen_cfg_module "" "" "DEPENDS;EXPORTS" ${ARGV})
#     if (gen_cfg_module_UNPARSED_ARGUMENTS)
#         message(FATAL_ERROR "Incorrect call of gen_cfg_module([DEPENDS ...] [EXPORTS ...]): ARGV=${ARGV}")
#     endif()
#     if (gen_cfg_module_DEPENDS)
#         set(DEPENDS ${gen_cfg_module_DEPENDS})
#     else()
#         set(DEPENDS ${${PROJECT_NAME}_DEPENDS})
#     endif()
#     if (gen_cfg_module_EXPORTS)
#         set(EXPORTS ${gen_cfg_module_EXPORTS})
#     else()
#         set(EXPORTS alps::${PROJECT_NAME})
#     endif()
#     configure_file("${PROJECT_SOURCE_DIR}/../common/cmake/ALPSModuleConfig.cmake.in" 
#                    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
#     configure_file("${PROJECT_SOURCE_DIR}/../common/cmake/ALPSCoreConfig.cmake.in" 
#                    "${PROJECT_BINARY_DIR}/ALPSCoreConfig.cmake" @ONLY)
#     install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION "share/${PROJECT_NAME}/")
#     install(FILES "${PROJECT_BINARY_DIR}/ALPSCoreConfig.cmake" DESTINATION "share/ALPSCore/")
# endfunction()


# This is a helper function used by make_python_module macro
# Sets up building a C-Python module
# Adds the target to the PARENT-scoped variable
# (that is, modifies the variable in the caller's scope)
# arguments: NAME name VAR varname NO_BOOST|USE_BOOST [SRCDIR srcdir] sources...
function(make_python_module_helper)
  CMAKE_PARSE_ARGUMENTS(_mk_pymod "USE_BOOST;NO_BOOST" "NAME;VAR;SRCDIR" "" ${ARGV})
  if (NOT _mk_pymod_NAME OR NOT _mk_pymod_VAR)
    message(FATAL_ERROR "make_python_module(): usage: make_python_module NAME modname VAR varname USE_BOOST|NO_BOOST [SRCDIR srcdir] srcs...")
  endif()
  if (NOT _mk_pymod_NO_BOOST AND NOT _mk_pymod_USE_BOOST)
    message(FATAL_ERROR "make_python_module():  either USE_BOOST or NO_BOOST must be used")
  endif()
  if (_mk_pymod_NO_BOOST AND _mk_pymod_USE_BOOST)
    message(FATAL_ERROR "make_python_module():  USE_BOOST and NO_BOOST are mutually exclusive")
  endif()
  set(libs_ ${PYTHON_LIBRARIES} ${ALPSCore_LIBRARIES})
  file(GLOB includes_ ${CMAKE_SOURCE_DIR}/*/include)
  if (_mk_pymod_USE_BOOST)
    find_package(Boost REQUIRED COMPONENTS python)
    list(APPEND libs_ ${Boost_LIBRARIES})
    list(APPEND includes_ ${Boost_INCLUDE_DIRS})
  endif()
  # if (_mk_pymod_DEPLIB)
  #   list(APPEND libs_ ${_mk_pymod_DEPLIB})
  # endif()
  set(srcdir_ ".")
  if (_mk_pymod_SRCDIR)
    set(srcdir_ ${_mk_pymod_SRCDIR})
  endif()
  
  set(target_ ${_mk_pymod_NAME}_c)
  set(srcs_ "")
  foreach(src_   ${_mk_pymod_NAME}.cpp ${_mk_pymod_UNPARSED_ARGUMENTS})
    list(APPEND srcs_ ${srcdir_}/${src_})
  endforeach()

  # C library implementing the module: 
  add_library(${target_} MODULE ${srcs_})
  target_include_directories(${target_} PRIVATE ${includes_} )
  set_target_properties(${target_} PROPERTIES PREFIX "")
  target_link_libraries(${target_} ${libs_})
  
  list(APPEND ${_mk_pymod_VAR} ${target_})
  set(${_mk_pymod_VAR} ${${_mk_pymod_VAR}} PARENT_SCOPE)
endfunction()

# This macro is to be used in CMake subdirs!
# Sets up building a C-Python module
# Adds the target to the **PARENT**-scoped variable `python_c_targets`
# (that is, modifies the variable in the parent directory scope!)
# arguments: NAME name USE_BOOST|NO_BOOST [SRCDIR srcdir] sources...
macro(make_python_module)
  # gen_documentation()
  if (DocumentationOnly)
    return()
  endif (DocumentationOnly)
  
  make_python_module_helper(VAR python_c_targets ${ARGV}) 
  set(python_c_targets ${python_c_targets} PARENT_SCOPE)
endmacro()
