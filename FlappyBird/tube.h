#ifndef TUBE_H
#define TUBE_H

#include <cstddef>
#include <memory>
#include "GL\glew.h"
#include "GL\SOIL.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "drawAble.h"
#include "shader.h"
#include "collisionWorld.h"
#include "config.h"


namespace TubeSp
{
	auto deletor = [](GLfloat *p) {delete[] p; };
	using ArrayDelete = decltype(deletor);
	constexpr std::size_t SIZE = /*3*/5 * 6 * 2;
	constexpr GLfloat WIDTH = 50.0f; //0.1f;
	constexpr GLfloat HEIGHT = 800.0f;//2.0f;

	inline auto getVertices(const GLfloat halfSpace)
	{
		return std::unique_ptr<GLfloat, ArrayDelete>(
			new GLfloat[SIZE]
		{  
			-WIDTH,  HEIGHT + halfSpace, 0.0f,	0.0f, 1.0f,
			-WIDTH, halfSpace, 0.0f,			0.0f, 0.0f, 
			WIDTH, halfSpace, 0.0f,				1.0f, 0.0f,
			WIDTH, halfSpace, 0.0f,				1.0f, 0.0f,
			WIDTH,  HEIGHT + halfSpace, 0.0f,	1.0f, 1.0f,
			-WIDTH, HEIGHT + halfSpace, 0.0f,	0.0f, 1.0f,

			-WIDTH,  -halfSpace, 0.0f,			0.0f, -1.0f,
			-WIDTH, -HEIGHT - halfSpace, 0.0f,	0.0f, 0.0f,
			WIDTH, -HEIGHT - halfSpace, 0.0f,	1.0f, 0.0f,
			WIDTH, -HEIGHT - halfSpace, 0.0f,	1.0f, 0.0f,
			WIDTH,  -halfSpace, 0.0f,			1.0f, -1.0f,
			-WIDTH, -halfSpace, 0.0f,			0.0f, -1.0f
		},
		deletor);
	}
}


// 用于表示油管的类
class Tube : public DrawAble {
public:
	using BoxT = utility::Collidable::BoxType;

	Tube(const glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, const GLfloat height = 0.0f, const GLfloat halfSpace = /*110.0f*/130.0f)
		: position_(pos), vertices_(TubeSp::getVertices(halfSpace)),
		upBox_(BoxT::RETENCGEL,
			glm::vec3(pos.x, pos.y + halfSpace + 0.5f * TubeSp::HEIGHT, pos.z), 2.0f * TubeSp::WIDTH,  TubeSp::HEIGHT), // UpBox
		downBox_(BoxT::RETENCGEL,
			glm::vec3(pos.x, pos.y - halfSpace - 0.5f * TubeSp::HEIGHT, pos.z), 2.0f * TubeSp::WIDTH, TubeSp::HEIGHT)  // DownBox
	{
		
		if (!this->textureLoaded_) {
			glGenTextures(1, &this->texture_);
			glBindTexture(GL_TEXTURE_2D, this->texture_);

			int textureWidth, textureHeight;
			unsigned char* image = SOIL_load_image("texture//tube.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);

			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);

			this->textureLoaded_ = true;
		}


		glGenVertexArrays(1, &this->VAO_);
		glBindVertexArray(this->VAO_);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		auto rp = vertices_.get();
		GLfloat(&vArray)[TubeSp::SIZE] = *reinterpret_cast<GLfloat(*)[TubeSp::SIZE]>(rp);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vArray), &vArray, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, /*3*/5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void draw(Shader &shader) override {
		shader.use();

		glm::mat4 model;
		model = glm::translate(model, this->position_);
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"),
			1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"),
			1, GL_FALSE, glm::value_ptr(PROJECTION));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glUniform1i(glGetUniformLocation(shader.getProgram(), "wallTex"), 0);

		glBindVertexArray(this->VAO_);
		glDrawArrays(GL_TRIANGLES, 0, TubeSp::SIZE / 3);
		glBindVertexArray(0);
	}

	void shift(const GLfloat deltaTime) {
		this->position_.x += this->speed_ * deltaTime;
		this->upBox_.position().x += this->speed_ * deltaTime;
		this->downBox_.position().x += this->speed_ * deltaTime;
	}

	auto getUpBox() const noexcept { return this->upBox_; }
	auto getDownBox() const noexcept { return this->downBox_; }

	const glm::vec3 &position() noexcept { return this->position_; }

private:
	const std::unique_ptr <GLfloat, TubeSp::ArrayDelete> vertices_;
	GLuint VAO_;
	const static GLfloat speed_;
	static GLuint texture_;
	static bool textureLoaded_;
	glm::vec3 position_;
	utility::Collidable upBox_;
	utility::Collidable downBox_;
};


const GLfloat Tube::speed_ = -2500.0f;//-2500.0f;//-2.5f;
GLuint Tube::texture_;
bool Tube::textureLoaded_ = false;


#endif // !TUBE_H
