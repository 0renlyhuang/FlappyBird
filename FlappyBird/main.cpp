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
#include "collisionWorld.h"


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

constexpr std::size_t tubeNum = 1;

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
	utility::CollisionWorld::setUp();

	pBird = std::make_unique<Bird>(glm::vec3{ 0.0f, 0.0f, 0.0f });
	pBirdShader = std::make_unique<Shader>("bird.vert", "bird.frag");

	std::default_random_engine e;

	for (int i = 0; i < tubeNum; ++i) {
		tubes.emplace_back(std::make_unique<Tube>(
			glm::vec3(0.2f + 0.5f * static_cast<float>(i), (static_cast<int>(e() % 9) - 4) * 0.1f, 0.0f)
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
		static int cnt = 0;
		++cnt;
		ptube->shift(deltaTime);
		ptube->draw(*pTubeShader);

		auto bird_position = pBird->position();  // 0.05
		auto down_box = ptube->getDownBox().position();  // 0.1*2
		auto up_box = ptube->getUpBox().position();
		// 如果碰撞
		if (pBird->collisionDetect(ptube->getDownBox()) || pBird->collisionDetect(ptube->getUpBox())) {
   			system("Pause");
		//	std::cout << "bird  position\n" <<
		//		pBird->position().x << "\n" <<
		//		pBird->position().y << "\n" <<
		//		pBird->position().z << "\n" <<
		//		endl;
		}
		auto a = 0;
	}

	

	glFlush();
}



void spaceDown(unsigned char key, int, int) {
	if (key == ' ') {
		isSpaceDown = true;
	}
	if (key == '1') {
			std::cout << "\n\n\n\nbird  position\n" <<
				"x: " << pBird->position().x << "\n" <<
				"y: " << pBird->position().y << "\n" <<
				"z: " << pBird->position().z << "\n" <<
				endl;

			std::cout << "\n\n\n\ntube  position\n" <<
				"x: " << tubes[0]->position_.x << "\n" <<
				"y: " << tubes[0]->position_.y << "\n" <<
				"z: " << tubes[0]->position_.z << "\n" <<
				endl;

			std::cout <<  
				"UP:\n" << 
				"Top-let: " << tubes[0]->position_.x - 0.5f * TubeSp::WIDTH << " " <<
				tubes[0]->position_.y + 0.5f * 0.3f + TubeSp::HEIGHT << "\n" <<
				"Bottom-right: " << tubes[0]->position_.x + 0.5f * TubeSp::WIDTH << " " <<
				tubes[0]->position_.y + 0.5f * 0.3f << "\n" <<
				"down:\n" <<
				"Top-let: " << tubes[0]->position_.x - 0.5f * TubeSp::WIDTH << " " <<
				tubes[0]->position_.y - 0.5f * 0.3f << "\n" <<
				"Bottom-right: " << tubes[0]->position_.x + 0.5f * TubeSp::WIDTH << " " <<
				tubes[0]->position_.y - 0.5f * 0.3f - TubeSp::HEIGHT << "\n" << endl;
		system("Pause");
	}
}

void spaceUp(unsigned char key, int, int) {
	if (key == ' ') {
		isSpaceDown = false;
	}
}