cmake_minimum_required(VERSION 3.10)

project(blazefetch)

set(CMAKE_CXX_STANDARD 23)

# Set architecture flags
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64-bit architecture detected")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "32-bit architecture detected")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif()

# Source files
set(SRC blazefetch.cpp)
set(SRC_TEST blazefetch_test.cpp)

# Binary files
set(BIN blazefetch)
set(BIN_TEST blazefetch_test)

# Flags
set(FLAG -lX11)
set(ICONIC "-D ICONIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

# Build the project
add_executable(${BIN} ${SRC})
target_link_libraries(${BIN} PRIVATE ${FLAG})
set_target_properties(${BIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build the project with icons
add_executable(${BIN}_iconic ${SRC})
target_compile_definitions(${BIN}_iconic PRIVATE ${ICONIC})
target_link_libraries(${BIN}_iconic PRIVATE ${FLAG})
set_target_properties(${BIN}_iconic PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install targets
install(TARGETS ${BIN} ${BIN}_iconic DESTINATION /usr/local/bin)
