file(GLOB stbevent_SOURCES *.cpp *.cxx)
file(GLOB stbevent_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../../../include/stb/components/event/*.h)

if(WIN32)
   add_definitions(-DEVENT_EXPORTS)
endif(WIN32)

add_library(stbevent ${stbevent_HEADERS} ${stbevent_SOURCES})
target_link_libraries(stbevent stbkernel ${OPENTRACKER_LIBRARIES})

set_target_properties(stbevent PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})
install(TARGETS stbevent DESTINATION ${CMAKE_INSTALL_LIBDIR})
