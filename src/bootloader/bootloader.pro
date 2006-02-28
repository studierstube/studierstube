#################################################
#
# QMake definitions for bootloader
#

include ($$(STB4)/build/linux/options.pro)

TEMPLATE	= app

TARGET          = stb4

CONFIG	       += qt

DESTDIR  = $$(STB4)/bin/linux

QMAKE_CLEAN = $$(STB4)/bin/linux/stb4

INCLUDEPATH = $$(STB4)/include

target.path = $$INSTALL_PATH/lib
INSTALLS += target

SOURCES		= KernelLoader.cxx \
		  main.cxx

LIBS = -ldl -L$$(STB4)/src/base -lstbbase -lACE
