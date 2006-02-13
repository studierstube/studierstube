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
/** The header file for the Scheduler class.
*
* @author Denis Kalkofen
*
* $Id: Scheduler.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "Scheduler.h"
#include <tinyxml.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include "Kernel.h"
BEGIN_NAMESPACE_STB


Scheduler::Scheduler()
{
	mode=IDLE;
	updateRate=0.0;
	scheduled=false;

}

Scheduler::~Scheduler()
{
	unschedule();
}

void 
Scheduler::unschedule()
{
	if(!scheduled)
		return ;

	switch(mode)
	{
	case IDLE:
		timer->unschedule();
		delete timer;
		break;
	case TIMER:

		break;
	}
	scheduled=false;
}

void 
Scheduler::schedule()
{
	if(scheduled)
		return ;

	switch(mode)
	{
	case IDLE:
		scheduleIdleSensor();
		break;
	case TIMER:
		scheduleTimerSensor();
		break;
	}
	scheduled=true;
}

void 
Scheduler::parseConfiguration(TiXmlAttribute* attribute)
{
	if(!stricmp(attribute->Name(),"updateMode"))
	{
		if(!stricmp(attribute->Value(),"idle"))
			mode=IDLE;		
		else if(!stricmp(attribute->Value(),"timer"))
			mode=TIMER;		
	}
	else if(!stricmp(attribute->Name(),"updateRate"))
	{
		updateRate=(float)atof(attribute->Value());
	}
}

void
Scheduler::scheduleIdleSensor()
{
	//printf("scheduleIdleSensor()\n");
	//sensor= new SoIdleSensor();
	//sensor->setFunction(Kernel::update);
	//sensor->schedule();
}

void
Scheduler::scheduleTimerSensor()
{
	Kernel::getInstance()->logDebug("Info: kernel->schedule SoTimerSensor \n");
	timer=new SoTimerSensor();
	timer->setFunction(Kernel::update);
	timer->setInterval(updateRate);
	sensor = timer;
	sensor->schedule();
}
END_NAMESPACE_STB
//========================================================================
// End of Scheduler.cxx
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
