#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include "GL\glew.h"
#include "GL\SOIL.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "drawAble.h"
#include "shader.h"
#include "config.h"


// 用于解耦，分离具体的顶点参数
namespace BoardSp
{
	auto deletor = [](GLfloat *p) {delete[] p; };
	using ArrayDelete = decltype(deletor);
	constexpr  std::size_t SIZE = 5 * 6;
	constexpr GLfloat HALFEDGE = 50.0f;//10.0f;

	auto getVertices()
	{
		return std::unique_ptr<GLfloat, ArrayDelete>(
			new GLfloat[SIZE]
		{
			-HALFEDGE,  HALFEDGE, 0.0f,	 0.0f, 1.0f,
			-HALFEDGE, -HALFEDGE, 0.0f,	 0.0f, 0.0f,
			HALFEDGE, -HALFEDGE, 0.0f,	 1.0f, 0.0f,
			HALFEDGE, -HALFEDGE, 0.0f,	 1.0f, 0.0f,
			HALFEDGE,  HALFEDGE, 0.0f,	 1.0f, 1.0f,
			-HALFEDGE, HALFEDGE, 0.0f,	 0.0f, 1.0f
		},
			deletor);
	}
}


/*
\ 表示静态展板的类，用于贴上纹理
*/
class Board : public DrawAble {
public:
	Board(const char *tex, const glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, const glm::vec3 scale = { 1.0f, 1.0f, 1.0f })
		: Board(pos, scale)
	{
		glGenTextures(1, &this->texture_);
		glBindTexture(GL_TEXTURE_2D, this->texture_);

		int textureWidth, textureHeight;
		unsigned char* image = SOIL_load_image(tex, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void draw(Shader &shader) override {
		shader.use();

		glm::mat4 model;
		model = glm::translate(model, this->position_);
		model = glm::scale(model, this->scale_);
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"),
			1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"),
			1, GL_FALSE, glm::value_ptr(PROJECTION));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glUniform1i(glGetUniformLocation(shader.getProgram(), "tex"), 0);

		glBindVertexArray(this->VAO_);
		glDrawArrays(GL_TRIANGLES, 0, BoardSp::SIZE / 3);
		glBindVertexArray(0);
	}

protected:
	Board(const glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, const glm::vec3 scale = { 1.0f, 1.0f, 1.0f })
		: position_(pos), scale_(scale)
	{
		glGenVertexArrays(1, &this->VAO_);
		glBindVertexArray(this->VAO_);
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		auto rp = vertices_.get();
		GLfloat(&vArray)[BoardSp::SIZE] = *reinterpret_cast<GLfloat(*)[BoardSp::SIZE]>(rp);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vArray), &vArray, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	static const std::unique_ptr <GLfloat, BoardSp::ArrayDelete> vertices_;
	glm::vec3 position_;
	const glm::vec3 scale_;
	GLuint VAO_;
	GLuint texture_;
};

const std::unique_ptr <GLfloat, BoardSp::ArrayDelete> Board::vertices_(BoardSp::getVertices());

#endif // !BOARD_H