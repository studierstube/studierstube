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
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */

#include <stb/components/starlight/SoSpaceMouseKit.h>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/SceneManager.h>
#include <stb/components/event/event.h>

SO_KIT_SOURCE(SoSpaceMouseKit);

void SoSpaceMouseKit::initClass(void){
	SO_KIT_INIT_CLASS(SoSpaceMouseKit, SoBaseKit, "BaseKit");
}
SoSpaceMouseKit::SoSpaceMouseKit(void){
	SO_KIT_CONSTRUCTOR(SoSpaceMouseKit);

// Set up Fields
	SO_KIT_ADD_FIELD(shape, (""));
	SO_KIT_ADD_FIELD(resetPosition, ());
	SO_KIT_ADD_FIELD(resetRotation, ());
//	SO_KIT_ADD_FIELD(storePosition, ());
//	SO_KIT_ADD_FIELD(recallPosition, ());
	SO_KIT_ADD_FIELD(measureAdvance, ());
	SO_KIT_ADD_FIELD(measureBack, ());

	SO_KIT_ADD_FIELD(toggleRotationAxis, ());
//	SO_KIT_ADD_FIELD(showDistance, (true));

//	SO_KIT_ADD_FIELD(pickedObject, (""));

//	SO_KIT_ADD_FIELD(translationField, (0.0f, 0.0f, 0.0f));
//	SO_KIT_ADD_FIELD(rotationField, (0.0f, 0.0f, 0.0f, 1.0f));
	SO_KIT_ADD_FIELD(otcButton, (0));
//	SO_KIT_ADD_FIELD(confidenceField, (1.0f));

	SO_KIT_ADD_FIELD(cursorKey,(""));
	SO_KIT_ADD_FIELD(cursorValue,(""));

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, this, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(geometry, SoNode, SoCube, TRUE, topSeparator, \x0, TRUE);

	SO_KIT_INIT_INSTANCE();

	// instantiate Sensors
	this->shapeSensor = new SoFieldSensor(SoSpaceMouseKit::shapeFM, this);
	this->shapeSensor->setPriority(0); // immediate sensor
	this->resetPosSensor = new SoFieldSensor(SoSpaceMouseKit::resetPosFM, this);
	this->resetPosSensor->setPriority(0); // immediate sensor
	this->resetRotSensor = new SoFieldSensor(SoSpaceMouseKit::resetRotFM, this);
	this->resetRotSensor->setPriority(0); // immediate sensor
	//this->storePosSensor = new SoFieldSensor(SoSpaceMouseKit::storePosFM, this);
	//this->storePosSensor->setPriority(0); // immediate sensor
	//this->recallPosSensor = new SoFieldSensor(SoSpaceMouseKit::recallPosFM, this);
	//this->recallPosSensor->setPriority(0); // immediate sensor
	//this->measureAdvSensor = new SoFieldSensor(SoSpaceMouseKit::measureAdvFM, this);
	//this->measureAdvSensor->setPriority(0); // immediate sensor
	//this->measureBackSensor = new SoFieldSensor(SoSpaceMouseKit::measureBackFM, this);
	//this->measureBackSensor->setPriority(0); // immediate sensor
	this->rotACASensor = new SoFieldSensor(SoSpaceMouseKit::rotACAFM, this);
	//this->rotACASensor->setPriority(0); // immediate sensor

	measureState = 0;
	//cursorEvents=NULL;

	setUpConnections(TRUE, TRUE);
}

SoSpaceMouseKit::~SoSpaceMouseKit(void) {
	delete this->shapeSensor;
	delete this->resetPosSensor;
	delete this->resetRotSensor;
	//delete this->storePosSensor;
	//delete this->recallPosSensor;
	//delete this->measureAdvSensor;
	//delete this->measureBackSensor;
	delete this->rotACASensor;
}

void SoSpaceMouseKit::handleEvent( SoHandleEventAction * action ) {
	const SoEvent *event = action->getEvent();
}

// SetupConnections
SbBool SoSpaceMouseKit::setUpConnections(SbBool onOff, SbBool doItAlways = 0) {

	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onOff, doItAlways);

		// Attach Field Sensors
		shapeFM(this, NULL);
		if (this->shapeSensor->getAttachedField() != &this->shape)
			this->shapeSensor->attach(& this->shape);
		resetPosFM(this, NULL);
		if (this->resetPosSensor->getAttachedField() != &this->resetPosition)
			this->resetPosSensor->attach(& this->resetPosition);
		resetRotFM(this, NULL);
		if (this->resetRotSensor->getAttachedField() != &this->resetRotation)
			this->resetRotSensor->attach(& this->resetRotation);
		//storePosFM(this, NULL);
		//if (this->storePosSensor->getAttachedField() != &this->storePosition)
		//this->storePosSensor->attach(& this->storePosition);
		//recallPosFM(this, NULL);
		//if (this->recallPosSensor->getAttachedField() != &this->recallPosition)
		//	this->recallPosSensor->attach(& this->recallPosition);
		//measureAdvFM(this, NULL);
		//if (this->measureAdvSensor->getAttachedField() != &this->measureAdvance)
		//this->measureAdvSensor->attach(& this->measureAdvance);
		//measureBackFM(this, NULL);
		//if (this->measureBackSensor->getAttachedField() != &this->measureBack)
		//	this->measureBackSensor->attach(& this->measureBack);
		rotACAFM(this, NULL);
		if (this->rotACASensor->getAttachedField() != &this->toggleRotationAxis)
			this->rotACASensor->attach(& this->toggleRotationAxis);

		// 
		//Event* event=(Event*)(Kernel::getInstance()->getComponentManager()->load("Event"));
		//cursorEvents = event->createSoTrakEngine();
		//cursorEvents->key.connectFrom(&cursorKey);
		//cursorEvents->value.connectFrom(&cursorValue);
		/////////////////////////////////////////////////////////// to do - not implemented yet ...
		//cursorEvents->getAttribute("float", "scalingFactor");
		//cursorEvents->getAttribute("float", "cursorDistance");
		//mover->translation.connectFrom(&tracker->translation);
		//mover->rotation.connectFrom(&tracker->rotation);

	} else {
		// Detach Field Sensors
		if (this->shapeSensor->getAttachedField() != NULL)
			this->shapeSensor->detach();
		if (this->resetPosSensor->getAttachedField() != NULL)
			this->resetPosSensor->detach();
		if (this->resetRotSensor->getAttachedField() != NULL)
			this->resetRotSensor->detach();
		//if (this->storePosSensor->getAttachedField() != NULL)
		//	this->storePosSensor->detach();
		//if (this->recallPosSensor->getAttachedField() != NULL)
		//	this->recallPosSensor->detach();

		//if (this->measureAdvSensor->getAttachedField() != NULL)
		//	this->measureAdvSensor->detach();
		//if (this->measureBackSensor->getAttachedField() != NULL)
		//	this->measureBackSensor->detach();

		if (this->rotACASensor->getAttachedField() != NULL)
			this->rotACASensor->detach();

		// We disconnect BEFORE base class.
		SoBaseKit::setUpConnections(onOff, doItAlways);
	}
	return !(connectionsSetUp = onOff);
}

// Callback for the field sensor.
void SoSpaceMouseKit::shapeFM(void * userdata, SoSensor * sensor) 
{
	SoSpaceMouseKit *me = (SoSpaceMouseKit *)userdata;
	//if (sensor==NULL) return;
	me->loadGeometry();
}

void SoSpaceMouseKit::loadGeometry() 
{
    const char *geomfile;
	SoSeparator *geom;

    //stb::Kernel::getInstance()->log("[SoSpaceMouseKit]: update Cursor geometry: ");  
    //stb::Kernel::getInstance()->log(shape.getValue().getString());
   	
	geomfile = shape.getValue().getString();
	if (*geomfile == 0) {	// don't try to load an empty file
		return;
	}
    // read geometry into the geometry part
	SoInput::addDirectoryFirst("./");
    SoInput myinput;
	if (!myinput.openFile(geomfile)) 
	{
		//stb::Kernel::getInstance()->logDebug("[SoSpaceMouseKit]: can not open file:");
        SoInput::removeDirectory("./");
        return;
    }
	geom = SoDB::readAll(&myinput);
    myinput.closeFile();
    SoInput::removeDirectory("./");
    if (geom == NULL) 
    {
        //stb::Kernel::getInstance()->log("[SoSpaceMouseKit]: problem reading file: ");
		//stb::Kernel::getInstance()->log(geomfile);
	    //stb::Kernel::getInstance()->log("\n");
        return;
    }
	setPart("geometry", geom);
}


// Callback for the field sensor.
void SoSpaceMouseKit::resetPosFM(void * userdata, SoSensor * sensor) 
{
	SoSpaceMouseKit *me = (SoSpaceMouseKit *)userdata;
	if (sensor == NULL)
		return;
	me->otcButton = OTCOM_RESETPOSITION;
}
// Callback for the field sensor.
void SoSpaceMouseKit::resetRotFM(void * userdata, SoSensor * sensor) 
{
	SoSpaceMouseKit *me = (SoSpaceMouseKit *)userdata;
	if (sensor == NULL)
		return;
	me->otcButton = OTCOM_RESETROTATION;
}

/*
// TODO: implement store and recall functions

// Callback for the field sensor.
void SoSpaceMouseKit::storePosFM(void * userdata, SoSensor * sensor) {
	SoSpaceMouseKit *me = (SoSpaceMouseKit *)userdata;
	if (sensor == NULL)
		return;
}
// Callback for the field sensor.
void SoSpaceMouseKit::recallPosFM(void * userdata, SoSensor * sensor) {
	SoSpaceMouseKit *me = (SoSpaceMouseKit *)userdata;
	if (sensor == NULL)
		return;
}

*/

/*
  Distance measuring:
  STATES:
  0 - none
  1 - located first point (draw line from this point to mouse cursor)
  2 - located second point (line/distance still present - next state is 0)
   
*/

// Callback for the field sensor.
void SoSpaceMouseKit::measureAdvFM(void * userdata, SoSensor * sensor) 
{
	SoSpaceMouseKit *me = (SoSpaceMouseKit *)userdata;
	printf ("#### MEASURE ADVANCE ####\n");
	if (sensor == NULL)
		return;

	SbVec3f *pos = me->calculateWorldPosition();
	printf ("GOT POSITION: %.02f %.02f %.02f", pos[0], pos[1], pos[2]);

	me->measureState = (me->measureState+1)%3;

	//measureStateMachine();
}

// Callback for the field sensor.

void SoSpaceMouseKit::measureBackFM(void * userdata, SoSensor * sensor) 
{
	SoSpaceMouseKit *me = (SoSpaceMouseKit *)userdata;
	if (sensor == NULL)
		return;

 	//me->measureState = (me->measureState-1)%3;   

	//measureStateMachine();
}

/*
void measureStateMachine() {
	switch (measureState) {
		case 0:
		break;

		case 1:
		break;

		case 2:
		break;
	}
}
*/

// 
// This function calculates the absolute World Position of the SpaceMouseKit
// 
	
SbVec3f *SoSpaceMouseKit::calculateWorldPosition() 
{
	SbVec3f *translation = new SbVec3f();
	SoSearchAction * searchaction = new SoSearchAction;
	searchaction->setNode(this);
	searchaction->apply(stb::Kernel::getInstance()->getSceneManager()->getSceneRoot());

	SoPath * path = searchaction->getPath();
	assert(path != NULL);

	// TODO: Get Viewport Region from current Studierstube Viewer
	//       (couldn't figure out that yet)
	//const SbViewportRegion vpr = stb::Kernel::getInstance()->getSceneManager()->getDisplayRoot->getByName("SoViewPort");
	//SoGetMatrixAction * getmatrixaction = new SoGetMatrixAction(vpr);		// doesn't work (yet)

	//getmatrixaction->apply(path);
	//SbMatrix transformation = getmatrixaction->getMatrix();

	//SbRotation rotation;
	//SbVec3f scalevector;
	//SbRotation scaleorientation;

	//transformation.getTransform(*translation, rotation, scalevector, scaleorientation);

	return translation;
}


void SoSpaceMouseKit::rotACAFM(void *userdata, SoSensor * sensor) {
  SoSpaceMouseKit *me = (SoSpaceMouseKit *)userdata;
  if (sensor == NULL)
	  return;
  me->otcButton = OTCOM_TOGGLE_ROTATECAMERAAXIS;
}
