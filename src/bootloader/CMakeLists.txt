file(GLOB workspace_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cxx ../base/string.cxx ../base/OS.cxx)
file(GLOB workspace_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../../include/stb/base/*.h ../../include/stb/bootloader/*.h)

add_executable(workspace ${workspace_HEADERS} ${workspace_SOURCES})
target_link_libraries(workspace ${ACE_LIBRARIES})

install(TARGETS workspace DESTINATION ${CMAKE_INSTALL_BINDIR})
