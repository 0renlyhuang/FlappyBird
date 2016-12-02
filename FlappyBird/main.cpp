#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include "gl\glew.h"
#include "gl\freeglut.h"
#include "glm\glm.hpp"
#include "shader.h"
#include "bird.h"
#include "tube.h"


using std::cerr;
using std::endl;
using std::unique_ptr;


void init();
void display();
void spaceDown(unsigned char key, int, int);
void spaceUp(unsigned char key, int, int);


bool isSpaceDown = false;
GLfloat deltaTime = 0.0;
GLfloat lastFrame = 0.0;

constexpr std::size_t tubeNum = 200;

unique_ptr<Bird> pBird;
unique_ptr<Shader> pTubeShader;
unique_ptr<Shader> pBirdShader;
std::vector<unique_ptr<Tube>> tubes;


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);

	if (glewInit()) {
		cerr << "Unable to initialize GLEW... exiting" << endl;
		std::exit(EXIT_FAILURE);
	}

	init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(spaceDown);
	glutKeyboardUpFunc(spaceUp);
	//glutMouseFunc(mouseLeftDetetor);
	//glutMotionFunc(mouseMoveDetetor);



	// 3.进入事件循环体系
	// 一个无限循环
	glutMainLoop();
}



void init() {
	pBird = std::make_unique<Bird>(glm::vec3{ 0.0f, 0.0f, 0.0f });
	pBirdShader = std::make_unique<Shader>("bird.vert", "bird.frag");

	std::default_random_engine e;

	for (int i = 0; i < tubeNum; ++i) {
		tubes.emplace_back(std::make_unique<Tube>(
			glm::vec3(0.0f + 0.5f * static_cast<float>(i), (static_cast<int>(e() % 9) - 4) * 0.1f, 0.0f)
			));
	}
	pTubeShader = std::make_unique<Shader>("tube.vert", "tube.frag");
}



void display() {
	GLfloat currFrame = 0.0001f * glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currFrame - lastFrame;
	lastFrame = currFrame;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pBirdShader->use();

	if (isSpaceDown) {
		pBird->fly();
	}
		
	pBird->fall(deltaTime);
	pBird->draw(*pBirdShader);

	for (auto &ptube : tubes) {
		ptube->shift(deltaTime);
		ptube->draw(*pTubeShader);
	}

	glFlush();
}



void spaceDown(unsigned char key, int, int) {
	if (key == ' ') {
		isSpaceDown = true;
	}
}

void spaceUp(unsigned char key, int, int) {
	if (key == ' ') {
		isSpaceDown = false;
	}
}