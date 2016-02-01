#
# Provide common definitions for building alps modules 
#

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


# Usage: add_this_package(srcs...)
# The `srcs` are source file names in directory "src/"
# Defines ${PROJECT_NAME} target
# Exports alps::${PROJECT_NAME} target
function(add_this_package)
   # This is needed to compile tests:
   include_directories(
     ${PROJECT_SOURCE_DIR}/include
     ${PROJECT_BINARY_DIR}/include
   )
  
  set(src_list_ "")
  foreach(src_ ${ARGV})
    list(APPEND src_list_ "src/${src_}.cpp")
  endforeach()
  add_library(${PROJECT_NAME} ${ALPS_BUILD_TYPE} ${src_list_})
  set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

  install(TARGETS ${PROJECT_NAME} 
          EXPORT ${PROJECT_NAME} 
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          INCLUDES DESTINATION include)
  install(EXPORT ${PROJECT_NAME} NAMESPACE alps:: DESTINATION share/${PROJECT_NAME})
  target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)

  install(DIRECTORY include DESTINATION .
          FILES_MATCHING PATTERN "*.hpp" PATTERN "*.hxx"
         )
endfunction(add_this_package)

macro(add_testing)
  option(Testing "Enable testing" ON)
  if (Testing)
    enable_testing()
    add_subdirectory(test)
  endif (Testing)
endmacro(add_testing)

macro(gen_documentation)
  set(DOXYFILE_EXTRA_SOURCES "${DOXYFILE_EXTRA_SOURCES} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src" PARENT_SCOPE)
  option(Documentation "Build documentation" OFF)
  if (Documentation)
    set(DOXYFILE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/include")
    set(DOXYFILE_IN "${PROJECT_SOURCE_DIR}/../common/doc/Doxyfile.in") 
    include(UseDoxygen)
  endif(Documentation)
endmacro(gen_documentation)

macro(gen_hpp_config)
  configure_file("${PROJECT_SOURCE_DIR}/include/config.hpp.in" "${PROJECT_BINARY_DIR}/include/alps/config.hpp")
  install(FILES "${PROJECT_BINARY_DIR}/include/alps/config.hpp" DESTINATION include/alps) 
endmacro(gen_hpp_config)

macro(gen_pkg_config)
  # Generate pkg-config file
  configure_file("${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.pc.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc")
  install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION "lib/pkgconfig")
endmacro(gen_pkg_config)


# Function: generates package-specific CMake configs
# Arguments: [DEPENDS <list-of-dependencies>] [EXPORTS <list-of-exported-targets>]
# If no <list-of-dependencies> are present, the contents of ${PROJECT_NAME}_DEPENDS is used
# If no exported targets are present, alps::${PROJECT_NAME} is assumed.
function(gen_cfg_module)
    include(CMakeParseArguments) # arg parsing helper
    cmake_parse_arguments(gen_cfg_module "" "" "DEPENDS;EXPORTS" ${ARGV})
    if (gen_cfg_module_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Incorrect call of gen_cfg_module([DEPENDS ...] [EXPORTS ...]): ARGV=${ARGV}")
    endif()
    if (gen_cfg_module_DEPENDS)
        set(DEPENDS ${gen_cfg_module_DEPENDS})
    else()
        set(DEPENDS ${${PROJECT_NAME}_DEPENDS})
    endif()
    if (gen_cfg_module_EXPORTS)
        set(EXPORTS ${gen_cfg_module_EXPORTS})
    else()
        set(EXPORTS alps::${PROJECT_NAME})
    endif()
    configure_file("${PROJECT_SOURCE_DIR}/../common/cmake/ALPSModuleConfig.cmake.in" 
                   "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
    configure_file("${PROJECT_SOURCE_DIR}/../common/cmake/ALPSCoreConfig.cmake.in" 
                   "${PROJECT_BINARY_DIR}/ALPSCoreConfig.cmake" @ONLY)
    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION "share/${PROJECT_NAME}/")
    install(FILES "${PROJECT_BINARY_DIR}/ALPSCoreConfig.cmake" DESTINATION "share/ALPSCore/")
endfunction()
