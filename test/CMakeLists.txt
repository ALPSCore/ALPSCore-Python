# CMake script for testing
set(tests hdf5)





foreach (f_ ${tests})
  add_pytest(${f_} ${CMAKE_BINARY_DIR}/python)
endforeach()

# add_dependencies(pytest pypackage)
# add_custom_target(test DEPENDS pypackage pytest)
