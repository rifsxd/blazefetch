cmake_minimum_required(VERSION 3.10)
project(blazefetch)

set(CMAKE_CXX_STANDARD 11)

# Source files
set(SRC blazefetch.cpp)
set(SRC_TEST blazefetch_test.cpp)

# Binary files
set(BIN blazefetch)
set(BIN_TEST blazefetch_test)

# Flags
set(FLAG -lX11)
set(ICONIC "-D ICONIC")

# Build the project
add_executable(${BIN} ${SRC})
target_link_libraries(${BIN} PRIVATE ${FLAG})
set_target_properties(${BIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build the project with icons
add_executable(${BIN}_iconic ${SRC})
target_compile_definitions(${BIN}_iconic PRIVATE ${ICONIC})
target_link_libraries(${BIN}_iconic PRIVATE ${FLAG})
set_target_properties(${BIN}_iconic PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build the test
add_executable(${BIN_TEST} ${SRC_TEST})
target_link_libraries(${BIN_TEST} PRIVATE ${FLAG})
set_target_properties(${BIN_TEST} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build the test with icons
add_executable(${BIN_TEST}_iconic ${SRC_TEST})
target_compile_definitions(${BIN_TEST}_iconic PRIVATE ${ICONIC})
target_link_libraries(${BIN_TEST}_iconic PRIVATE ${FLAG})
set_target_properties(${BIN_TEST}_iconic PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install targets
install(TARGETS ${BIN} ${BIN}_iconic ${BIN_TEST} ${BIN_TEST}_iconic DESTINATION /usr/local/bin)