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
/** The header file for the StbViewer class.
*
* @author Denis Kalkofen
*
* $Id: StbViewer.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _STBVIEWER_H_
#define _STBVIEWER_H_

#include "configSTB.h"

#define STUDIERSTUBE_VERSION_STRING "Studierstube v4.0"

#ifdef WIN32
	#ifdef STBVIEWER_EXPORTS
		#define STBVIEWER_API __declspec(dllexport)
	#else
		#define STBVIEWER_API __declspec(dllimport)
	#endif
#else
    #define STBVIEWER_API
#endif

#include <vector>
class SoDisplay;
class SoNode;

class STBVIEWER_API StbViewer
{
public:
	static void init();

	static void fin();

	static void addSoDisplay(SoDisplay* aSoDisplay);

	static void removeSoDisplay(SoDisplay* aSoDisplay);

private:
	StbViewer(){};
	
	~StbViewer(){};
	
	static StbViewer* instance;

	//List of all SoDisplays loaded
	// export explicit instantiation of vector template .. this will avoid warning C4251
#ifdef WIN32
	template class STBVIEWER_API std::allocator<SoDisplay*>;
	template class STBVIEWER_API std::vector<SoDisplay*, std::allocator<SoDisplay*> >;
#endif
	std::vector<SoDisplay*> soDisplays;
};

#endif//_STBVIEWER_H_
