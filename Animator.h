#pragma once
#include "TriangleMesh.h"
#include "ModulationSource.h"

// This struct defines the current status of an object. Other object states exist
// could be added.
struct ObjectState
{
	Material mat;
	Vector3 location;
	Vector3 scale;
};

// This class keeps a pointer to an object and a modulation source. This class keeps track of what the
// object's resting and excited states are and interpolates between the two. This is done according to
// the values read when ModulationSource::getModForTimeStep is called
class Animator
{
private:
	// Pointer to the scene object which is receiving modulation
	// from mod_source. mod_source calculates the intensity of modulation
	// to be applied to the object variable.
	TriangleMesh* object;
	ModulationSource* mod_source;

	ObjectState rest_state;

public:
	// This function resets all of the relevant values in the object, according to what is going on
	// in the audio data at the time step, time_step. 
	void update(int time_step);

	// This variable defines what the TriangleMesh will look like at
	// 100% excitation
	ObjectState excited_state;
	Animator(TriangleMesh* object, ModulationSource* mod_source);
	~Animator(void);
};

