set(targets_ "")
make_python_module_helper(NAME pyobjtest NO_BOOST VAR targets_)
add_pytest(pyobj_interface ${CMAKE_CURRENT_BINARY_DIR})
