cmake_minimum_required(VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Rehoboam")

# Compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags based on the compiler and build type
if(MSVC)
    # Set compiler flags for MSVC
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /D_FILE_OFFSET_BITS=64 /Zi")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W3 /D_FILE_OFFSET_BITS=64")
    endif()
else()
    # Set compiler flags for GCC/Clang
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations -Wno-psabi -D_FILE_OFFSET_BITS=64 -g")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -W -Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations -Wno-psabi -D_FILE_OFFSET_BITS=64")
    endif()
endif()

# Include directories
include_directories(/opt/vc/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find Rehoboam
find_package(Rehoboam REQUIRED)

# Determine the appropriate library directory based on the platform
if(WIN3)
    link_directories(${CMAKE_SOURCE_DIR}/lib/x64/windows)
elseif(APPLE)
    link_directories(${CMAKE_SOURCE_DIR}/lib/x64/macos)
elseif(UNIX) # Assuming UNIX is Linux for Raspberry Pi
    link_directories(${CMAKE_SOURCE_DIR}/lib/armv7l)
endif()

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# Define the executable
add_executable(rehoboam ${SOURCES} include/network/command_message_client.h src/network/command_message_client.cpp include/network/command.h include/state/application_state.h src/state/application_state.cpp src/applications/shader_application.cpp include/applications/lava_lamp.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET rehoboam PROPERTY CXX_STANDARD 20)
endif()

# Link libraries to the executable
if(WIN3)
    target_link_libraries(rehoboam glfw3 Rehoboam::SocketLibrary OpenSSL::SSL OpenSSL::Crypto)
elseif(APPLE)
    target_link_libraries(rehoboam glfw3 Rehoboam::SocketLibrary OpenSSL::SSL OpenSSL::Crypto "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
elseif(UNIX) # Assuming UNIX is Linux for Raspberry Pi
    target_link_libraries(rehoboam glfw3 Rehoboam::SocketLibrary OpenSSL::SSL OpenSSL::Crypto)
endif()

# Copy shaders to build directory
# Find all .glsl files in the shaders directory and subdirectories
file(GLOB_RECURSE SHADERS "shaders/*.glsl")

# Ensure the shaders directory exists in the build directory
set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders")
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

# Create a list to hold our custom command outputs
set(SHADER_OUTPUTS "")

foreach(SHADER ${SHADERS})
    # Get the path of the folder within the shaders directory
    get_filename_component(SHADER_PATH "${SHADER}" PATH)
    # Remove the source directory from the path to get the relative directory path
    file(RELATIVE_PATH SHADER_REL_PATH "${CMAKE_SOURCE_DIR}/shaders" "${SHADER_PATH}")
    # Make sure the corresponding directory structure exists in the build directory
    file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}/${SHADER_REL_PATH}")

    get_filename_component(SHADER_NAME "${SHADER}" NAME)
    # Combine directory structure with the shader filename
    set(SHADER_OUTPUT_PATH "${SHADER_OUTPUT_DIR}/${SHADER_REL_PATH}/${SHADER_NAME}")
    message(STATUS "Preparing to copy shader: ${SHADER}")

    add_custom_command(
            OUTPUT ${SHADER_OUTPUT_PATH}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SHADER}
            ${SHADER_OUTPUT_PATH}
            DEPENDS ${SHADER}
            COMMENT "Copying ${SHADER_NAME} to build directory"
    )
    # Append the output path of the shader to our list of shader outputs
    list(APPEND SHADER_OUTPUTS ${SHADER_OUTPUT_PATH})
endforeach()

# Create a custom target that depends on all of our shader outputs
add_custom_target(Shaders ALL DEPENDS ${SHADER_OUTPUTS})

# Make sure our main target depends on this
add_dependencies(rehoboam Shaders)

