# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Peggle_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Peggle_autogen.dir\\ParseCache.txt"
  "Peggle_autogen"
  )
endif()
