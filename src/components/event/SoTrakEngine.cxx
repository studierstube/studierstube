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
/** header file for SoTrakEngine engine
  *
  * @author Gerhard Reitmayr, Istvan Barakonyi
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/engines/SoSubEngine.h>

#include "SoTrakEngine.h"
#include "EventContextFilter.h"

SO_ENGINE_SOURCE(SoTrakEngine);

void SoTrakEngine::initClass(void)
{    
    if( SoType::fromName("SoTrakEngine").isBad() )
    {
        SO_ENGINE_INIT_CLASS(SoTrakEngine,SoEngine,"Engine");
    }
}

SoTrakEngine::SoTrakEngine(void) : adapter(NULL)
{
    SO_ENGINE_CONSTRUCTOR(SoTrakEngine);

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

SoTrakEngine::~SoTrakEngine()
{
    if (adapter!=NULL) delete adapter;
}


void SoTrakEngine::inputChanged(SoField* whichField)
{
    if (whichField==&key || whichField==&value){
        delete adapter;
        adapter=new EventAdapter<SoTrakEngine>(this);
    }
}

void SoTrakEngine::processEvent(SoInputEvent *event)
{
    if (event){
        SbVec3f pos=event->getSFVec3f("event.position");
        SbRotation rot=event->getSFRotation("event.orientation");

        translationIn.setValue(pos);
        rotationIn.setValue(rot);
        for (int i=0;i<8;i++){
            char fieldName[255],buttonEventName[255];
            sprintf(fieldName,"buttonIn%d",i);
            SoSFBool *field=(SoSFBool*)(this->getField(fieldName));
            if (field){
                sprintf(buttonEventName,"event.button.%d",i);
                field->setValue(event->getSFBool(buttonEventName));
            }
        }

        evaluate();
    }
}

void SoTrakEngine::evaluate() 
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
