# You can configure your CMake configuration in this file
include ("./cmake/config.cmake")

# Don't change anything in the following files
include ("./cmake/options.cmake")	# parse users options given to CMake
include ("./cmake/compilation.cmake")	# compilation stuff
include ("./cmake/module.cmake")	# module configuration
include ("./cmake/system.cmake")	# system detection
