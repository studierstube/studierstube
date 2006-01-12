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
* $Id: SoTrackedItem.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef  SOTRACKEDITEM_H
#define  SOTRACKEDITEM_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h> 
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFRotation.h>
#include <OpenTracker.h>

#include "StbEvent.h"
#include "StbSink.h"
#include "StbKernel/interfaces/SoTrackedItemInterface.h"

class STBEVENT_API SoTrackedItem 
	:   public SoTrackedItemInterface
{
	SO_NODE_HEADER(SoTrackedItem);

public:
	SoTrackedItem(void);

	~SoTrackedItem(void);

	static void initClass();

	static SoTrackedItem* getNewInstance();

protected:


	virtual void GLRender(SoGLRenderAction *action);

	StbSink *stbSink;
	/// sensor for configuration field
    SoFieldSensor configSensor;

    /// callback for configuration field sensor
    static void stbSinkChanged( void * data, SoSensor * sensor);

	bool connectStbSink();

	ot::State trState;
};


#endif 

