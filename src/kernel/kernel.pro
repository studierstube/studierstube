TEMPLATE	= lib
CONFIG		= warn_on debug

QMAKE_CLEAN = $$(STB4)/lib/*stbkernel*

DESTDIR  = $$(STB4)/lib

SOURCES		= Application.cxx \
		  Component.cxx \
                  ComponentThread.cxx \
		  ComponentInfo.cxx \
		  ComponentManager.cxx \
		  ComponentRetriever.cxx \
		  Config.cxx \
		  Kernel.cxx \
		  SceneManager.cxx \
		  SchedulerBase.cxx \
		  SchedulerLinux.cxx \
		  Studierstube.cxx \
                  interfaces/SoTrakEngineInterface.cxx \
                  ../base/string.cxx \
                  ../base/OS.cxx
INTERFACES	= 

TARGET = stbkernel

INCLUDEPATH += $$(STB4)/include /usr/local/include/tinyxml

LIBS += -lACE -lCoin -ltinyxml
