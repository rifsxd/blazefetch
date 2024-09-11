cmake_minimum_required(VERSION 3.10)

project(blazefetch)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Allow user to specify the compiler (default to g++)
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER g++)
endif()

# Allow user to specify the compiler (default to gcc)
if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER gcc)
endif()

# Allow user to specify the compiler (default to clang++)
#if(NOT CMAKE_CXX_COMPILER)
#    set(CMAKE_CXX_COMPILER clang++)
#endif()

# Allow user to specify the compiler (default to clang)
#if(NOT CMAKE_C_COMPILER)
#    set(CMAKE_C_COMPILER clang)
#endif()

# Make the build process more verbose
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Set build type
set(CMAKE_BUILD_TYPE Debug)

# Source files
set(SRC blazefetch.cpp)

# Executable names      
set(BIN blazefetch)
set(BIN_ICONIC ${BIN}_iconic)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

# Linker flags
set(LINK_LIBS X11 Xrandr asound curl)

# Create object library
add_library(${BIN}_obj OBJECT ${SRC})
target_include_directories(${BIN}_obj PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Export the Architecture during build process
add_definitions(-DBUILD_ARCH="${CMAKE_SYSTEM_PROCESSOR}")

# Common properties
function(set_common_properties TARGET)
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
    target_link_libraries(${TARGET} PRIVATE ${LINK_LIBS})
    set_target_properties(${TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endfunction()

# Build the project without icons
add_executable(${BIN} $<TARGET_OBJECTS:${BIN}_obj>)
set_common_properties(${BIN})

# Build the project with icons
add_executable(${BIN_ICONIC} $<TARGET_OBJECTS:${BIN}_obj>)
target_compile_definitions(${BIN_ICONIC} PRIVATE ICONIC)
set_common_properties(${BIN_ICONIC})

# Install targets
install(TARGETS ${BIN} ${BIN_ICONIC} DESTINATION /usr/local/bin)

# Extract the compiler name from the full path
get_filename_component(COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME)

# Custom message for build configuration
message(STATUS "Build configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler: ${COMPILER_NAME} (Version: ${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Source Files: ${SRC}")
message(STATUS "  Executables: ${BIN}, ${BIN_ICONIC}")
