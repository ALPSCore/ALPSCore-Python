# 
# alps-python cmake configuration script.
# 

cmake_minimum_required (VERSION 2.8.12)

project (alps-python CXX)

# add common modules from ../common/cmake
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/common/cmake)

# add common definitions
include(ALPSCommonModuleDefinitions)

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

set(python_c_targets "")
add_subdirectory(hdf5)
add_subdirectory(mc)

add_subdirectory(python)


#install(EXPORT ${PROJECT_NAME} NAMESPACE alps-python:: DESTINATION share/${PROJECT_NAME})

add_testing()

#gen_pkg_config()
#gen_find_module(alps/utilities/boost_python.hpp HEADER_ONLY) 

add_subdirectory("utilities/test")
