#include <GL/glew.h>
#include "SceneGraph.h"
#include <glut.h>
#include <iostream>
#include <math.h>
#include "Animator.h"

#define X_PIXELS 512
#define Y_PIXELS 512

//This global variable describes the contents of the scene. In mainLoopFunction(), all of the
//elements in scene_objects are drawn at the time instant specified by frame_number
SceneGraph scene = SceneGraph();
int frame_number = 0;

void mainLoopFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.drawAtTimeIndex(frame_number++);
	glutPostRedisplay();
	glutSwapBuffers();
}

int main(int a, char** c)
{
	glutInit(&a, c);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(X_PIXELS, Y_PIXELS);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1000);
	glutCreateWindow("Bunny");
	glewInit();
	glEnable(GL_DEPTH_TEST);
	
	scene.loadMeshFromString("../Triangle Mesh Files/Bunny Mesh.obj");
	ImagePlane cur_im_plane = {-0.1, 0.1, -0.1, 0.1, 0.1, 1000, X_PIXELS, Y_PIXELS};
	scene.setFrustum(cur_im_plane);
	//scene.loadMeshFromString("../Triangle Mesh Files/Space Shuttle Mesh.obj");
	//scene.loadMeshFromString("../Triangle Mesh Files/slot_machine.obj");

	// One band. Low cutoff is 0 Hz, High cutoff is 20 KHz
	int freq_divider[] = {0, 20000};
	ModulationSource mod_src = *ModulationSource::initModSource("../Wav Imports/Santa Claus is HOT HOT HOT.wav", 1, freq_divider, 30);
	Animator animator = Animator(scene.getIthObj(0), &mod_src);
	Vector3 huge = {30, 100, 0.5};
	Material final_mat = Material(RGBColor(0.7, 0, 0), RGBColor(0, 0, 1), RGBColor(0, 1, 1), RGBColor(0.7, 0, 0));
	animator.excited_state.scale = huge;
	animator.excited_state.mat = final_mat;
	scene.addAnimation(animator);
	glutDisplayFunc(mainLoopFunction);
	glutMainLoop();
	return 0;
}