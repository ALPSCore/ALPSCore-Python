set(target_ "pyobjtest_c")

add_library(${target_} pyobj_interface.cpp)
target_include_directories(${target_} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/  ${CMAKE_SOURCE_DIR}/utilities/include)
set_target_properties(${target_} PROPERTIES PREFIX "")
target_link_libraries(${target_} ${PYTHON_LIBRARIES} ${ALPSCore_LIBRARIES})

configure_file("${CMAKE_SOURCE_DIR}/python/run_pytest.sh.in" "run_pytest.sh")
