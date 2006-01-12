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
/** The header file for the SoDisplay class.
*
* @author Denis Kalkofen
*
* $Id: SoDisplay.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SODISPLAYMODE_H_
#define _SODISPLAYMODE_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/actions/SoGLRenderAction.h> 

#include "StbViewer/StbViewer.h"
#include "StbViewer/SoStudierstubeViewer.h"

class STBVIEWER_API SoDisplayMode : public SoNode
{
SO_NODE_HEADER(SoDisplayMode);

protected:
    /** constructor. */
    SoDisplayMode(                    
        );
    /** Abstract destructor. */
    virtual ~SoDisplayMode(           
        ) {} 
	
	

public:
    /** Inventor class initialization */
    static void initClass();
   	
	virtual void setViewer(SoStudierstubeViewer* aViewer);
	
protected:
	virtual void GLRender(SoGLRenderAction *action);

	SoStudierstubeViewer* viewer;
};


//----------------------------------------------------------------------------
#endif//_SODISPLAYMODE_H_

