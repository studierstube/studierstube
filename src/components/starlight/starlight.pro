TEMPLATE	= lib
CONFIG		= warn_on debug

QMAKE_CLEAN = $$(STB4)/lib/*stbviewer*

DESTDIR  = $$(STB4)/lib

SOURCES		= GLtools.cpp \
		  SoAbortGroup.cpp \
		  SoClassLoader.cpp \
		  SoConditionalTrigger.cpp \
		  SoContext.cpp \
		  SoContextElement.cpp \
		  SoContextMultiSwitch.cpp \
		  SoContextReport.cpp \
		  SoContextSeparator.cpp \
		  SoContextSwitch.cpp \
		  SoEaseIn.cpp \
		  SoEngineWrapper.cpp \
		  SoFactorRotation.cpp \
		  SoFanIn.cpp \
		  SoFileSubgraph.cpp \
		  SoFrameBuffer.cpp \
		  SoGLFrameBufferElement.cpp \
		  SoGLStencilBufferElement.cpp \
		  SoKeyToggle.cpp \
		  SoMultMatrix.cpp \
		  SoMultiSwitch.cpp \
		  SoMultiSwitchElement.cpp \
		  SoNodeContext.cpp \
		  SoNodeContextReport.cpp \
		  SoNodeToName.cpp \
		  SoRoute.cpp \
		  SoSFFieldContainer.cpp \
		  SoStencilBuffer.cpp \
		  SoStringCalculator.cpp \
		  SoStringMap.cpp \
		  SoUse.cpp \
		  SoVariable.cpp \
		  starlight.cpp
INTERFACES	= 

TARGET = starlight

INCLUDEPATH += $$(STB4)/include

LIBS += -lCoin -lSoQt -L$$(STB4)/lib
