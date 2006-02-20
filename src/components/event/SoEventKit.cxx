/* ========================================================================
 * Copyright (C) 2000-2005  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Source file for SoEventKit
  * 
  * @author Gerhard Reitmayr
  *
  * $Id: SoEventKit.cpp 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/components/event/SoEventKit.h>

SO_KIT_SOURCE(SoEventKit);

void SoEventKit::initClass(void)
{
    if( SoType::fromName("SoEventKit").isBad())
    {
        SO_KIT_INIT_CLASS(SoEventKit, SoBaseKit, "BaseKit");
    }
}

SoEventKit::SoEventKit(void)
{
    SO_KIT_CONSTRUCTOR(SoEventKit);

    SO_KIT_INIT_INSTANCE();
}
