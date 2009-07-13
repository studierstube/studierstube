file(GLOB stbstarlight_SOURCES *.cpp *.cxx)
file(GLOB stbstarlight_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../../../include/stb/components/starlight/*.h)

if(WIN32)
   add_definitions(-DSTARLIGHT_EXPORTS)
endif(WIN32)

add_library(stbstarlight ${stbstarlight_HEADERS} ${stbstarlight_SOURCES})
target_link_libraries(stbstarlight stbkernel ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

if (UNIX)
target_link_libraries(stbstarlight ltdl)
endif (UNIX)

set_target_properties(stbstarlight PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})
install(TARGETS stbstarlight DESTINATION ${CMAKE_INSTALL_LIBDIR})
