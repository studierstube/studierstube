TEMPLATE	= lib
CONFIG		= qt warn_on debug

QMAKE_CLEAN = $$(STB4)/lib/*simple*

DESTDIR  = $$(STB4)/lib

SOURCES		= SimpleApp.cxx

INTERFACES	= 

TARGET = simple

DEFINES += LINUX

INCLUDEPATH += $$(STB4)/include

LIBS += -lCoin -L$$(STB4)/lib -lstbkernel -lstbevent
