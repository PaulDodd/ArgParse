

# build test programs.
set(TEST_SOURCE_FILE "${PROJECT_SOURCE_DIR}/utils.cxx")
set(TEST_PROG_NAME "argparse_test")
add_executable(${TEST_PROG_NAME} ${TEST_SOURCE_FILE})
install (TARGETS ${TEST_PROG_NAME} DESTINATION ${PROJECT_BINARY_DIR})

