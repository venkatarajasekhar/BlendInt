#
# Try to find GLFW library and include path.
# Once done this will define
#
# GLFW_FOUND
# GLFW_INCLUDE_DIR
# GLFW_LIBRARIES
#

if(NOT GLFW_FOUND)

FIND_PATH(GLFW_INCLUDE_DIR GLFW/glfw3.h
  PATHS
    /usr/local/include
    /usr/X11/include
    /usr/include
    /opt/local/include
    NO_DEFAULT_PATH
    )

FIND_LIBRARY( GLFW_LIBRARIES NAMES glfw glfw3
  PATHS
    /usr/local
    /usr/X11
    /usr
    PATH_SUFFIXES
    a
    lib64
    lib
    NO_DEFAULT_PATH
)

SET(GLFW_FOUND "NO")
IF (GLFW_INCLUDE_DIR AND GLFW_LIBRARIES)
	SET(GLFW_FOUND "YES")
ENDIF (GLFW_INCLUDE_DIR AND GLFW_LIBRARIES)

if(GLFW_FOUND)
  message(STATUS "Found GLFW: ${GLFW_INCLUDE_DIR}")
else(GLFW_FOUND)
  message(FATAL_ERROR "could NOT find GLFW")
endif(GLFW_FOUND)

endif(NOT GLFW_FOUND)
