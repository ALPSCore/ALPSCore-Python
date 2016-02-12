# CMake script for testing
set(tests hdf5)





foreach (f_ ${tests})
  add_pytest(${f_})
endforeach()

# add_dependencies(pytest pypackage)
# add_custom_target(test DEPENDS pypackage pytest)
