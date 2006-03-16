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
/** The source file for the StbLogger class
*
* @author Erick Mendez
*
* @file                                                                   */
/* ======================================================================= */

#include <stb/kernel/StbLogger.h>
#include <stdarg.h>

BEGIN_NAMESPACE_STB

void logPrint(stb::string message)
{
	StbLogger::getInstance()->printMessage(message.c_str());
}

void logPrint(const char *message, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, message);
	vsprintf(tmpString, message, marker);

	StbLogger::getInstance()->printMessage(tmpString);
}

void logPrintD(stb::string debugMessage)
{
	StbLogger::getInstance()->printDebug(debugMessage.c_str());
}

void logPrintD(const char *debugMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, debugMessage);
	vsprintf(tmpString, debugMessage, marker);

	StbLogger::getInstance()->printDebug(tmpString);
}

void logPrintS(stb::string setupMessage)
{
	StbLogger::getInstance()->printSetup(setupMessage.c_str());
}

void logPrintS(const char *setupMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, setupMessage);
	vsprintf(tmpString, setupMessage, marker);

	StbLogger::getInstance()->printSetup(tmpString);
}

void logPrintI(stb::string infoMessage)
{
	StbLogger::getInstance()->printInfo(infoMessage.c_str());
}

void logPrintI(const char *infoMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, infoMessage);
	vsprintf(tmpString, infoMessage, marker);

	StbLogger::getInstance()->printInfo(tmpString);
}

void logPrintW(stb::string warningMessage)
{
	StbLogger::getInstance()->printWarning(warningMessage.c_str());
}

void logPrintW(const char *warningMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, warningMessage);
	vsprintf(tmpString, warningMessage, marker);

	StbLogger::getInstance()->printWarning(tmpString);
}

void logPrintE(stb::string errorMessage)
{
	StbLogger::getInstance()->printErrorAndContinue(errorMessage.c_str());
}

void logPrintE(const char *errorMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, errorMessage);
	vsprintf(tmpString, errorMessage, marker);

	StbLogger::getInstance()->printErrorAndContinue(tmpString);
}

void logPrintEAbort(stb::string errorMessage)
{
	StbLogger::getInstance()->printErrorAndAbort(errorMessage.c_str());
}

void logPrintEAbort(const char *errorMessage, ...)
{
	char tmpString[1024]; 
	va_list marker;

	va_start(marker, errorMessage);
	vsprintf(tmpString, errorMessage, marker);

	StbLogger::getInstance()->printErrorAndAbort(tmpString);
}

StbLogger*	StbLogger::instance=NULL;

StbLogger::StbLogger()
{
}

StbLogger::~StbLogger()
{
}

StbLogger *StbLogger::getInstance()
{
	if(instance == NULL)
		instance = new StbLogger();

	return instance;
}

void StbLogger::printMessage(const char *message)
{
	// We should be using ACE here

	printf("%s",message);

}

void StbLogger::printDebug(const char *debugMessage)
{
	// We should be using ACE here
//#ifdef _DEBUG
	printf("    >>>>>>>>>> %s",debugMessage);
//#endif //_DEBUG

}

void StbLogger::printSetup(const char *setupMessage)
{
	// We should be using ACE here
	printf("SETUP: %s",setupMessage);
}

void StbLogger::printInfo(const char *infoMessage)
{
	// We should be using ACE here
	printf("INFO : %s",infoMessage);
}

void StbLogger::printWarning(const char *warningMessage)
{
	// We should be using ACE here
	printf("WARN : %s",warningMessage);
}

void StbLogger::printErrorAndContinue(const char *errorMessage)
{
	// We should be using ACE here
	printf("ERROR: %s",errorMessage);
}

void StbLogger::printErrorAndAbort(const char *errorMessage)
{
	// We should be using ACE here
	printErrorAndContinue(errorMessage);

	// FIXME: Something more elegant here please. Mendez 20060316
	exit(0);
}

END_NAMESPACE_STB