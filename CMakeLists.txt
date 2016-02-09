# 
# alps-python cmake configuration script.
# 

cmake_minimum_required (VERSION 2.8.12)

# define project name
project (alps-python CXX)
#message ("\nALPSCore module : ${PROJECT_NAME}\n")

# add common modules from ../common/cmake
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/common/cmake)

# add common definitions
include(ALPSCommonModuleDefinitions)

# gen_documentation()

# search for python
find_package(PythonLibs REQUIRED)
# NOTE: we need Python, but not necessarily the same version as the libraries (as of now!)
find_package(PythonInterp REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# find numpy
find_package(NumPy REQUIRED)
include_directories(${NUMPY_INCLUDE_DIRS})

# Boost
#set(Boost_USE_STATIC_LIBS        ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(ALPSCore REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

# This macro is to be used in CMake subdirs!
# Sets up building a C-Python module
# Adds the target to the **PARENT**-scoped variable python_c_targets
# (that is, modifies the variable in the parent directory scope!)
# arguments: name sources...
macro(make_python_module name)
  gen_documentation()
  if (DocumentationOnly)
    return()
  endif (DocumentationOnly)
  find_package(Boost REQUIRED COMPONENTS python)
  set(target_ ${name}_c)
  set(srcs_ "")
  foreach(src_    ${name}.cpp ${ARGN})
    list(APPEND srcs_ src/${src_})
  endforeach()
  # C library implementing the module: 
  add_library(${target_} MODULE ${srcs_} ${CMAKE_SOURCE_DIR}/utilities/src/import_numpy.cpp)
  target_include_directories(${target_} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/  ${CMAKE_SOURCE_DIR}/utilities/include/ )
  set_target_properties(${name}_c PROPERTIES PREFIX "")
  target_link_libraries(${target_} ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} ${ALPSCore_LIBRARIES})
  list(APPEND python_c_targets ${target_})
  set(python_c_targets ${python_c_targets} PARENT_SCOPE)
endmacro()

set(python_c_targets "")
add_subdirectory(hdf5)
add_subdirectory(mc)

add_subdirectory(python)


#install(EXPORT ${PROJECT_NAME} NAMESPACE alps-python:: DESTINATION share/${PROJECT_NAME})



#add_testing()

#gen_pkg_config()

#gen_find_module(alps/utilities/boost_python.hpp HEADER_ONLY) 
