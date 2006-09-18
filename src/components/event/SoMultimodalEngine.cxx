/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Source file for SoMultimodalEngine engine
  *
  * @author Eduardo Veas
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/engines/SoSubEngine.h>
#include <stb/components/event/EventContextFilter.h>
#include <stb/components/event/SoMultimodalEngine.h>


SO_ENGINE_SOURCE(SoMultimodalEngine);

void SoMultimodalEngine::initClass(){
   SO_ENGINE_INIT_CLASS(SoMultimodalEngine, SoEngine, "Engine");
};


//Constructor
SoMultimodalEngine::SoMultimodalEngine(): adapter(NULL)
{
  //	printf("SOMULTIMODALENGINE::CONSTRUCTOR\n");
  // Do Standard Constructor Stuff
  SO_ENGINE_CONSTRUCTOR(SoMultimodalEngine);

  SO_ENGINE_ADD_INPUT(key,(""));
  SO_ENGINE_ADD_INPUT(value,(""));
  SO_ENGINE_ADD_INPUT(buttonHisteresis,(FALSE));

  // define input fields
  SO_ENGINE_ADD_INPUT( attrName, (""));
  SO_ENGINE_ADD_INPUT( attrType, (""));
  SO_ENGINE_ADD_INPUT( vec3fIn, (0,0,0));
  SO_ENGINE_ADD_INPUT( rotationIn, (0, 0, 0, 1));
  //basic Types
  SO_ENGINE_ADD_INPUT( boolIn, (TRUE));
  SO_ENGINE_ADD_INPUT( floatIn, (0.0));
  SO_ENGINE_ADD_INPUT( intIn, (0));
  SO_ENGINE_ADD_INPUT( shortIn, (0));

  SO_ENGINE_ADD_INPUT( uintIn, (0));
  SO_ENGINE_ADD_INPUT( ushortIn, (0));
  SO_ENGINE_ADD_INPUT( stringIn, (""));

  /* just in case 
  SO_ENGINE_ADD_INPUT( imageIn, (0));
  SO_ENGINE_ADD_INPUT(  image3In, (0));
  SbMatrix identity;
  identity.makeIdentity();
  SO_ENGINE_ADD_INPUT( matrixIn, (identity)); */


  // define output fields specifying types
  SO_ENGINE_ADD_OUTPUT( vec3fValue,   SoSFVec3f);
  SO_ENGINE_ADD_OUTPUT( rotationValue,   SoSFRotation);
  // basic Types
  SO_ENGINE_ADD_OUTPUT( boolValue, SoSFBool );
  SO_ENGINE_ADD_OUTPUT( floatValue,   SoSFFloat);
  SO_ENGINE_ADD_OUTPUT( intValue,   SoSFInt32);
  SO_ENGINE_ADD_OUTPUT( shortValue,   SoSFShort);

  SO_ENGINE_ADD_OUTPUT( uintValue,   SoSFUInt32);
  SO_ENGINE_ADD_OUTPUT( ushortValue,   SoSFUShort);
  SO_ENGINE_ADD_OUTPUT( stringValue,   SoSFString);

  /* just in case 
  SO_ENGINE_ADD_OUTPUT( imageValue,   SoSFImage);
  SO_ENGINE_ADD_OUTPUT( image3Value,   SoSFImage3);
  SO_ENGINE_ADD_OUTPUT( matrixValue,   SoSFMatrix); */

  buttonHistory = FALSE;
  buttonChange  = TRUE;
  
};

// Destructor does nothing
SoMultimodalEngine::~SoMultimodalEngine(){
    if (adapter!=NULL) delete adapter;	
};


void SoMultimodalEngine::evaluate() {
	//printf("SOMULTIMODALENGINE::EVALUATE evaluating station%d, attr %s\n", station.getValue(), ((attrName.getValue()).getString()));

    SO_ENGINE_OUTPUT(vec3fValue, SoSFVec3f, setValue(
        vec3fIn.getValue()));
    SO_ENGINE_OUTPUT(stringValue, SoSFString, setValue(
        stringIn.getValue()));
    SO_ENGINE_OUTPUT(rotationValue, SoSFRotation, setValue(
        rotationIn.getValue()));
    SO_ENGINE_OUTPUT(boolValue, SoSFBool, setValue(
        boolIn.getValue()));
    SO_ENGINE_OUTPUT(floatValue, SoSFFloat, setValue(
        floatIn.getValue()));
    SO_ENGINE_OUTPUT(intValue, SoSFInt32, setValue(
        intIn.getValue()));
    SO_ENGINE_OUTPUT(shortValue, SoSFShort, setValue(
        shortIn.getValue()));
    SO_ENGINE_OUTPUT(uintValue, SoSFUInt32, setValue(
        uintIn.getValue()));

    SO_ENGINE_OUTPUT(ushortValue, SoSFUShort, setValue(
        ushortIn.getValue()));

};
void SoMultimodalEngine::inputChanged(SoField * whichField){
  //printf("SOMULTIMODALENGINE::INPUTCHANGED \n");
    if (whichField==&key || whichField==&value){
        delete adapter;
        adapter=new EventAdapter<SoMultimodalEngine>(this);
    }
};

void SoMultimodalEngine::processEvent(SoInputEvent *event)
{
    if (event){
      SbString key = attrName.getValue();
      SbString eventstr = key.getSubString(0, 5);
      if (eventstr != "event."){
	key = "event.";
	key += attrName.getValue();
      }
      //      printf("SOMULTIMODALENGINE::PROCESSEVENT: multimodal %s\n", key.getString());	
      
      if (event->containsKey(key.getString())){
	// evaluate the type of the event in order to decide in which output to 
	// place it

	if(attrType.getValue()=="string"){

	  if (event->isOfType(key, typeid(SbString))){

	    stringIn.setValue(event->getSFString(key));
	    evaluate();
	  }

	} else if (attrType.getValue() == "int"){
	  if (event->isOfType(key, typeid(int32_t))){
	    intIn.setValue(event->getSFInt32(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "short"){
	  if (event->isOfType(key, typeid(short))){
	    shortIn.setValue(event->getSFShort(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "uint"){
	  if (event->isOfType(key, typeid(uint32_t))){
	    uintIn.setValue(event->getSFUint32(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "ushort"){
	  if (event->isOfType(key, typeid(unsigned short))){
	    ushortIn.setValue(event->getSFUshort(key));
	    evaluate();
	  }
	} else if (attrType.getValue() =="ulong"){
	  if (event->isOfType(key, typeid(uint32_t))){
	    uintIn.setValue(event->getSFUint32(key));
	    evaluate();
	  }
	} else if ((attrType.getValue() =="float") || (attrType.getValue() =="double")){
	  if ((event->isOfType(key, typeid(float))) || (event->isOfType(key, typeid(double)))){

	    floatIn.setValue(event->getSFFloat(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "bool"){
	  if (event->isOfType(key, typeid(SbBool))){
	    boolIn.setValue(event->getSFBool(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "vec3f"){
	  if (event->isOfType(key, typeid(SbVec3f))){
	    vec3fIn.setValue(event->getSFVec3f(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "rotation") {
	  if (event->isOfType(key, typeid(SbRotation))){
	    rotationIn.setValue(event->getSFRotation(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "time"){
	  if (event->isOfType(key, typeid(SbTime))){
	    timeIn.setValue(event->getSFTime(key));
	    evaluate();
	  }
	} else if (attrType.getValue() == "matrix"){
	  
	} else if (attrType.getValue() == "image"){
	  
	}
      }
      //      evaluate(); 
    }
    
    
     
	  
}




