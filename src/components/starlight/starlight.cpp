// starlight.cpp : Defines the entry point for the WIN32 DLL.

#include <stb/components/starlight/starlight.h>


#include <stb/components/starlight/SoAbortGroup.h>
#include <stb/components/starlight/SoClassLoader.h>
#include <stb/components/starlight/SoConditionalTrigger.h>
#include <stb/components/starlight/SoContextElement.h>
#include <stb/components/starlight/SoContext.h>
#include <stb/components/starlight/SoContextMultiSwitch.h>
#include <stb/components/starlight/SoContextReport.h>
#include <stb/components/starlight/SoContextSeparator.h>
#include <stb/components/starlight/SoContextSwitch.h>
#include <stb/components/starlight/SoEaseIn.h>
#include <stb/components/starlight/SoEngineWrapper.h>
#include <stb/components/starlight/SoFactorRotation.h>
#include <stb/components/starlight/SoFanIn.h>
#include <stb/components/starlight/SoFileSubgraph.h>
#include <stb/components/starlight/SoFrameBuffer.h>
#include <stb/components/starlight/SoGLFrameBufferElement.h>
#include <stb/components/starlight/SoGLStencilBufferElement.h>
#include <stb/components/starlight/SoKeyToggle.h>
#include <stb/components/starlight/SoMultiSwitchElement.h>
#include <stb/components/starlight/SoMultiSwitch.h>
#include <stb/components/starlight/SoMultMatrix.h>
#include <stb/components/starlight/SoNodeContext.h>
#include <stb/components/starlight/SoNodeContextReport.h>
#include <stb/components/starlight/SoNodeToName.h>
#include <stb/components/starlight/SoRoute.h>
#include <stb/components/starlight/SoSFFieldContainer.h>
#include <stb/components/starlight/SoStencilBuffer.h>
#include <stb/components/starlight/SoStringCalculator.h>
#include <stb/components/starlight/SoStringMap.h>
#include <stb/components/starlight/SoUse.h>
#include <stb/components/starlight/SoVariable.h>
#include <stb/components/starlight/SoExtrusionKit.h>
#include <stb/components/starlight/SoLineStringKit.h>
#include <stb/components/starlight/SoPipeKit.h>
#include <stb/components/starlight/SoPhantomShape.h>


CREATE_COMPONENT_FUNC(Starlight)

BEGIN_NAMESPACE_STB

/**
*     The Constructor	
*/
Starlight::Starlight()
{
    //nil
}
/**
*     The destructor.
*/
Starlight::~Starlight()
{
    //nil
}
/// Called before the application is destructed.
void 
Starlight::shutDown()
{

}

bool
Starlight::init(void)
{
    if(isInit)
        return true;
    isInit=true;
    SoClassLoader::initClass();
    SoConditionalTrigger::initClass();
    SoContextElement::initClass();
    SoContext::initClass();
    SoContextMultiSwitch::initClass();
    SoContextReport::initClass();
    SoContextSeparator::initClass();
    SoContextSwitch::initClass();
    SoEaseIn::initClass();
    SoEngineWrapper::initClass();
    SoFactorRotation::initClass();
    SoFanIn::initClass();
    SoFileSubgraph::initClass();
    SoFrameBuffer::initClass();
    SoGLFrameBufferElement::initClass();
    SoGLStencilBufferElement::initClass();
    SoKeyToggle::initClass();
    SoMultiSwitchElement::initClass();
    SoMultiSwitch::initClass();
    SoMultMatrix::initClass();
    SoNodeContext::initClass();
    SoNodeContextReport::initClass();
    SoNodeToName::initClass();
    SoRoute::initClass();
    SoSFFieldContainer::initClass();
    SoStencilBuffer::initClass();
    SoStringCalculator::initClass();
    SoStringMap::initClass();
    SoUse::initClass();
    SoVariable::initClass();


	// Ericks Nodes and Node Kits
	SoExtrusionKit::initClass();
	SoLineStringKit::initClass();
	SoPipeKit::initClass();
	SoPhantomShape::initClass();



    return isInit;
}
END_NAMESPACE_STB
