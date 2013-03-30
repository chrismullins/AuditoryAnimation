#include "SceneGraph.h"

SceneGraph::SceneGraph(void)
{
	setTransformPipeline();
	setLighting();
}

SceneGraph::~SceneGraph(void)
{
}

void SceneGraph::setTransformPipeline()
{
	glEnable(GL_NORMALIZE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(frustum.left, frustum.right, frustum.bottom, frustum.top, frustum.near, frustum.far);
	glViewport(0, 0, frustum.x_pixels, frustum.y_pixels);
}

// Resets the current lighting in the scene. Ideally this should be done by Animator instances
void SceneGraph::setLighting()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float Ia[] = {0.2, 0.2, 0.2, 0};
	float location[] = {-0.1, -0.1, 5.0, 0};
	float l[] = {-1, -1, -1, 0};
	float la[] = {0.2, 0.2, 0.2, 0};
	float ld[] = {1, 1, 1, 0};
	float ls[] = {0, 0, 0, 0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ia);
	glLightfv(GL_LIGHT0, GL_POSITION, location);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l);
	glLightfv(GL_LIGHT0, GL_AMBIENT, la);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
}

void SceneGraph::draw()
{
	//Simply draws each mesh inside the current scene
	setLighting();
	for (int i = 0; i < scene_objects.size(); i++)
		scene_objects[i].draw();
}

// Delegates creating the mesh to a TriangleMesh object and adds it to the scene
void SceneGraph::loadMeshFromString(std::string filename)
{
	TriangleMesh to_add = TriangleMesh(filename);
	Vector3 scale = {3, 8, 8};
	Vector3 loc = {0.1, -1, -1.5};
	Material color = Material(RGBColor(0.5, 0.5, 0.5), RGBColor(0.5, 0.5, 0), RGBColor(), RGBColor());
	to_add.scale_factor = scale;
	to_add.location = loc;
	to_add.mat = color;
	scene_objects.push_back(to_add);
}

TriangleMesh* SceneGraph::getIthObj(int index)
{
	return &scene_objects[index];
}

void SceneGraph::setFrustum(ImagePlane new_frustum)
{
	frustum = new_frustum;
	setTransformPipeline();
}

void SceneGraph::drawAtTimeIndex(int time_step)
{
	setLighting();

	// Each Animator instant is responsible for updating the
	// state of a particular TriangleMesh object based on the time_step.
	for (int i = 0; i < scene_animators.size(); i++)
		scene_animators[i].update(time_step);
	
	// Now, all objects have been altered based on the sound input,
	// simply draw each object.
	for (int i = 0; i < scene_objects.size(); i++)
	{
		scene_objects[i].update();
		scene_objects[i].draw();
	}
}

void SceneGraph::addAnimation(Animator to_add)
{
	scene_animators.push_back(to_add);
}