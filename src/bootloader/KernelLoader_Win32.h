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
/** The header file for the KernelLoader_Win32 class.
*
* @author Denis Kalkofen
*
* $Id: KernelLoader_Win32.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _KERNELLOADER_WIN32_H_
#define _KERNELLOADER_WIN32_H_

#include "KernelLoader_Base.h"

namespace STB{	
/**
*	The KernelLoader Interface
*/
	class KernelLoader_Win32 : public STB::KernelLoader_Base
{
public:
	/**
	*	The destructor.
	*/
	~KernelLoader_Win32();

	/**
	*	Constructor()
	*/
	KernelLoader_Win32();

	/**
	*	set the name of the library which is going to be loaded.
	*/
	virtual void setLibName(const char* aLibName);

	/**
	*	set the name of the function which is going to be called.
	*/
	virtual void setExecFuncName(const char* aFuncName);


	/**
	*	loads the lib and calls the function defined by 'execFuncName'.
	*/
	virtual void runKernel(int argc, char* argv[]);

protected:	

};//class

} //namespace STB

#endif//_KERNELLOADER_H_
