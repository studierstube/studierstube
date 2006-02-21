TEMPLATE	= lib
CONFIG		= warn_on debug

QMAKE_CLEAN = $$(STB4)/lib/*stbevent*

DESTDIR  = $$(STB4)/lib

DEFINES += LINUX

HEADERS		= 

SOURCES         = EventBus.cxx \
                  EventContext.cxx \
                  EventContextFilter.cxx \
                  EventModule.cxx \
                  EventMouseSource.cxx \
                  EventSink.cxx \
                  EventSource.cxx \
                  SoEventAction.cxx \
                  SoEventGroup.cxx \
                  SoEventKit.cxx \
                  SoEventNode.cxx \
                  SoEventRoot.cxx \
                  SoEventSeparator.cxx \
                  SoEventSource.cxx \
                  SoInputEvent.cxx \
                  SoOpenTrackerSource.cxx \
                  SoTrackedArtifactKit.cxx \
                  SoTrakEngine.cxx \
                  event.cxx

INTERFACES	= 

TARGET = stbevent

INCLUDEPATH += $$(STB4)/include

LIBS += -lCoin -L$$(STB4)/lib -lstbkernel -lstarlight -lOpenTracker

