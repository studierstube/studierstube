TEMPLATE	= lib
CONFIG		= qt warn_on debug

QMAKE_CLEAN = $$(STB4)/lib/*stbvideo*

DEFINES        += LINUX

DESTDIR  = $$(STB4)/lib

SOURCES		= Video.cxx

INTERFACES	= 

TARGET = stbvideo

INCLUDEPATH += $$(STB4)/include

LIBS += -lCoin -lACE -L$$(STB4)/lib -lstbkernel -lopenvideo
