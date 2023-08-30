cmake_minimum_required(VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Rehoboam")

# Compiler flags
set(CMAKE_CXX_STANDARD 17)
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
add_executable(rehoboam ${SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET rehoboam PROPERTY CXX_STANDARD 20)
endif()

# Link libraries to the executable
if(WIN3)
    target_link_libraries(rehoboam glfw3)
elseif(APPLE)
    target_link_libraries(rehoboam glfw3 "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
elseif(UNIX) # Assuming UNIX is Linux for Raspberry Pi
    target_link_libraries(rehoboam glfw3)
endif()

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders/)
