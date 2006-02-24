/* ========================================================================
 * Copyright (C) 2000-2005  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** impl file for SoTrakEngineInterface engine
  *
  * @author Denis Kalkofen
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/engines/SoSubEngine.h>
#include <stb/kernel/interfaces/SoTrakEngineInterface.h>

BEGIN_NAMESPACE_STB

SO_ENGINE_SOURCE(SoTrakEngineInterface);

void SoTrakEngineInterface::initClass(void)
{    
    if( SoType::fromName("SoTrakEngineInterface").isBad() )
    {
        SO_ENGINE_INIT_CLASS(SoTrakEngineInterface,SoEngine,"Engine");
    }
}

SoTrakEngineInterface::SoTrakEngineInterface() 
{
    SO_ENGINE_CONSTRUCTOR(SoTrakEngineInterface);

    SO_ENGINE_ADD_INPUT(key,(""));
    SO_ENGINE_ADD_INPUT(value,(""));
	
    SO_ENGINE_ADD_INPUT(translationIn, (0,0,0));
    SO_ENGINE_ADD_INPUT(rotationIn, (0,0,0,1));
    SO_ENGINE_ADD_INPUT(buttonIn0, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn1, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn2, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn3, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn4, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn5, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn6, (FALSE));
    SO_ENGINE_ADD_INPUT(buttonIn7, (FALSE));

    SO_ENGINE_ADD_OUTPUT(translation, SoSFVec3f);
    SO_ENGINE_ADD_OUTPUT(rotation, SoSFRotation);
    SO_ENGINE_ADD_OUTPUT(button0, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button1, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button2, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button3, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button4, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button5, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button6, SoSFBool);
    SO_ENGINE_ADD_OUTPUT(button7, SoSFBool);
}

SoTrakEngineInterface::~SoTrakEngineInterface()
{
    //nil
}

void SoTrakEngineInterface::evaluate() 
{
    SO_ENGINE_OUTPUT(translation,SoSFVec3f,setValue(translationIn.getValue()));
    SO_ENGINE_OUTPUT(rotation,SoSFRotation,setValue(rotationIn.getValue()));
    SO_ENGINE_OUTPUT(button0,SoSFBool,setValue(buttonIn0.getValue()));
    SO_ENGINE_OUTPUT(button1,SoSFBool,setValue(buttonIn1.getValue()));
    SO_ENGINE_OUTPUT(button2,SoSFBool,setValue(buttonIn2.getValue()));
    SO_ENGINE_OUTPUT(button3,SoSFBool,setValue(buttonIn3.getValue()));
    SO_ENGINE_OUTPUT(button4,SoSFBool,setValue(buttonIn4.getValue()));
    SO_ENGINE_OUTPUT(button5,SoSFBool,setValue(buttonIn5.getValue()));
    SO_ENGINE_OUTPUT(button6,SoSFBool,setValue(buttonIn6.getValue()));
    SO_ENGINE_OUTPUT(button7,SoSFBool,setValue(buttonIn7.getValue()));
}

END_NAMESPACE_STB