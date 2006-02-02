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
/** The header file for the KernelLoader_Base class.
*
* @author Denis Kalkofen
*
* $Id: KernelLoader_Base.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */
#ifndef _KERNELLOADERBASE_H_
#define _KERNELLOADERBASE_H_

namespace stb{
/**
*	The KernelLoader Interface
*/
class KernelLoader_Base
{
public:
	/**
	*	The destructor.
	*/
	~KernelLoader_Base();

	virtual void setLibName(const char* aLibName)=0;

	virtual void setExecFuncName(const char* aFuncName)=0;

	virtual void runKernel(int argc, char* argv[])=0;

protected:	
	/**
	*	Constructor()
	*/
	KernelLoader_Base();

	char libName[1024];

	char execFuncName[1024];
};// class

} //namespace STB

#endif//_KERNELLOADERBASE_H_
