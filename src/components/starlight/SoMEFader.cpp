 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  include <windows.h>
#  include <math.h>
#endif

#include <stb/components/starlight/SoMEFader.h>


SO_ENGINE_SOURCE(SoMEFader);

void
SoMEFader::initClass()
{
    SO_ENGINE_INIT_CLASS(SoMEFader, SoEngine, "Engine");
}

SoMEFader::SoMEFader()
{
    SO_ENGINE_CONSTRUCTOR(SoMEFader);

    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, EASE );
    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, LOGARITHMIC );
    SO_ENGINE_DEFINE_ENUM_VALUE(Styles, PULSE );

    // **********   Field Additions
    SO_ENGINE_ADD_INPUT(signal, (FALSE));
    SO_ENGINE_ADD_INPUT(ease, (1));
    SO_ENGINE_ADD_INPUT(duration, (1));
    SO_ENGINE_ADD_INPUT(interpolate0, (0));
    SO_ENGINE_ADD_INPUT(interpolate1, (1));
    SO_ENGINE_ADD_INPUT(in, (0));
    SO_ENGINE_ADD_OUTPUT(out, SoSFFloat);
    SO_ENGINE_SET_SF_ENUM_TYPE(style, Styles);
    SO_ENGINE_ADD_INPUT(style, (EASE));


    // **********   Private Engine Initializations
    conditional=new SoConditionalTrigger;
    oneshot=new SoOneShot;
    easein=new SoEaseIn;
    interpolatefloat=new SoInterpolateFloat;

    conditional->boolIn.setValue(false);
    conditional->triggerBool=true;
    conditional->comparison=SoConditionalTrigger::EQUAL;
    oneshot->trigger.connectFrom(&conditional->trigger);
    oneshot->flags=(SoOneShot::RETRIGGERABLE | SoOneShot::HOLD_FINAL);
    easein->in.connectFrom(&oneshot->ramp);
    interpolatefloat->alpha.connectFrom(&easein->out);
    signalSensor=new SoFieldSensor(SoMEFader::refreshCB, this);
    signalSensor->attach(&this->signal);
    in.connectFrom(&interpolatefloat->output);

}

SoMEFader::~SoMEFader()
{
    delete [] signalSensor;
}


void SoMEFader::refreshCB(void *data, SoSensor * /*sensor*/)
{
    SoMEFader *self= (SoMEFader *)data;
    self->updateEngines();
}


void SoMEFader::updateEngines()
{
    conditional->boolIn.setValue(signal.getValue());
    oneshot->duration.setValue(duration.getValue());
    interpolatefloat->input0.setValue(interpolate0.getValue());
    interpolatefloat->input1.setValue(interpolate1.getValue());
    easein->ease.setValue(ease.getValue());
    easein->style.setValue(style.getValue());
}

void SoMEFader::evaluate()
{
    float val = in.getValue();
	SO_ENGINE_OUTPUT(out, SoSFFloat, setValue(val) );
}


