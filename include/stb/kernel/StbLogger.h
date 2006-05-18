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
void STB_API logPrint(stb::string message);
void STB_API logPrint(const char *message, ...);
void STB_API logPrintD(stb::string debugMessage);
void STB_API logPrintD(const char *debugMessage, ...);
void STB_API logPrintS(stb::string setupMessage);
void STB_API logPrintS(const char *setupMessage, ...);
void STB_API logPrintI(stb::string infoMessage);
void STB_API logPrintI(const char *infoMessage, ...);
void STB_API logPrintW(stb::string warningMessage);
void STB_API logPrintW(const char *warningMessage, ...);
void STB_API logPrintE(stb::string errorMessage);
void STB_API logPrintE(const char *errorMessage, ...);
void STB_API logPrintEAbort(stb::string errorMessage);
void STB_API logPrintEAbort(const char *errorMessage, ...);

/*
 * Enumerations
 */
enum LOG_MODE 
{
	OFF=0,
	FILE=1,
	CONSOLE=2
};

class STB_API StbLogger
{
public:

	/// The instance retriever, this is a singleton
	static StbLogger *getInstance();

	/// The destructor
	~StbLogger();

	/// This hold the logging method
	LOG_MODE logMode;

	/// The Filename to log to
	stb::string logFilename;

	void printMessage(const char *message);
	void printDebug(const char *debugMessage);
	void printSetup(const char *setupMessage);
	void printInfo(const char *infoMessage);
	void printWarning(const char *warningMessage);
	void printErrorAndContinue(const char *errorMessage);
	void printErrorAndAbort(const char *errorMessage);

protected:

	/// The instance
	static StbLogger *instance;

	/// The constructor
	StbLogger();
};

END_NAMESPACE_STB
#endif //_STBLOGGER_H_
