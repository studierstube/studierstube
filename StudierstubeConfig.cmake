# - Configure the studierstube library
# Once done this will define
#
#  STUDIERSTUBE_FOUND - system has STUDIERSTUBE
#  STUDIERSTUBE_INCLUDE_DIRS - the STUDIERSTUBE include directories
#  STUDIERSTUBE_LIBRARIES - Link these to use STUDIERSTUBE
#  STUDIERSTUBE_LIBRARY_DIRS - link directories, useful for rpath
#

if(STUDIERSTUBE_INCLUDE_DIRS AND STUDIERSTUBE_LIBRARY_DIRS)

  # in cache already:
  set(STUDIERSTUBE_FOUND TRUE)

else(STUDIERSTUBE_INCLUDE_DIRS AND STUDIERSTUBE_LIBRARY_DIRS)

  get_filename_component(STUDIERSTUBE_ROOT "${CMAKE_CURRENT_LIST_FILE}" PATH)

  if(NOT EXISTS ${STUDIERSTUBE_ROOT}/CMakeLists.txt)
    # we are *not* in the source tree, so find the actual root from here:
    get_filename_component(STUDIERSTUBE_ROOT "${STUDIERSTUBE_ROOT}" PATH)
    get_filename_component(STUDIERSTUBE_ROOT "${STUDIERSTUBE_ROOT}" PATH)
  endif(NOT EXISTS ${STUDIERSTUBE_ROOT}/CMakeLists.txt)

  set(STUDIERSTUBE_INCLUDE_DIRS "${STUDIERSTUBE_ROOT}/include" CACHE STRING "Studierstube include directories")
  set(STUDIERSTUBE_LIBRARIES "stbevent stbkernel" CACHE STRING "Studierstube libraries names")

  # what about stbstarlight, stbvideo, stbviewer_qt?

  # search for the library (if we are in the source tree, it might not exist):
  find_library(STUDIERSTUBE_LIBRARY_PATH stbkernel PATHS
    "${STUDIERSTUBE_ROOT}/lib"
    "${STUDIERSTUBE_ROOT}/lib/Release"
    "${STUDIERSTUBE_ROOT}/lib${LIBSUFFIX}"
    "${STUDIERSTUBE_ROOT}/lib64"
    "${STUDIERSTUBE_ROOT}/lib/win32"
    # make sure not to mix locally installed headers with globally installed binaries:
    NO_DEFAULT_PATH
    )

  if(STUDIERSTUBE_LIBRARY_PATH)
    # store library directories in cache:
    get_filename_component(STUDIERSTUBE_LIBRARY_DIRS "${STUDIERSTUBE_LIBRARY_PATH}" PATH)
    set(STUDIERSTUBE_LIBRARY_DIRS "${STUDIERSTUBE_LIBRARY_DIRS}" CACHE STRING "Studierstube library directories")
    set(STUDIERSTUBE_FOUND TRUE)
  endif(STUDIERSTUBE_LIBRARY_PATH)

  if(STUDIERSTUBE_FOUND)
    if(NOT Studierstube_FIND_QUIETLY)
      message(STATUS "Found Studierstube: ${STUDIERSTUBE_LIBRARY_DIRS} ${STUDIERSTUBE_INCLUDE_DIRS}")
    endif(NOT Studierstube_FIND_QUIETLY)
  else(STUDIERSTUBE_FOUND)
    if(Studierstube_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find Studierstube")
    endif(Studierstube_FIND_REQUIRED)
  endif(STUDIERSTUBE_FOUND)

endif(STUDIERSTUBE_INCLUDE_DIRS AND STUDIERSTUBE_LIBRARY_DIRS)
