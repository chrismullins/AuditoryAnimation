#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(std::string file_name)
{
	std::vector<Vector3> gNormals = std::vector<Vector3>();
	std::vector<Vector3> gPositions = std::vector<Vector3>();
	std::vector<Triangle> gTriangles = std::vector<Triangle>();
	load_mesh(file_name);
}

void TriangleMesh::load_mesh(std::string fileName)
{
	std::ifstream fin(fileName.c_str());
	if (!fin.is_open())
	{
		printf("ERROR: Unable to load mesh from %s!\n", fileName.c_str());
		exit(0);
	}

	float xmin = FLT_MAX;
	float xmax = -FLT_MAX;
	float ymin = FLT_MAX;
	float ymax = -FLT_MAX;
	float zmin = FLT_MAX;
	float zmax = -FLT_MAX;

	while (true)
	{
		char line[1024] = {0};
		fin.getline(line, 1024);

		if (fin.eof())
			break;

		if (strlen(line) <= 1)
			continue;

		std::vector<std::string> tokens;
		tokenize(line, tokens, " ");

		if (tokens[0] == "v")
		{
			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			float z = atof(tokens[3].c_str());

			xmin = std::min(x, xmin);
			xmax = std::max(x, xmax);
			ymin = std::min(y, ymin);
			ymax = std::max(y, ymax);
			zmin = std::min(z, zmin);
			zmax = std::max(z, zmax);

			Vector3 position = {x, y, z};
			gPositions.push_back(position);
		}
		else if (tokens[0] == "vn")
		{
			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			float z = atof(tokens[3].c_str());
			Vector3 normal = {x, y, z};
			gNormals.push_back(normal);
		}
		else if (tokens[0] == "f")
		{
			unsigned int a = face_index(tokens[1].c_str());
			unsigned int b = face_index(tokens[2].c_str());
			unsigned int c = face_index(tokens[3].c_str());
			Triangle triangle;
			triangle.indices[0] = a - 1;
			triangle.indices[1] = b - 1;
			triangle.indices[2] = c - 1;
			gTriangles.push_back(triangle);
		}
	}

	fin.close();

	printf("Loaded mesh from %s. (%lu vertices, %lu normals, %lu triangles)\n", fileName.c_str(), gPositions.size(), gNormals.size(), gTriangles.size());
	printf("Mesh bounding box is: (%0.4f, %0.4f, %0.4f) to (%0.4f, %0.4f, %0.4f)\n", xmin, ymin, zmin, xmax, ymax, zmax);
	init();
}

int TriangleMesh::face_index(const char* string)
{
	int length = strlen(string);
	char* copy = new char[length + 1];
	memset(copy, 0, length+1);
	strcpy(copy, string);

	std::vector<std::string> tokens;
	tokenize(copy, tokens, "/");
	delete[] copy;
	if (tokens.front().length() > 0 && tokens.back().length() > 0 && atoi(tokens.front().c_str()) == atoi(tokens.back().c_str()))
	{
		return atoi(tokens.front().c_str());
	}
	else
	{
		printf("ERROR: Bad face specifier!\n");
		exit(0);
	}
}

void TriangleMesh::tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter)
{
	char* token = strtok(string, delimiter);
	while (token != NULL)
	{
		tokens.push_back(std::string(token));
		token = strtok(NULL, delimiter);
	}
}

// Resets the model view matrix for a particular triangle mesh based on it's location and scale factor
void TriangleMesh::setModelTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(location.x, location.y, location.z);
	glScalef(scale_factor.x, scale_factor.y, scale_factor.z);
}

// Resets the material properties of this TriangleMesh
// NOTE: There is a bug in the usage of this function. The mechanism by which colors are updated does not seem to
// care when/if this is called.
void TriangleMesh::setMaterial()
{
	float ka[] = {mat.ambient.red, mat.ambient.green, mat.ambient.blue, 1};
	float kd[] = {mat.diffuse.red, mat.diffuse.green, mat.diffuse.blue, 0};
	float ks[] = {mat.specular.red, mat.specular.green, mat.specular.blue, 0};
	float const p = mat.phong_exponent;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
	glMaterialfv(GL_FRONT, GL_SHININESS, &p);
}

//This function binds gPositions, gNormals and gTriangles to vertex, normal and index buffers respectively.
void TriangleMesh::init()
{
	//Initialization of buffers
	glEnableClientState(GL_VERTEX_ARRAY);
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//bunny_mesh.buffer_size is the number of vertices. Multiply by 6 to get the total number of floats in the array
	glBufferData(GL_ARRAY_BUFFER, sizeof( Vector3 ) * (*this).gPositions.size(), &(*this).gPositions[0], GL_STATIC_DRAW);

	glVertexPointer(3, GL_FLOAT, 0, 0);

	//Bind gNormals to a buffer
	if (gNormals.size() > 0)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		unsigned int vbo_norm;
		glGenBuffers(1, &vbo_norm);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
		glBufferData(GL_ARRAY_BUFFER, sizeof( Vector3 ) * (*this).gNormals.size(), &(*this).gNormals[0], GL_STATIC_DRAW);
		glNormalPointer(GL_FLOAT, 0, 0);
	}
	//Now indices
	glEnableClientState(GL_INDEX_ARRAY);
	unsigned int vbo_ind;
	glGenBuffers(1, &vbo_ind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ind);

	//bunny_mesh.buffer_size is the number of vertices. Multiply by 6 to get the total number of floats in the array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triangle) * (*this).gTriangles.size(), &(*this).gTriangles[0], GL_STATIC_READ);

	glIndexPointer(GL_INDEX_ARRAY, 0, 0);
}

void TriangleMesh::draw()
{
	glDrawElements(GL_TRIANGLES, 3 * (*this).gTriangles.size(), GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh(void)
{
}

void TriangleMesh::setMaterial(Material m)
{
	mat = m;
	setMaterial();
}

void TriangleMesh::update()
{
	setMaterial();
	setModelTransform();
}