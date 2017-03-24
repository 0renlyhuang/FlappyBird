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
#include "SoundManager.h"
#include "button.h"
#include "scoreBoard.h"
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
bool isOver = false;
bool startButtonDown = false;
bool OKButtonDown = false;
bool isSpaceDown = false;
GLfloat deltaTime = 0.0;
GLfloat lastFrame = 0.0;

constexpr std::size_t tubeNum = 999;

unique_ptr<Button> pStartButton;
unique_ptr<Button> pOKButton;
unique_ptr<Board> pBackground;
unique_ptr<Board> pTitle;
unique_ptr<Board> pGameOver;
unique_ptr<Bird> pBird;
unique_ptr<ScoreBoard> pScore;
std::vector<unique_ptr<Tube>> tubes;
unique_ptr<Shader> pButtonShader;
unique_ptr<Shader> pTubeShader;
unique_ptr<Shader> pBoardShader;

int currTube = 0;  // Index
std::size_t wingSound;
std::size_t pointSound;
std::size_t dieSound;
std::size_t hitSound;
std::size_t clickSound;


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

	// 设置音效管理
	SoundManager::setUp(argc, argv);

	init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(spaceDown);
	glutKeyboardUpFunc(spaceUp);
	glutMouseFunc(mouseClick);


	glutMainLoop();
}



void init() {
	glEnable(GL_DEPTH_TEST);

	utility::CollisionWorld::setUp();

	pStartButton = std::make_unique<Button>("texture//startButton.png");
	pOKButton = std::make_unique<Button>("texture//OKButton.png", glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.41f, 0.5f, 1.0f });
	pButtonShader = std::make_unique<Shader>("board.vert", "board.frag");

	pTitle = std::make_unique<Board>("texture//title.png", glm::vec3{ 0.0f, 200.0f, 0.0f }, glm::vec3{ 2.2f, 2.0f, 1.0f });
	pGameOver = std::make_unique<Board>("texture//gameOver.png", glm::vec3{ 0.0f, 250.0f, 0.0f }, glm::vec3{ 4.0f, 4.0f, 1.0f });

	pBackground = std::make_unique<Board>("texture//background.png", glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{10.0f, 10.0f, 1.0f});
	
	
	pBoardShader = std::make_unique<Shader>("board.vert", "board.frag");

	pScore = std::make_unique<ScoreBoard>(glm::vec3{ 0.0f, 400.0f, 0.0f }, glm::vec3{ 0.26f, 0.36f, 1.0f }, 0);

	pBird = std::make_unique<Bird>(glm::vec3{ 0.0f, -109.693f, 0.0f });

	std::default_random_engine e;

	for (int i = 0; i < tubeNum; ++i) {
		tubes.emplace_back(std::make_unique<Tube>(
			glm::vec3(500.0f + 400.0f * static_cast<float>(i), (static_cast<int>(e() % 6) - 3) * 80.0f, 0.0f/*30.0f, -160.0f, 0.0f*/)
			));
	}
	pTubeShader = std::make_unique<Shader>("tube.vert", "tube.frag");

	auto pSoundManager = SoundManager::instance();
	wingSound = pSoundManager->load("sounds//wing.wav");
	pointSound = pSoundManager->load("sounds//point.wav");
	dieSound = pSoundManager->load("sounds//die.wav");
	hitSound = pSoundManager->load("sounds//hit.wav");
	clickSound = pSoundManager->load("sounds//buttonClick.wav");
}


// 重置游戏
void reInit() {
	pBird = std::make_unique<Bird>(glm::vec3{ 0.0f, -109.693f, 0.0f });
	std::default_random_engine e;
	tubes.clear();
	for (int i = 0; i < tubeNum; ++i) {
		tubes.emplace_back(std::make_unique<Tube>(
			glm::vec3(500.0f + 400.0f * static_cast<float>(i), (static_cast<int>(e() % 6) - 3) * 80.0f, 0.0f)
			));
	}
	currTube = 0;
	pScore->setValue(0);
}



void display() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat currFrame = 0.0001f * glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currFrame - lastFrame;
	lastFrame = currFrame;
	
	// 如果游戏还未开始,画开始按钮,标题
	if (!isStarted) {
		pButtonShader->use();
		pStartButton->draw(*pButtonShader);
		
		pBoardShader->use();
		pTitle->draw(*pBoardShader);
		
		// 如果上一次游戏结束,重新开始
		if (isOver) {
			isOver = false;
			reInit();
		}
	}
	// 如果游戏结束，画出重新开始的按钮，提示
	else if (isOver) {
		pBoardShader->use();
		pGameOver->draw(*pBoardShader);
		pButtonShader->use();
		pOKButton->draw(*pButtonShader);
	}
	// 如果游戏正在进行，画出积分板，画鸟，画管子
	else {
		pBoardShader->use();
		pScore->draw(*pBoardShader);


		pBoardShader->use();

		if (isSpaceDown) {	
			pBird->fly();
		}

		pBird->fall(deltaTime);
		
		// 确定翅膀扇动的频率
		static int flutterRate = 0;
		++flutterRate;
		if (flutterRate % 10 == 0)
			pBird->flutter();
		
		pBird->draw(*pBoardShader);


		pTubeShader->use();

		for (auto &ptube : tubes) {
			ptube->shift(deltaTime);
			ptube->draw(*pTubeShader);
		}

		if (currTube < tubes.size()) {

			// 如果和当前tube或者前一个tube碰撞
			if (currTube > 0
				? pBird->collisionDetect(tubes[currTube]->getDownBox())
				|| pBird->collisionDetect(tubes[currTube]->getUpBox())
				|| pBird->collisionDetect(tubes[currTube - 1]->getDownBox())
				|| pBird->collisionDetect(tubes[currTube - 1]->getUpBox())
				: pBird->collisionDetect(tubes[currTube]->getDownBox()) || pBird->collisionDetect(tubes[currTube]->getUpBox())) {
				// cout << "\nCollide\n";
				// system("Pause");
				SoundManager::instance()->play(hitSound);
				SoundManager::instance()->play(dieSound);
				isOver = true;
			}

			// 如果通过当前的tube
			if (pBird->position().x > tubes[currTube]->position().x) {
				pScore->setValue(pScore->getValue() + 1);
				++currTube;
				SoundManager::instance()->play(pointSound);
			}
		}

	}


	pBoardShader->use();
	pBackground->draw(*pBoardShader);

 	glFlush();
}


// 判断空格是否按下
void spaceDown(unsigned char key, int, int) {
	if (key == ' ') {
		if (isStarted && !isOver) {
			SoundManager::instance()->play(wingSound);		
		}
		isSpaceDown = true;
	}

	if (key == 'a') {
		for (auto &ptube : tubes) {
			std::cout << "(" <<
				ptube->position().x <<
				", " << ptube->position().y << ")\n";
			}
 		int a = 1;
	}
}

// 判断空格是否抬起
void spaceUp(unsigned char key, int, int) {
	if (key == ' ') {
		isSpaceDown = false;
	}
}

// 处理鼠标点击事件
void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			cout << x << " " << y << endl;
			if (!isStarted && pStartButton->cover(x, y)) { 
				pStartButton->down(); startButtonDown = true; 
				SoundManager::instance()->play(clickSound);
			}
			if (isOver && pOKButton->cover(x, y)) { 
				pOKButton->down(); 
				OKButtonDown = true; 
				SoundManager::instance()->play(clickSound);
			}
		}
		else if (state == GLUT_UP) {
			if (!isStarted && startButtonDown) {
				pStartButton->up();
				startButtonDown = false;
				isStarted = true;
			}
			if (isOver && OKButtonDown) {
				pOKButton->up();
				OKButtonDown = false;
				isStarted = false;			
			}
		}
	}
}