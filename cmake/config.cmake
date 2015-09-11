################################################################################
### Feel free to change anything in this file (just keep the variables name) ###
################################################################################

# Should CMake stop the generation in this cases?
set (FATAL_ERROR_IF_OS_WINDOW		FALSE)	# default: FALSE
set (FATAL_ERROR_IF_OS_LINUX		FALSE)	# default: FALSE
set (FATAL_ERROR_IF_OS_MACOSX		FALSE)	# default: FALSE
set (FATAL_ERROR_IF_UNKNOWN_OS		TRUE)	# default: TRUE
set (FATAL_ERROR_IF_ARCH_32B		FALSE)	# default: FALSE
set (FATAL_ERROR_IF_ARCH_64B		FALSE)	# default: FALSE
set (FATAL_ERROR_IF_UNKNOWN_ARCH	TRUE)	# default: TRUE

# Output directories
set (BIN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")  # default: "${CMAKE_CURRENT_BINARY_DIR}/bin"
set (DYNAMIC_LIBRARY_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib") # default: "${CMAKE_CURRENT_BINARY_DIR}/lib"
set (STATIC_LIBRARY_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib") # default: "${CMAKE_CURRENT_BINARY_DIR}/lib"
set (AUTOGEN_TARGETS_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/autogen")
# Define the gcc / g++ / clang / clang++ compilation flags (CFLAGS)
#  common to debug, dev and release compilation
set (GCC_COMMON_COMPILATION_FLAGS "-Wall -Wextra") # default: "-Wall -Wextra"
#  debug compilation only
set (GCC_DEBUG_COMPILATION_FLAGS "-O0 -ggdb -DDEBUG") # default: "-O0 -ggdb -DDEBUG"
#  release compilation only
set (GCC_RELEASE_COMPILATION_FLAGS "-O2 -Werror -DRELEASE") # default: "-O2 -Werror -DRELEASE"

# Define the Microsoft Visual Studio compilation flags
#  common to debug, dev and release compilation
set (MSVC_COMMON_COMPILATION_FLAGS "") # default: ""
#  debug compilation only
set (MSVC_DEBUG_COMPILATION_FLAGS "") # default: ""
#  release compilation only
set (MSVC_RELEASE_COMPILATION_FLAGS "") # default: ""
