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
/** The header file for the OpenTrackerContext class.
*
* @author Denis Kalkofen
*
* $Id: OpenTrackerContext.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _OPENTRACKERCONTEXT_H_
#define _OPENTRCAKERCONTEXT_H_

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
#endif

#include <string>

#include "StbKernel/interfaces/OpenTrackerContextInterface.h"

#include "StbEvent.h"



class StbSink;
class StbModule;
class ACE_Mutex;

class STBEVENT_API OpenTrackerContext :
	public OpenTrackerContextInterface
{
  public:
	~OpenTrackerContext();

	static OpenTrackerContext* getInstance();

	virtual bool startOpenTracker(const char* otConfigName);

	virtual bool stopOpenTracker();

	StbSink* getStbSink(std::string stbtSinkName);

protected:
	OpenTrackerContext();

	static OpenTrackerContext* instance;

	std::string otConfigFileName;

	StbModule* eventHandler;

	static bool stopOTracker;

	static ACE_Mutex* updateLock;

	static void* otMainLoop(void*);

	
};


#endif 

