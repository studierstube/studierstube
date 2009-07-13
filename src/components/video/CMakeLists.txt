file(GLOB stbvideo_SOURCES *.cpp *.cxx)
file(GLOB stbvideo_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../../../include/stb/components/video/*.h)

if(WIN32)
   add_definitions(-DVIDEO_EXPORTS)
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

if(UNIX)
   add_definitions(-DOV_IS_LINUX)
endif(UNIX)

add_library(stbvideo ${stbvideo_HEADERS} ${stbvideo_SOURCES})
target_link_libraries(stbvideo stbkernel ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${OPENVIDEO_LIBRARIES})

set_target_properties(stbvideo PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})
install(TARGETS stbvideo DESTINATION ${CMAKE_INSTALL_LIBDIR})
