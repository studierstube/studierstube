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
/** The header file for the Config class.
*
* @author Denis Kalkofen
*
* $Id: Config.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */


#ifndef __CONFIG_H_
#define __CONFIG_H_

namespace STB{

class Config{
public:
	Config();

	~Config();

	enum LOG_MODE {
		OFF,
		FILE,
		CONSOLE,
		CONSOLE_AND_FILE
	};

	// parses the config file
	bool readConfigFile(const char* nFileName);

	/*
	*	returns the current log_mode
	*/
	LOG_MODE getLogMode();
protected:
	/*
	* sets one of the kernels parameter.
	* returns true if key exists. 
	*/
	bool setConfig(const char* key, const char* value);

	//parameter 
	LOG_MODE logMode;
};

}//namespace


#endif //