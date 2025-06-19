cmake_minimum_required(VERSION 3.10)

project(blazefetch)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default compilers if not provided
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER g++)
endif()

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

# Executable name
set(BIN blazefetch)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")

# Linker libraries
set(LINK_LIBS X11 Xrandr asound curl)

# Export the architecture and define ICONIC
add_definitions(-DBUILD_ARCH="${CMAKE_SYSTEM_PROCESSOR}" -DICONIC)

# Add executable
add_executable(${BIN} ${SRC})
target_include_directories(${BIN} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(${BIN} PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(${BIN} PRIVATE ${LINK_LIBS})
set_target_properties(${BIN} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install target
install(TARGETS ${BIN} DESTINATION /usr/local/bin)

# Custom build messages
get_filename_component(COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME)
message(STATUS "Build configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler: ${COMPILER_NAME} (Version: ${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Source Files: ${SRC}")
message(STATUS "  Executable: ${BIN}")
