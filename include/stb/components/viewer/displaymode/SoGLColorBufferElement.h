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
/** The header file for the SoGLColorBufferElement class.
*
* @author Denis Kalkofen
*
* $Id: SoGLColorBufferElement.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOGLCOLORBUFFERELEMENT_H_
#define _SOGLCOLORBUFFERELEMENT_H_

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbLinear.h> 



class  SoGLColorBufferElement : public SoElement 
{   
    SO_ELEMENT_HEADER(SoGLColorBufferElement);
public:
    static void initClass(void);
	
	/// sets the stencil mask to define to which planes to write
    static void setMask(SoState * state, SbVec4f newMask);

protected:
    virtual ~SoGLColorBufferElement();
    
public:
	virtual void init(SoState * state);

	virtual void push(SoState * state);

    virtual void pop(SoState * state, const SoElement * prevTopElement);
    
    virtual SbBool matches(const SoElement * element) const;

	virtual SoElement * copyMatchInfo(void) const;
     
    bool operator==( const SoGLColorBufferElement & arg ) const;
    bool operator!=( const SoGLColorBufferElement & arg ) const;

private:
	SbVec4f mask;

	void updategl( ) const;
};

inline bool SoGLColorBufferElement::operator==( const SoGLColorBufferElement & arg ) const
{
    if(this->mask   == arg.mask)
        return true;
    else
        return false;
}

inline bool SoGLColorBufferElement::operator!=( const SoGLColorBufferElement & arg ) const
{
    return !operator==(arg);
}


#endif // !_SOGLCOLORBUFFERELEMENT_H_
