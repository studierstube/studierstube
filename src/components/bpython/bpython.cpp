// bpython.cpp : Defines the entry point for the WIN32 DLL.

#include <stb/components/bpython/bpython.h>


#include <stb/components/bpython/SoPyScript.h>




CREATE_COMPONENT_FUNC(BPython)

BEGIN_NAMESPACE_STB

/**
*     The Constructor	
*/
BPython::BPython()
{
    //nil
}
/**
*     The destructor.
*/
BPython::~BPython()
{
    //nil
}
/// Called before the application is destructed.
void 
BPython::shutDown()
{

}

bool
BPython::init(void)
{
    if(isInit)
        return true;

    isInit=true;
    SoPyScript::initClass();

    return isInit;
}


SoPyScript* 
BPython::createSoPyScript()
{
    return new SoPyScript;
}


END_NAMESPACE_STB
