#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include "gl\glew.h"
#include "gl\freeglut.h"
#include "glm\glm.hpp"
#include "shader.h"
#include "board.h"
#include "bird.h"
#include "tube.h"
#include "collisionWorld.h"
#include "button.h"
#include "config.h"


using std::cerr;
using std::endl;
using std::unique_ptr;


void init();
void display();
void spaceDown(unsigned char key, int, int);
void spaceUp(unsigned char key, int, int);
void mouseClick(int button, int state, int x, int y);


bool isStarted = false;
bool startButtonDown = false;
bool isSpaceDown = false;
GLfloat deltaTime = 0.0;
GLfloat lastFrame = 0.0;

constexpr std::size_t tubeNum = 10;

unique_ptr<Button> pStartButton;
unique_ptr<Board> pBackground;
unique_ptr<Board> pTitle;
unique_ptr<Bird> pBird;
unique_ptr<Shader> pButtonShader;
unique_ptr<Shader> pTubeShader;
unique_ptr<Shader> pBirdShader;
unique_ptr<Shader> pBackgroundShader;
std::vector<unique_ptr<Tube>> tubes;


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGTH);
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
	glutMouseFunc(mouseClick);
	//glutMotionFunc(mouseMoveDetetor);



	// 3.进入事件循环体系
	// 一个无限循环
	glutMainLoop();
}



void init() {
	glEnable(GL_DEPTH_TEST);

	utility::CollisionWorld::setUp();

	pStartButton = std::make_unique<Button>("startButton.png");
	pButtonShader = std::make_unique<Shader>("board.vert", "board.frag");

	pTitle = std::make_unique<Board>("title.png", glm::vec3{ 0.0f, 200.0f, 0.0f }, glm::vec3{ 2.2f, 2.0f, 1.0f });

	pBackground = std::make_unique<Board>("background.png", glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{10.0f, 10.0f, 1.0f});
	pBackgroundShader = std::make_unique<Shader>("board.vert", "board.frag");

	pBird = std::make_unique<Bird>(glm::vec3{ 0.0f, -109.693f, 0.0f });
	pBirdShader = std::make_unique<Shader>("bird.vert", "bird.frag");

	std::default_random_engine e;

	for (int i = 0; i < tubeNum; ++i) {
		tubes.emplace_back(std::make_unique<Tube>(
			glm::vec3(500.0f + 300.0f * static_cast<float>(i), (static_cast<int>(e() % 9) - 4) * 80.0f, 0.0f/*30.0f, -160.0f, 0.0f*/)
			));
	}
	pTubeShader = std::make_unique<Shader>("tube.vert", "tube.frag");
}



void display() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat currFrame = 0.0001f * glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currFrame - lastFrame;
	lastFrame = currFrame;
	
	if (!isStarted) {
		pButtonShader->use();
		pStartButton->draw(*pButtonShader);
		
		pBackgroundShader->use();
		pTitle->draw(*pBackgroundShader);
	}
	else {
		pBirdShader->use();

		if (isSpaceDown) {
			pBird->fly();
		}

		pBird->fall(deltaTime);
		pBird->draw(*pBirdShader);


		pTubeShader->use();
		for (auto &ptube : tubes) {
			static int cnt = 0;
			++cnt;
			ptube->shift(deltaTime);
			ptube->draw(*pTubeShader);

			auto bird_position = pBird->position();
			auto down_box = ptube->getDownBox().position();
			auto up_box = ptube->getUpBox().position();
			// 如果碰撞
			if (pBird->collisionDetect(ptube->getDownBox()) || pBird->collisionDetect(ptube->getUpBox())) {
				std::cout << "collision: \n" <<
					"Bird pos: \n" <<
					"x: " << pBird->position().x <<
					"\ny: " << pBird->position().y << "\n" <<
					"top-left:\n" <<
					"x: " << pBird->pBox()->topLeft().first << "\n" <<
					"y: " << pBird->pBox()->topLeft().second << "\n" <<
					"bottom-right:\n" <<
					"x: " << pBird->pBox()->bottomRight().first << "\n" <<
					"y: " << pBird->pBox()->bottomRight().second << "\n" <<
					endl;

				std::cout << "tube pos: \n" <<
					"up\n" <<
					"top-left:\n" <<
					"x: " << ptube->getUpBox().pBox()->topLeft().first << "\n" <<
					"y: " << ptube->getUpBox().pBox()->topLeft().second << "\n" <<
					"bottom-right:\n" <<
					"x: " << ptube->getUpBox().pBox()->bottomRight().first << "\n" <<
					"y: " << ptube->getUpBox().pBox()->bottomRight().second << "\n" <<
					"\n" <<
					"down\n" <<
					"top-left:\n" <<
					"x: " << ptube->getDownBox().pBox()->topLeft().first << "\n" <<
					"y: " << ptube->getDownBox().pBox()->topLeft().second << "\n" <<
					"bottom-right:\n" <<
					"x: " << ptube->getDownBox().pBox()->bottomRight().first << "\n" <<
					"y: " << ptube->getDownBox().pBox()->bottomRight().second << "\n" <<
					endl;

				system("Pause");
			}

		}
	}
	pBackgroundShader->use();
	pBackground->draw(*pBackgroundShader);

 	glFlush();
}



void spaceDown(unsigned char key, int, int) {
	if (key == ' ') {
		isSpaceDown = true;
	}

	if (key == 'a') {
		pStartButton->down();
	}
}

void spaceUp(unsigned char key, int, int) {
	if (key == ' ') {
		isSpaceDown = false;
	}
	if (key == 'a') {
		pStartButton->up();
	}
}

void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			cout << x << " " << y << endl;
			if (pStartButton->cover(x, y)) { pStartButton->down(); startButtonDown = true; }
		}
		else if (state == GLUT_UP) {
			if (startButtonDown) {
				pStartButton->up();
				startButtonDown = false;
				isStarted = true;
			}
		}
	}
}