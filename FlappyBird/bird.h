#ifndef BIRD_H
#define BIRD_H

#include <cstddef>
#include <memory>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "physic.h"
#include "drawAble.h"
#include "shader.h"


namespace BirdSp 
{
	auto deletor = [](GLfloat *p) {delete[] p; };
	using ArrayDelete = decltype(deletor);
	constexpr  std::size_t SIZE = 3 * 6;

	auto getVertices() 
	{
		return std::unique_ptr<GLfloat, ArrayDelete>(
			new GLfloat[SIZE]
		{
			-0.1f,  0.1f, 0.0f,
			-0.1f, -0.1f, 0.0f,
			0.1f, -0.1f, 0.0f,
			0.1f, -0.1f, 0.0f,
			0.1f,  0.1f, 0.0f,
			-0.1f, 0.1f, 0.0f,
		},	
		deletor);
	}
}



class Bird : public DrawAble, public utility::Collidable {
public:
	Bird(const glm::vec3 &pos, const GLfloat speed = -1.0f) 
		: vertices_(BirdSp::getVertices()), speed_(speed), position_(pos)
	{
		glGenVertexArrays(1, &this->VAO_);
		glBindVertexArray(this->VAO_);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		auto rp = vertices_.get();
		GLfloat(&vArray)[BirdSp::SIZE] = *reinterpret_cast<GLfloat(*)[BirdSp::SIZE]>(rp);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vArray), &vArray, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	void fly() {
		this->speed_ = 10.0f;
	}

	void fall(const GLfloat deltaTime) {
		static int cnt = 0;
		++cnt;
		
		this->position_.y += utility::Motion::displacement(this->speed_, deltaTime);
		this->speed_ = utility::Motion::velocity(this->speed_, deltaTime);
		
		if (cnt % 100 == 0) {
			cout << "deltaTime: " << deltaTime << endl;
			cout << this->speed_ << endl;
		}
	}

	void draw(Shader &shader) override {
		shader.use();
		
		glm::mat4 model;
		model = glm::translate(model, this->position_);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"),
			1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(this->VAO_);
		glDrawArrays(GL_TRIANGLES, 0, BirdSp::SIZE / 3);
		glBindVertexArray(0);
	}

private:
	const std::unique_ptr <GLfloat, BirdSp::ArrayDelete> vertices_;
	GLfloat speed_;
	GLuint VAO_;
	glm::vec3 position_;
};

#endif // !BIRD_H
