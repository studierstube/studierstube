#ifndef  _InventorUtils_
#define  _InventorUtils_

#include <Inventor/nodes/SoSeparator.h>

class InventorUtils
{
public:
    virtual ~InventorUtils(){}

    InventorUtils(){}

    SoSeparator*loadFile(const char* filename);

    SoNode* find(const SoType type,SoNode* start);
    SbBool findAll(const SoType type,SoNode* start,SoPathList& paths);
};

#endif
