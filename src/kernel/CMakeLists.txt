file(GLOB_RECURSE stbkernel_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cxx ../base/*.cxx ../base/*.cpp)
file(GLOB_RECURSE stbkernel_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../../include/stb/kernel/*.h ../../include/stb/base/*.h)

if(WIN32)
   add_definitions(-DKERNEL_EXPORTS)
endif(WIN32)

add_library(stbkernel ${stbkernel_HEADERS} ${stbkernel_SOURCES})
target_link_libraries(stbkernel ${COIN_LIBRARIES} ${ACE_LIBRARIES} ${TINY_XML_LIBRARY})

set_target_properties(stbkernel PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})
install(TARGETS stbkernel DESTINATION ${CMAKE_INSTALL_LIBDIR})
