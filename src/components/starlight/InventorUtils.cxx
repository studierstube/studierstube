#include <stb/components/starlight/InventorUtils.h>
#include <iostream>
#include <stb/kernel/StbLogger.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/SoPath.h> 

SoSeparator*
InventorUtils::loadFile(const char* filename)
{
    SoInput::addDirectoryFirst("./");
    SoInput myinput;
    if (!myinput.openFile(filename)) 
    {
        stb::logPrintI("InventorUtils: can not open file: %s\n",filename);
        SoInput::removeDirectory("./");
        return false;
    }
    SoSeparator* root=NULL;
    root=SoDB::readAll(&myinput);
    myinput.closeFile();
    SoInput::removeDirectory("./");
    if (root==NULL) 
    {
        stb::logPrintI("InventorUtils: problem reading file:%s\n",filename);
        return false;
    }

    stb::logPrintI("InventorUtils: read:%s\n",filename);
    return root;   
}

SoNode*
InventorUtils::find(const SoType type/*SoNode::getClassTypeId()*/,SoNode* start)
{

    SoPathList paths;
    if(!findAll(type,start,paths))
        return NULL;

    return paths[0]->getTail();   
}




SbBool 
InventorUtils::findAll(const SoType type/*SoNode::getClassTypeId()*/,SoNode* start,SoPathList& paths)
{
    SoSearchAction sAction;
    sAction.reset();
    sAction.setType(type);
    sAction.setSearchingAll(TRUE);
    sAction.setInterest(SoSearchAction::ALL);
    sAction.apply(start);
    paths = sAction.getPaths();

    if(paths.getLength()==0){
        return false;
    }

    return true;
}