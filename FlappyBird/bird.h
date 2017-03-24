#ifndef BIRD_H
#define BIRD_H

#include <cstddef>
#include <memory>
#include "GL\glew.h"
#include "GL\SOIL.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "physic.h"
#include "drawAble.h"
#include "shader.h"
#include "collisionWorld.h"
#include "config.h"
#include "displayBoard.h"

/*
\  用于定义鸟的类型
*/
class Bird : public DisplayBoard, public utility::Collidable {
public:
	using BoxT = utility::Collidable::BoxType;

	Bird(const glm::vec3 &pos, const GLfloat speed = -1000.0f)
		: DisplayBoard({ 
						 "texture//birdNormal.png", "texture//birdFlutterDownNormal.png", "texture//birdFlutterUpNormal.png",
						 "texture//birdNormalFly.png", "texture//birdFlutterDownFly.png", "texture//birdFlutterUpFly.png",
						 "texture//birdNormalFall.png", "texture//birdFlutterDownFall.png", "texture//birdFlutterUpFall.png"		 
					   },
		  pos, {0.6f, 0.6f, 1.0f}),
		  utility::Collidable(BoxT::RETENCGEL, pos, 2.0f * (BoardSp::HALFEDGE * 0.6f - 5.0f), 2.0f * (BoardSp::HALFEDGE * 0.6f - 5.0f)),
		  speed_(speed)
	{}

	void fly() {
		this->speed_ = 5500.0f;
		this->setTexture(Fly);
	}

	void fall(const GLfloat deltaTime) {
		this->Board::position_.y += utility::Motion::displacement(this->speed_, deltaTime);
		// this->utility::Collidable::position().y += utility::Motion::displacement(this->speed_, deltaTime);
		this->utility::Collidable::position().y = this->Board::position_.y;
		this->speed_ = utility::Motion::velocity(this->speed_, deltaTime);
		if (this->speed_ < 1000.0f && this->speed_ > -1000.0f) {
			if (this->getIndex() > FlutterUpNormal)
				this->setTexture(Normal);
		}
		else if (this->speed_ < 0.0f) {
			if (this->getIndex() < Fall)
				this->setTexture(Fall);
		}
	}

	// 用于实现鸟煽动翅膀的动画
	void flutter() {
		switch (this->getIndex())
		{
		case Normal:
			this->setTexture(FlutterUpNormal);
			break;
		
		case FlutterUpNormal:
			this->setTexture(FlutterDownNormal);
			break;
		
		case FlutterDownNormal:
			this->setTexture(Normal);
			break;

		case Fall:
  			this->setTexture(FlutterUpFall);
			break;

		case FlutterUpFall:
			this->setTexture(FlutterDownFall);
			break;

		case FlutterDownFall:
			this->setTexture(Fall);
			break;

		case Fly:
			this->setTexture(FlutterUpFly);
			break;

		case FlutterUpFly:
			this->setTexture(FlutterDownFly);
			break;

		case FlutterDownFly:
			this->setTexture(Fly);
			break;

		default:
			break;
		}
	}

private:
	enum { Normal = 0, FlutterDownNormal, FlutterUpNormal,
		   Fly, FlutterDownFly, FlutterUpFly,
		   Fall, FlutterDownFall, FlutterUpFall};
	GLfloat speed_;
};

#endif // !BIRD_H
