# Make a Python distribution

set(pyfiles "alps/__init__.py" "alps/hdf5.py" "alps/mc.py")
set(txtfiles "${CMAKE_SOURCE_DIR}/ACKNOWLEDGE.TXT"
             "${CMAKE_SOURCE_DIR}/COPYRIGHT.TXT"
             "${CMAKE_SOURCE_DIR}/LICENSE.TXT")
set(readme "${CMAKE_SOURCE_DIR}/README.md")
set(manifest "${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in")

set(c_pydir "${CMAKE_CURRENT_BINARY_DIR}/alps/alps_c/")
set(setup_file "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set(copy_stamp_file "${CMAKE_CURRENT_BINARY_DIR}/copy.stamp")
set(module_stamp_file "${CMAKE_CURRENT_BINARY_DIR}/python.stamp")

configure_file("setup.py.in" ${setup_file})
# configure_file("run_pytest.sh.in" "run_pytest.sh" @ONLY)
file(MAKE_DIRECTORY "${c_pydir}")
file(GENERATE OUTPUT "${c_pydir}/__init__.py" CONTENT "")

add_custom_command(OUTPUT ${copy_stamp_file}
                   COMMAND ${CMAKE_COMMAND} -E touch ${copy_stamp_file})

foreach(tgt_ ${python_c_targets})
  add_custom_command(OUTPUT ${copy_stamp_file} 
                     DEPENDS ${tgt_}
                     APPEND
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${tgt_}> "${c_pydir}/"
                     # COMMAND ${CMAKE_COMMAND} -E touch "copy_${tgt_}.stamp"
                     VERBATIM)
endforeach()

set(copy_cmds_ "")
foreach (f_ ${pyfiles})
  add_custom_command(OUTPUT ${copy_stamp_file} 
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${f_}
                     APPEND
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${f_} alps/
                     VERBATIM)
endforeach()
foreach(f_ ${txtfiles} ${manifest})
  add_custom_command(OUTPUT ${copy_stamp_file} 
                     DEPENDS ${f_}
                     APPEND
                     COMMAND ${CMAKE_COMMAND} -E copy ${f_} ./
                     VERBATIM)
endforeach()
add_custom_command(OUTPUT ${copy_stamp_file} 
                   DEPENDS ${readme}
                   APPEND
                   COMMAND ${CMAKE_COMMAND} -E copy ${readme} ./README.txt)

add_custom_command(OUTPUT ${module_stamp_file}
                   VERBATIM
                   DEPENDS ${copy_stamp_file} ${setup_file}
                   COMMAND ${PYTHON_EXECUTABLE} ${setup_file} "sdist" "--dist-dir" ${CMAKE_BINARY_DIR}
                   COMMAND ${CMAKE_COMMAND} -E touch ${module_stamp_file}
                   WORKING_DIRECTORY ".")

add_custom_target(pypackage ALL DEPENDS ${module_stamp_file})
