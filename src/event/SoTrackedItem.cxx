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
/** The header file for the SoTrackedItem class.
*
* @author Denis Kalkofen
*
* $Id: SoTrackedItem.cpp 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "SoTrackedItem.h"
#include "StbSink.h"
#include "OpenTrackerContext.h"

SO_NODE_SOURCE(SoTrackedItem);

SoTrackedItem* 
SoTrackedItem::getNewInstance()
{
	return (new SoTrackedItem());
}

void
SoTrackedItem::initClass()
{
    SO_NODE_INIT_CLASS(SoTrackedItem, SoTrackedItemInterface, "SoTrackedItemInterface");
}


SoTrackedItem::SoTrackedItem(void)
{
	SO_NODE_CONSTRUCTOR(SoTrackedItem);
	
	SO_NODE_ADD_FIELD(stbSinkName, (""));

	SO_NODE_ADD_FIELD(translation, (0,0,0));
	SO_NODE_ADD_FIELD(rotation, (0,0,1,0));
	SO_NODE_ADD_FIELD(button0, (FALSE));
	SO_NODE_ADD_FIELD(button1, (FALSE));
	SO_NODE_ADD_FIELD(button2, (FALSE));
	SO_NODE_ADD_FIELD(button3, (FALSE));
	SO_NODE_ADD_FIELD(button4, (FALSE));
	SO_NODE_ADD_FIELD(button5, (FALSE));
	SO_NODE_ADD_FIELD(button6, (FALSE));
	SO_NODE_ADD_FIELD(button7, (FALSE));

	stbSink=NULL;
	configSensor.setFunction(SoTrackedItem::stbSinkChanged);
    configSensor.setData( this );
    configSensor.attach( &stbSinkName);

}



SoTrackedItem::~SoTrackedItem(void)
{
	//nil
}


void 
SoTrackedItem::stbSinkChanged( void * data, SoSensor * sensor)
{
    SoTrackedItem * self = (SoTrackedItem *)data;
	self->connectStbSink();
}

bool 
SoTrackedItem::connectStbSink()
{
	stbSink=OpenTrackerContext::getInstance()->getStbSink(stbSinkName.getValue().getString());
	if(!stbSink)
		return false;

	return true;
}

void 
SoTrackedItem::GLRender(SoGLRenderAction *action)
{
	
	if(!stbSink && !connectStbSink())
	{
	    printf("STB_ERROR: SoTrackedItem:: can't connect to '%s' \n",stbSinkName.getValue().getString());
	    return ;
	}

	
	if(stbSink->getState(trState)){
		this->translation.setValue(trState.position);
		this->rotation.setValue(trState.orientation);
	}
}
