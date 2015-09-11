message (STATUS
  "\n  This script builds the project.
  Options can be used in order to configure the process:
    DEBUG: Enable debug compilation
    RELEASE: Enable release compilation
  To set an option simply type -D<OPTION>=<VALUE> after 'cmake <path>'
    For example: cmake .. -DDEBUG=1"
)

# cmake -DDEBUG=1
if (DEBUG)
  message (STATUS "DEBUG option catched, switching to debug compilation")
  set (CMAKE_BUILD_TYPE "Debug")
endif (DEBUG)

# cmake -DRELEASE=1
if (NOT DEBUG AND RELEASE)
  message (STATUS "RELEASE option catched, switching to release compilation")
  set (CMAKE_BUILD_TYPE "Release")
endif (NOT DEBUG AND RELEASE)


# To avoid problems, it's better to clean the cache
set (DEBUG "" CACHE "String" "" FORCE)
set (RELEASE "" CACHE "String" "" FORCE)
