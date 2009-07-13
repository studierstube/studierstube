file(GLOB_RECURSE stbviewer_qt_SOURCES *.cpp *.cxx)
file(GLOB stbviewer_qt_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../../../include/stb/components/viewer/*.h)

if(WIN32)
   add_definitions(-DVIEWER_EXPORTS)
endif(WIN32)

# Qt:
include(${QT_USE_FILE})

if (QUARTER_FOUND)
   # Quarter
   add_definitions(-DUSE_QUARTER)
else (QUARTER_FOUND)
   # SoQt:
   add_definitions(-DUSE_SOQT)
endif (QUARTER_FOUND)

add_library(stbviewer_qt ${stbviewer_qt_HEADERS} ${stbviewer_qt_SOURCES})
target_link_libraries(stbviewer_qt stbkernel ${QT_LIBRARIES})

if (QUARTER_FOUND)
   target_link_libraries(stbviewer_qt ${QUARTER_LIBRARIES})
else (QUARTER_FOUND)
   target_link_libraries(stbviewer_qt ${SOQT_LIBRARIES})
endif (QUARTER_FOUND)

set_target_properties(stbviewer_qt PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})
install(TARGETS stbviewer_qt DESTINATION ${CMAKE_INSTALL_LIBDIR})
