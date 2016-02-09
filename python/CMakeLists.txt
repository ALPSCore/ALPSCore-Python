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
file(MAKE_DIRECTORY "${c_pydir}")
file(GENERATE OUTPUT "${c_pydir}/__init__.py" CONTENT "")

set(py_c_targets_copies "")
foreach(tgt_ ${python_c_targets})
  set(out_ "copy_${tgt_}.stamp")
  add_custom_command(OUTPUT ${out_}
                     DEPENDS ${tgt_}
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${tgt_}> "${c_pydir}/"
                     COMMAND ${CMAKE_COMMAND} -E touch "copy_${tgt_}.stamp"
                     VERBATIM)
  list(APPEND py_c_targets_copies ${out_})
endforeach()
#set_source_files_properties(${py_c_targets_copies} PROPERTIES SYMBOLIC TRUE)


set(copy_cmds_ "")
foreach (f_ ${pyfiles})
  list(APPEND copy_cmds_ COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${f_} alps/)
endforeach()
foreach(f_ ${txtfiles} ${manifest})
  list(APPEND copy_cmds_ COMMAND ${CMAKE_COMMAND} -E copy ${f_} ./)
endforeach()
list(APPEND copy_cmds_ COMMAND ${CMAKE_COMMAND} -E copy ${readme} ./README.txt)

add_custom_command(OUTPUT ${copy_stamp_file}
                   DEPENDS ${pyfiles} ${txtfiles} ${manifest} ${readme}
                   ${copy_cmds_}
                   COMMAND ${CMAKE_COMMAND} -E touch ${copy_stamp_file}
                   VERBATIM
                   WORKING_DIRECTORY ".")

add_custom_command(OUTPUT ${module_stamp_file}
                   VERBATIM
                   DEPENDS #${py_c_targets_copies} 
                           ${python_c_targets}
                           ${copy_stamp_file}
                           ${setup_file}
                   COMMAND ${PYTHON_EXECUTABLE} ${setup_file} "sdist"
                   COMMAND ${CMAKE_COMMAND} -E touch ${module_stamp_file}
                   WORKING_DIRECTORY ".")

add_custom_target(pypackage ALL DEPENDS ${module_stamp_file})
