cmake_minimum_required(VERSION 3.10)

project(blazefetch)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Specify the compiler
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER g++)
endif()

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
