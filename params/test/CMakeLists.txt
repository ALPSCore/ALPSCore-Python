set(targets_ "")
make_python_module_helper(NAME pyparams_test NO_BOOST VAR targets_ ../src/params.cpp)
add_pytest(pyparam_interface ${CMAKE_CURRENT_BINARY_DIR})
