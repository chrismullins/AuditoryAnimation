#pragma once
#include <GL/glew.h>
#include "TriangleMesh.h"
#include "Animator.h"

// This structure is responsible for setting up the viewport, perspective and projection transforms
struct ImagePlane
{
	float left; float right; float bottom;
	float top; float near; float far;
	unsigned int x_pixels; unsigned int y_pixels;
};

// This class contains all of the objects and animators for a particular scene. Calls to draw the scene will
// pass an integer representing the frame number. Each Animator object will be responsible for updating it's TriangleMesh
class SceneGraph
{
private:
	std::vector<TriangleMesh> scene_objects;
	std::vector<Animator> scene_animators;
	void setTransformPipeline();
	ImagePlane frustum;

	// ModulationSources should be able to see all of the current objects
	friend class ModulationSource;
public:
	// Adds an animator to the 
	void addAnimation(Animator n);
	TriangleMesh* getIthObj(int index);
	void setFrustum(ImagePlane new_frustum);
	void update();
	// Uses the code from PA3 to parse a .obj file into a triangle mesh, and adds it to the scene
	void loadMeshFromString(std::string s);
	// This function is included for now, but will likely be taken over 
	void setLighting();
	// Draws the scene without applying any modulation
	void draw();
	// This function draws all of the objects in the scene at the specified instant. This allows 
	// animations to be visualized.
	void drawAtTimeIndex(int time_step);
	SceneGraph(void);
	~SceneGraph(void);
};

