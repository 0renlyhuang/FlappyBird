#ifndef BACKGROUND
#define BACKGROUND

#include <memory>
#include "GL\glew.h"
#include "GL\SOIL.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "drawAble.h"
#include "shader.h"


namespace BackgroundSp
{
	auto deletor = [](GLfloat *p) {delete[] p; };
	using ArrayDelete = decltype(deletor);
	constexpr  std::size_t SIZE = 5 * 6;
	constexpr GLfloat HALFEDGE = 30.0f;//10.0f;

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


class Background : public DrawAble {
public:
	Background() : vertices_(BackgroundSp::getVertices()) {
		glGenTextures(1, &this->texture_);
		glBindTexture(GL_TEXTURE_2D, this->texture_);

		int textureWidth, textureHeight;
		unsigned char* image = SOIL_load_image("background.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);


		glGenVertexArrays(1, &this->VAO_);
		glBindVertexArray(this->VAO_);
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		auto rp = vertices_.get();
		GLfloat(&vArray)[BackgroundSp::SIZE] = *reinterpret_cast<GLfloat(*)[BackgroundSp::SIZE]>(rp);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vArray), &vArray, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void draw(Shader &shader) override {
		shader.use();

		glm::mat4 model;	
		model = glm::scale(model, glm::vec3(17.0f, 17.0f, 17.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"),
			1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 projection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"),
			1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glUniform1i(glGetUniformLocation(shader.getProgram(), "backgroundTex"), 0);

		glBindVertexArray(this->VAO_);
		glDrawArrays(GL_TRIANGLES, 0, BackgroundSp::SIZE / 3);
		glBindVertexArray(0);
	}
private:
	const std::unique_ptr <GLfloat, BackgroundSp::ArrayDelete> vertices_;
	GLuint VAO_;
	GLuint texture_;
};

#endif // !BACKGROUND
