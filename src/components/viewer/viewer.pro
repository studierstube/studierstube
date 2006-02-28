TEMPLATE	= lib
CONFIG		= qt warn_on debug

QMAKE_CLEAN = $$(STB4)/lib/*stbviewer*

DESTDIR  = $$(STB4)/lib

DEFINES += USE_SOQT

SOURCES		= SoDisplay.cxx \
		  SoOffAxisCamera.cxx \
		  SoStbCamera.cxx \
		  SoStudierstubeViewer.cxx \
		  Viewer.cxx \
		  SoViewport.cxx \
		  SoVideoBackground.cxx \
		  displaymode/SoAnaglyphDisplayMode.cxx \
		  displaymode/SoDisplayMode.cxx \
		  displaymode/SoFieldSequentialDisplayMode.cxx \
		  displaymode/SoGLColorBufferElement.cxx \
		  displaymode/SoGLPolygonStippleElement.cxx \
		  displaymode/SoLineSequentialDisplayMode.cxx \
		  controlmode/SoDesktopControlMode.cxx \
		  controlmode/SoStbCameraControlMode.cxx \
                  controlmode/SoTrackedViewpointMobileDisplayControlMode.cxx \
                  controlmode/SoTrackedViewpointControlMode.cxx \
                  controlmode/SoTrackedDisplayControlMode.cxx \
		  controlmode/MultRotRot.cxx

INTERFACES	= 

TARGET = stbviewer

INCLUDEPATH += $$(STB4)/include

LIBS += -lCoin -lSoQt -L$$(STB4)/lib -lstbkernel
