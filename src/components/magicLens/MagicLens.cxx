/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The header file for the MagicLens class.
*
* @author Denis Kalkofen
*
* $Id: MagicLens.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */
#include <stb/components/magicLens/MagicLens.h>

#include <stb/kernel/Kernel.h>
#include <stb/kernel/SceneManager.h>
#include <stb/components/starlight/starlight.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>

#include <stb/components/magicLens/SoMagicLens.h>
#include <stb/components/magicLens/SoMagicLensKit.h>
#include <stb/components/magicLens/SoMagicScene.h>
#include <stb/components/magicLens/SoMagicSceneKit.h>
#include <stb/components/magicLens/SoMagicStylesKit.h>

CREATE_COMPONENT_FUNC(MagicLens)

BEGIN_NAMESPACE_STB

Starlight*	MagicLens::starlight=NULL;

MagicLens::MagicLens()
{
   configFile="";
  
}

MagicLens::~MagicLens()
{
   //nil
}

//static
Starlight* 
MagicLens::getStarlight()
{
    if(starlight == NULL)
            starlight=(Starlight*)(stb::Kernel::getInstance()->getComponentManager()->load("Starlight"));

    return starlight;
}

/// Called before the application is destructed.
bool 
MagicLens::init()
{
    stb::Kernel::getInstance()->logDebug("init MagicLens\n");

    if(isInit)
        return true;
   
    starlight=(Starlight*)(stb::Kernel::getInstance()->getComponentManager()->load("Starlight"));
    if(!starlight)
    {
        return false;
    }
    // init nodes
    SoMagicLens::initClass();
    SoMagicLensKit::initClass();
    SoMagicScene::initClass();
    SoMagicSceneKit::initClass();
    SoMagicStylesKit::initClass();
    //get MagicLens's parameter
    retrieveParameter();
    isInit=true;
    return isInit;
}

void 
MagicLens::setParameter(stb::string key, std::string value)
{
    if(key=="configFile")
    {
        configFile=value;
    }
    //else if()
    //{
    //}
}

/// Called before the application is destructed.
void 
MagicLens::shutDown()
{

}

END_NAMESPACE_STB
//========================================================================
// End of MagicLens.cxx
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
