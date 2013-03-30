#include "Animator.h"

// The resting state is defined to be the state of the object at the moment
// that this instant of Animator is instantiated
Animator::Animator(TriangleMesh* obj, ModulationSource* source)
{
	object = obj;
	mod_source = source;

	rest_state.location = (*obj).location;
	rest_state.mat = (*obj).mat;
	rest_state.scale = (*obj).scale_factor;

	excited_state.location = Vector3();
	excited_state.scale = Vector3();
	excited_state.mat = Material();
}


Animator::~Animator(void)
{
}

void Animator::update(int time_step)
{
	float mod_level = (*mod_source).getModForTimeStep(time_step);

	// TODO: interpolate between rest_state and excited_state based
	// on the value of mod_level
	if (mod_level > 0)
	{
		(*object).location.x = rest_state.location.x * (1 - mod_level) + excited_state.location.x * mod_level;
		(*object).location.y = rest_state.location.y * (1 - mod_level) + excited_state.location.y * mod_level;
		(*object).location.z = rest_state.location.z * (1 - mod_level) + excited_state.location.z * mod_level;

		(*object).scale_factor.x = rest_state.scale.x * (1 - mod_level) + excited_state.scale.x * mod_level;
		(*object).scale_factor.y = rest_state.scale.y * (1 - mod_level) + excited_state.scale.y * mod_level;
		(*object).scale_factor.z = rest_state.scale.z * (1 - mod_level) + excited_state.scale.z * mod_level;

		(*object).mat.ambient = excited_state.mat.ambient * (1 - mod_level) + excited_state.mat.ambient * (mod_level);
		(*object).mat.diffuse = excited_state.mat.diffuse * (1 - mod_level) + excited_state.mat.diffuse * (mod_level);
		(*object).mat.specular = excited_state.mat.specular * (1 - mod_level) + excited_state.mat.specular * (mod_level);
		(*object).mat.reflective = excited_state.mat.reflective * (1 - mod_level) + excited_state.mat.reflective * (mod_level);
	}
	(*object).update();
}