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


//namespace BirdSp 
//{
//	auto deletor = [](GLfloat *p) {delete[] p; };
//	using ArrayDelete = decltype(deletor);
//	constexpr  std::size_t SIZE = 5 * 6;
//	constexpr GLfloat HALFEDGE = 30.0f;//10.0f;
//
//	auto getVertices() 
//	{
//		return std::unique_ptr<GLfloat, ArrayDelete>(
//			new GLfloat[SIZE]
//		{
//			-HALFEDGE,  HALFEDGE, 0.0f,	 0.0f, 1.0f,
//			-HALFEDGE, -HALFEDGE, 0.0f,	 0.0f, 0.0f,
//			HALFEDGE, -HALFEDGE, 0.0f,	 1.0f, 0.0f,
//			HALFEDGE, -HALFEDGE, 0.0f,	 1.0f, 0.0f,
//			HALFEDGE,  HALFEDGE, 0.0f,	 1.0f, 1.0f,
//			-HALFEDGE, HALFEDGE, 0.0f,	 0.0f, 1.0f
//		},	
//		deletor);
//	}
//}
//
//
//
//class Bird : public DrawAble, public utility::Collidable {
//public:
//	using BoxT = utility::Collidable::BoxType;
//
//	Bird(const glm::vec3 &pos, const GLfloat speed = -1000.0f) 
//		: vertices_(BirdSp::getVertices()), speed_(speed),
//			utility::Collidable(BoxT::RETENCGEL, pos, 2.0f * (BirdSp::HALFEDGE - 5.0f), 2.0f * (BirdSp::HALFEDGE - 5.0f))
//	{
//		//std::cout << "bird initial position\n" <<
//		//	"Top-let: " << pos.x - 0.5f * BirdSp::EDGE << " " << 
//		//		pos.y + 0.5f * BirdSp::EDGE << " " << 
//		//	"Bottom-right: " << pos.x + 0.5f * BirdSp::EDGE << " " << 
//		//		pos.y - 0.5f * BirdSp::EDGE << "\n" << endl;
//
//		glGenTextures(1, &this->texture_);
//		glBindTexture(GL_TEXTURE_2D, this->texture_);
//
//		int textureWidth, textureHeight;
//		unsigned char* image = SOIL_load_image("bird.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		SOIL_free_image_data(image);
//		glBindTexture(GL_TEXTURE_2D, 0);
//
//
//		glGenVertexArrays(1, &this->VAO_);
//		glBindVertexArray(this->VAO_);
//		GLuint VBO;
//		glGenBuffers(1, &VBO);
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//		auto rp = vertices_.get();
//		GLfloat(&vArray)[BirdSp::SIZE] = *reinterpret_cast<GLfloat(*)[BirdSp::SIZE]>(rp);
//
//		glBufferData(GL_ARRAY_BUFFER, sizeof(vArray), &vArray, GL_STATIC_DRAW);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
//		glEnableVertexAttribArray(1);
//
//		glBindVertexArray(0);
//	}
//
//	void fly() {
//		this->speed_ = 5500.0f;
//	}
//
//	void fall(const GLfloat deltaTime) {
//		static int cnt = 0;
//		++cnt;
//		
//		
//		this->position().y += utility::Motion::displacement(this->speed_, deltaTime);
//		this->speed_ = utility::Motion::velocity(this->speed_, deltaTime);
//		
//		// if (cnt % 100 == 0) {
//			//cout << "bird: \n";
//			//cout << "deltaTime: " << deltaTime << endl;
//			//cout << "speed: " <<  this->speed_ << endl;
//			//cout << "position: " << this->position().x << " " << this->position().y << " " << this->position().z << endl;
//			//cout << endl;
//		// }
//	}
//
//	void draw(Shader &shader) override {
//		shader.use();
//		
//		glm::mat4 model;
//		model = glm::translate(model, this->position());
//		// model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
//		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"),
//			1, GL_FALSE, glm::value_ptr(model));
//
//		//********************
//		//glm::mat4 projection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, -1.0f, 1.0f);
//		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"),
//			1, GL_FALSE, glm::value_ptr(PROJECTION));
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture_);
//		glUniform1i(glGetUniformLocation(shader.getProgram(), /*"birdTex"*/"tex"), 0);
//
//		glBindVertexArray(this->VAO_);
//		glDrawArrays(GL_TRIANGLES, 0, BirdSp::SIZE / 3);
//		glBindVertexArray(0);
//	}
//
//private:
//	const std::unique_ptr <GLfloat, BirdSp::ArrayDelete> vertices_;
//	GLfloat speed_;
//	GLuint VAO_;
//	GLuint texture_;
//};


#include "displayBoard.h"

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
	{
		//std::cout << "bird initial position\n" <<
		//	"Top-let: " << pos.x - 0.5f * BirdSp::EDGE << " " << 
		//		pos.y + 0.5f * BirdSp::EDGE << " " << 
		//	"Bottom-right: " << pos.x + 0.5f * BirdSp::EDGE << " " << 
		//		pos.y - 0.5f * BirdSp::EDGE << "\n" << endl;
	}

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
