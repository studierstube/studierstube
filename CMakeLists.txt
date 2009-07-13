project(studierstube)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true) # Avoid unneccesary code
cmake_minimum_required(VERSION 2.6.0)

file(TO_CMAKE_PATH "$ENV{ICG_CMAKE_MODULE_PATH}" OT_CMAKE_MODULE_PATH)
set(CMAKE_MODULE_PATH ${OT_CMAKE_MODULE_PATH})


# specify the lib and dll output folders for win32
if(WIN32)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${studierstube_SOURCE_DIR}/bin/win32)
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${studierstube_SOURCE_DIR}/lib/win32)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${studierstube_SOURCE_DIR}/lib/win32)
endif(WIN32)

find_package(Boost 1.33 REQUIRED)
find_package(ACE REQUIRED)
find_package(Coin REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL true)
include_directories(${Boost_INCLUDE_DIR})

find_package(Quarter)
if (NOT QUARTER_FOUND)
   find_package(SoQt REQUIRED)
   include_directories(${SOQT_INCLUDE_DIR})
   add_definitions(${SOQT_DEFINITIONS})
   link_directories(${SOQT_LINK_DIRECTORIES})
else (NOT QUARTER_FOUND)
   include_directories(${QUARTER_INCLUDE_DIR})
   add_definitions(${QUARTER_DEFINITIONS} -DQUARTER_DLL)
   link_directories(${QUARTER_LINK_DIRECTORIES})
endif (NOT QUARTER_FOUND)

find_package(OpenTracker REQUIRED)
find_package(OpenVideo REQUIRED)

find_package(TinyXMLMod)
if(NOT TINY_XML_MOD_FOUND)
  find_package(TinyXML)
  if(NOT TINY_XML_FOUND)
    message(FATAL_ERROR "neither TinyXML nor TinyXML_Mod could be found")
  endif(NOT TINY_XML_FOUND)
else(NOT TINY_XML_MOD_FOUND)
  add_definitions(-DUSE_TINYXMLMOD)
  if(WIN32)
     add_definitions(-DTINYXML_MOD_DLL)
     add_definitions(-DOT_DO_NOT_LINK_TINYXML_VIA_PRAGMA_COMMENT_LIB) # This is a workaround for win32
  endif(WIN32)
  set(TINY_XML_INCLUDE_DIR ${TINY_XML_MOD_INCLUDE_DIR})
  set(TINY_XML_LIBRARY_DIR ${TINY_XML_MOD_LIBRARY_DIR})
  set(TINY_XML_LIBRARY ${TINY_XML_MOD_LIBRARIES})
endif(NOT TINY_XML_MOD_FOUND)

set(CPACK_GENERATOR "TBZ2")
set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION_MAJOR "4")
set(CPACK_PACKAGE_VERSION_MINOR "3")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR})
set(CPACK_PACKAGE_NAME "studierstube")

# RPM variables:
if(NOT CMAKE_INSTALL_BINDIR)
set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
endif(NOT CMAKE_INSTALL_BINDIR)
if(NOT CMAKE_INSTALL_LIBDIR)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
endif(NOT CMAKE_INSTALL_LIBDIR)
if(NOT CMAKE_INSTALL_DATADIR)
set(CMAKE_INSTALL_DATADIR ${CMAKE_INSTALL_PREFIX}/share)
endif(NOT CMAKE_INSTALL_DATADIR)
if(NOT CMAKE_INSTALL_DOCDIR)
set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATADIR}/doc/packages)
endif(NOT CMAKE_INSTALL_DOCDIR)
if(NOT CMAKE_INSTALL_MANDIR)
set(CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_DATADIR}/man)
endif(NOT CMAKE_INSTALL_MANDIR)

if(UNIX)
   # configure stb4.pc:
   set(STB4_PREFIX ${CMAKE_INSTALL_PREFIX})
   set(STB4_LIBDIR ${CMAKE_INSTALL_LIBDIR})
   set(STB4_PROJECT_NAME ${CPACK_PACKAGE_NAME})
   set(STB4_PROJECT_VERSION ${CPACK_PACKAGE_VERSION})
   set(STB4_PROJECT_DESCRIPTION "Studierstube - The Augmented Reality Environment")
   set(STB4_PROJECT_LIBNAME ${CPACK_PACKAGE_NAME})
   set(STB4_PROJECT_DEFINES "")
   
   configure_file(stb4.pc.in stb4.pc @ONLY)
   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/stb4.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
   configure_file(studierstube.spec.in studierstube.spec)
endif(UNIX)

set(BUILD_SHARED_LIBS ON CACHE DOC "build the shared lib instead of the static?")

if(WIN32)
   add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
   add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
   add_definitions(-D_CRT_SECURE_NO_DEPRECATE_GLOBALS)
   # configure debug and static library postfixes
   if(BUILD_SHARED_LIBS)
      set(CMAKE_DEBUG_POSTFIX "d")
   else(BUILD_SHARED_LIBS)
      set(CMAKE_DEBUG_POSTFIX "d_static")
      set(CMAKE_RELEASE_POSTFIX "_static")
   endif(BUILD_SHARED_LIBS)
endif(WIN32)

# testing using CTest/Dart:
include(CTest)

# ACE
include_directories(${ACE_INCLUDE_DIR})
link_directories(${ACE_LIBRARY_DIR})

# Coin:
include_directories(${COIN_INCLUDE_DIR})
add_definitions(${COIN_DEFINITIONS})
link_directories(${COIN_LINK_DIRECTORIES})

# TinyXML or TinyXMLMod
include_directories(${TINY_XML_INCLUDE_DIR})
add_definitions(${TINY_XML_MOD_DEFINITIONS})
link_directories(${TINY_XML_LIBRARY_DIR})

# OpenVideo
if(OPENVIDEO_FOUND)
   include_directories(${OPENVIDEO_INCLUDE_DIRS})
   add_definitions(${OPENVIDEO_DEFINITIONS})
   link_directories(${OPENVIDEO_LIBRARY_DIRS})
endif()

# OpenTracker
if(OPENTRACKER_FOUND)
   include_directories(${OPENTRACKER_INCLUDE_DIRS})
   add_definitions(${OPENTRACKER_DEFINITIONS})
   link_directories(${OPENTRACKER_LIBRARY_DIRS})
endif()

# Boost
include_directories(${Boost_INCLUDE_DIRS})

# Studierstube itself
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(doc)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN ".svn" EXCLUDE)
install(FILES StudierstubeConfig.cmake DESTINATION lib/Studierstube)

# packaging:

set(CPACK_IGNORE_FILES
  ".depend"
  "/#.*#$"
  "/debian"
  "/doc/doc/"
  "/files.stb"
  "/log"
  "/rpm.stb"
  "/trunk/build/"
  "CMakeCache\\\\.txt"
  "CMakeFiles"
  "CPack.*Config\\\\.cmake"
  "CVS"
  "Makefile"
  "SCons*"
  "\\\\.#"
  "\\\\.a$"
  "\\\\.aux$"
  "\\\\.bz2$"
  "\\\\.dsp$"
  "\\\\.dvi$"
  "\\\\.eps$"
  "\\\\.gz$"
  "\\\\.iv$"
  "\\\\.log$"
  "\\\\.o$"
  "\\\\.out"
  "\\\\.pgm$"
  "\\\\.pro$"
  "\\\\.ps$"
  "\\\\.save"
  "\\\\.so"
  "\\\\.svn"
  "\\\\.swp$"
  "\\\\.tbz$"
  "\\\\.tex$"
  "\\\\.tgz$"
  "\\\\.vcproj$"
  "^/box.cpp"
  "_CPack_Packages"
  "a.out"
  "cgparams$"
  "cmake_install.cmake"
  "cvs_import"
  "install.sh"
  "install_manifest.txt"
  "makedist.sh"
  "moc_"
  "progress.make"
  "~$"
)

set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
include(CPack)
add_custom_target(dist COMMAND make clean && make package_source)
add_custom_target(clean-all COMMAND find \\\( -name CMakeFiles -or -name CMakeCache.txt -or -name Makefile -or -name DartTestfile.txt -or -name 'CPack*.cmake' -or -name _CPack_Packages -or -name '*~' \\\) -prune -exec rm -rf {} '\;')
