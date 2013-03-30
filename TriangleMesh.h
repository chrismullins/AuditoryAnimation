#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include <fstream>
#include <float.h>
#include "Material.h"

struct Vector3
{
	float			x, y, z;
};

struct Triangle
{
	unsigned int 	indices[3];
};

class TriangleMesh
{
private:
	// For parsing input files into triangle meshes
	void tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter);
	int face_index(const char* string);
	void load_mesh(std::string file_name);
	// Resets the model transform and material properties of the mesh
	void setModelTransform();
	void setMaterial();
	// Triangle mesh data
	std::vector<Vector3>	gPositions;
	std::vector<Vector3>	gNormals;
	std::vector<Triangle> gTriangles;
	// Puts the above data into GPU buffers
	void init();
	
public:
	void setMaterial(Material m);
	void update();
	//These members are made public for speed concerns. If you want to change one of these, you MUST call update() after changing
	Material mat;
	Vector3 scale_factor;
	Vector3 location;

	//Draws the mesh to the screen
	void draw();
	
	TriangleMesh(std::string file_name);
	~TriangleMesh(void);
};

