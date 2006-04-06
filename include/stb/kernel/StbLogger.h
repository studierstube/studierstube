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
/** The header file for the StbLogger class
*
* @author Erick Mendez
*
* @file                                                                   */
/* ======================================================================= */


#ifndef _STBLOGGER_H_
#define _STBLOGGER_H_

#include <stb/kernel/Studierstube.h>
#include <stb/base/macros.h>
#include <stb/base/string.h>

BEGIN_NAMESPACE_STB

#include "MessagesDefines.h"

/*
* Usage:  
*		- logPrintE("File Not Found");
*		- logPrintE(LOG_ERROR_FILE_NOT_FOUND);
*		- logPrintE("File %s Not Found", filename);
*
*/

void logPrint(stb::string message);
void logPrint(const char *message, ...);
void logPrintD(stb::string debugMessage);
void logPrintD(const char *debugMessage, ...);
void logPrintS(stb::string setupMessage);
void logPrintS(const char *setupMessage, ...);
void logPrintI(stb::string infoMessage);
void logPrintI(const char *infoMessage, ...);
void logPrintW(stb::string warningMessage);
void logPrintW(const char *warningMessage, ...);
void logPrintE(stb::string errorMessage);
void logPrintE(const char *errorMessage, ...);
void logPrintEAbort(stb::string errorMessage);
void logPrintEAbort(const char *errorMessage, ...);

class STB_API StbLogger
{
public:

	static StbLogger *getInstance();

	~StbLogger();

	void printMessage(const char *message);
	void printDebug(const char *debugMessage);
	void printSetup(const char *setupMessage);
	void printInfo(const char *infoMessage);
	void printWarning(const char *warningMessage);
	void printErrorAndContinue(const char *errorMessage);
	void printErrorAndAbort(const char *errorMessage);

protected:

	static StbLogger *instance;

	StbLogger();


};

END_NAMESPACE_STB
#endif //_STBLOGGER_H_
