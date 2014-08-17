
SET (CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS} -Wno-deprecated")
SET (CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS} -std=c++11 -fabi-version=7 -lstdc++")# -stdlib=libc++

# build test programs.
set(TEST_SOURCE_FILE "${PROJECT_SOURCE_DIR}/utils.cxx")
set(TEST_PROG_NAME "argparse_test")
add_executable(${TEST_PROG_NAME} ${TEST_SOURCE_FILE})
install (TARGETS ${TEST_PROG_NAME} DESTINATION ${PROJECT_BINARY_DIR})

