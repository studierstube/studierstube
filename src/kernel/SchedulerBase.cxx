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
/** The header file for the SchedulerBase class.
*
* @author Denis Kalkofen
*
* $Id: SchedulerBase.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */
#include "SchedulerBase.h"
#include <tinyxml.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include "Kernel.h"
BEGIN_NAMESPACE_STB


SchedulerBase::SchedulerBase()
{
	mode=IDLE;
	updateRate=0.0;
	scheduled=false;

}

SchedulerBase::~SchedulerBase()
{
	unschedule();
}

void 
SchedulerBase::unschedule()
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
SchedulerBase::schedule()
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
SchedulerBase::parseConfiguration(TiXmlAttribute* attribute)
{
	if(!stb::stricasecmp(attribute->Name(),"updateMode"))
	{
		if(!stb::stricasecmp(attribute->Value(),"idle"))
			mode=IDLE;		
		else if(!stb::stricasecmp(attribute->Value(),"timer"))
			mode=TIMER;		
	}
	else if(!stb::stricasecmp(attribute->Name(),"updateRate"))
	{
		updateRate=(float)atof(attribute->Value());
	}
}

void
SchedulerBase::scheduleIdleSensor()
{
	//printf("scheduleIdleSensor()\n");
	//sensor= new SoIdleSensor();
	//sensor->setFunction(Kernel::update);
	//sensor->schedule();
}

void
SchedulerBase::scheduleTimerSensor()
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
// End of SchedulerBase.cxx
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