TEMPLATE	= lib
CONFIG		= debug

QMAKE_CLEAN = $$(STB4)/lib/*stbevent*

DESTDIR  = $$(STB4)/lib

HEADERS		= EventBus.h \
		  EventContext.h \
		  EventContextFilter.h \
		  EventKeyboardSource.h \
		  EventModule.h \
		  EventMouseSource.h \
		  EventSchema.h \
		  EventSink.h \
		  EventSource.h \
		  SoEventAction.h \
		  SoEventKit.h \
		  SoEventNode.h \
		  SoEventGroup.h \
		  SoEventSeparator.h \
		  SoEventRoot.h \
		  SoEventSource.h \
		  SoInputEvent.h \
		  SoOpenTrackerSource.h \
		  SoTrackedArtifactKit.h \
		  SoTrakEngine.h \
		  event.h \
		  tools.h
SOURCES		= EventBus.cpp \
		  EventContext.cpp \
		  EventContextFilter.cpp \
		  EventModule.cpp \
		  EventMouseSource.cpp \
		  EventSink.cpp \
		  EventSource.cpp \
		  SoEventAction.cpp \
		  SoEventKit.cpp \
		  SoEventNode.cpp \
		  SoEventGroup.cpp \
		  SoEventSeparator.cpp \
		  SoEventRoot.cpp \
		  SoEventSource.cpp \
		  SoInputEvent.cpp \
		  SoOpenTrackerSource.cpp \
		  SoTrackedArtifactKit.cpp \
		  SoTrakEngine.cpp \
		  event.cpp
INTERFACES	= 

TARGET = stbevent

INCLUDEPATH += /data/home/breiting/devel/stb4/src

LIBS += -lCoin -L$$(STB4)/lib -lstbutils

