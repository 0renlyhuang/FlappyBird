#ifndef TUBE_H
#define TUBE_H

#include <cstddef>
#include <memory>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "drawAble.h"
#include "shader.h"
#include "collisionWorld.h"


namespace TubeSp
{
	auto deletor = [](GLfloat *p) {delete[] p; };
	using ArrayDelete = decltype(deletor);
	constexpr std::size_t SIZE = 3 * 6 * 2;
	constexpr GLfloat WIDTH = 0.1f;
	constexpr GLfloat HEIGHT = 2.0f;

	inline auto getVertices(const GLfloat space)
	{
		return std::unique_ptr<GLfloat, ArrayDelete>(
			new GLfloat[SIZE]
		{  
			-WIDTH,  HEIGHT, 0.0f,
			-WIDTH, space, 0.0f,
			WIDTH, space, 0.0f,
			WIDTH, space, 0.0f,
			WIDTH,  HEIGHT, 0.0f,
			-WIDTH, HEIGHT, 0.0f,

			-WIDTH,  -space, 0.0f,
			-WIDTH, -HEIGHT, 0.0f,
			WIDTH, -HEIGHT, 0.0f,
			WIDTH, -HEIGHT, 0.0f,
			WIDTH,  -space, 0.0f,
			-WIDTH, -space, 0.0f
		},
		deletor);
	}
}


class Tube : public DrawAble {
public:
	using BoxT = utility::Collidable::BoxType;

	Tube(const glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, const GLfloat height = 0.0f, const GLfloat space = 0.3f)
		: position_(pos), vertices_(TubeSp::getVertices(space)),
		upBox_(BoxT::RETENCGEL,
			utility::PointT<float>(pos.x - 0.5f * TubeSp::WIDTH, pos.y + 0.5f * space + TubeSp::HEIGHT),  // UpBox top-left
			utility::PointT<float>(pos.x + 0.5f * TubeSp::WIDTH, pos.y + 0.5f * space)),  // UpBox bottom-right
		downBox_(BoxT::RETENCGEL,
			utility::PointT<float>(pos.x - 0.5f * TubeSp::WIDTH, pos.y - 0.5f * space),  // DownBox top-left
			utility::PointT<float>(pos.x + 0.5f * TubeSp::WIDTH, pos.y - 0.5f * space - TubeSp::HEIGHT))  // DownBox bottom-right	
	{
		std::cout << "tube initial position\n" << 
			"UP:\n" << 
			"Top-let: " << pos.x - 0.5f * TubeSp::WIDTH << " " <<
			pos.y + 0.5f * space + TubeSp::HEIGHT << "\n" <<
			"Bottom-right: " << pos.x + 0.5f * TubeSp::WIDTH << " " <<
			pos.y + 0.5f * space << "\n" << 
			"down:\n" <<
			"Top-let: " << pos.x - 0.5f * TubeSp::WIDTH << " " <<
			pos.y - 0.5f * space << "\n" <<
			"Bottom-right: " << pos.x + 0.5f * TubeSp::WIDTH << " " <<
			pos.y - 0.5f * space - TubeSp::HEIGHT << "\n" << endl;

		glGenVertexArrays(1, &this->VAO_);
		glBindVertexArray(this->VAO_);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		auto rp = vertices_.get();
		GLfloat(&vArray)[TubeSp::SIZE] = *reinterpret_cast<GLfloat(*)[TubeSp::SIZE]>(rp);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vArray), &vArray, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	void draw(Shader &shader) override {
		shader.use();

		glm::mat4 model;
		model = glm::translate(model, this->position_);
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"),
			1, GL_FALSE, glm::value_ptr(model));

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

	glm::vec3 position_;
	utility::Collidable upBox_;
	utility::Collidable downBox_;
private:
	const std::unique_ptr <GLfloat, TubeSp::ArrayDelete> vertices_;
	GLuint VAO_;
	//glm::vec3 position_;
	const static GLfloat speed_;
	//utility::Collidable upBox_;
	//utility::Collidable downBox_;
};


const GLfloat Tube::speed_ = -2.5f;


#endif // !TUBE_H
