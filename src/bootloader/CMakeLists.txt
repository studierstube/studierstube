FILE(GLOB workspace_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.cxx ../base/string.cxx ../base/OS.cxx)
FILE(GLOB workspace_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../../include/stb/base/*.h ../../include/stb/bootloader/*.h)

ADD_EXECUTABLE(workspace ${workspace_HEADERS} ${workspace_SOURCES})
TARGET_LINK_LIBRARIES(workspace ${ACE_LIBRARIES})

INSTALL(TARGETS workspace DESTINATION ${CMAKE_INSTALL_BINDIR})
