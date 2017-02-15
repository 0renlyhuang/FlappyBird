#ifndef BUTTON_H
#define BUTTON_H

#include <memory>
#include "GL\glew.h"
#include "GL\SOIL.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "drawAble.h"
#include "shader.h"


namespace ButtonSp
{
	auto deletor = [](GLfloat *p) {delete[] p; };
	using ArrayDelete = decltype(deletor);
	constexpr  std::size_t SIZE = 5 * 6;
	constexpr GLfloat HALFEDGE = 10.0f;//10.0f;

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


class Button : public DrawAble {
public:
	Button(const char *tex, glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, glm::vec3 scale = { 5.0f, 5.0f, 1.0f }) 
		: position_(pos), isDown_(false), scale_(scale) 
	{
		glGenTextures(1, &this->texture_);
		glBindTexture(GL_TEXTURE_2D, this->texture_);

		int textureWidth, textureHeight;
		unsigned char* image = SOIL_load_image(tex, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);


		glGenVertexArrays(1, &this->VAO_);
		glBindVertexArray(this->VAO_);
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		auto rp = vertices_.get();
		GLfloat(&vArray)[ButtonSp::SIZE] = *reinterpret_cast<GLfloat(*)[ButtonSp::SIZE]>(rp);

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
		model = glm::translate(model, this->position_);
		model = glm::scale(model, this->isDown_ ? glm::vec3(this->scale_.x - 0.5f, this->scale_.y - 0.5f, 1.0f) : this->scale_);
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"),
			1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 projection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"),
			1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glUniform1i(glGetUniformLocation(shader.getProgram(), "buttonTex"), 0);

		glBindVertexArray(this->VAO_);
		glDrawArrays(GL_TRIANGLES, 0, ButtonSp::SIZE / 3);
		glBindVertexArray(0);
	}

	void down() { this->isDown_ = true; }

	void up() { this->isDown_ = false;  }

	bool cover(const int mouseX, const int mouseY) {
		glPushMatrix();
		glTranslatef(this->position_.x, this->position_.y, this->position_.z);
		if (this->isDown_) {
			glScalef(this->scale_.x - 0.5f, this->scale_.y - 0.5f, 1.0f);
		}
		else {
			glScalef(this->scale_.x, this->scale_.y, this->scale_.z);
		}
		

		GLint    viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		// 以左下为原点的屏幕坐标
		GLfloat  winX, winY, winZ;
		// clip space
		GLdouble posX, posY, posZ;


		glGetIntegerv(GL_VIEWPORT, viewport);   
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		glPopMatrix();

		winX = mouseX;
		winY = viewport[3] - mouseY;
		glReadPixels(mouseX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		// cout << "mouse position: " << "posX: " << posX << " posY: " << posY << endl;
		

		glm::mat4 model;
		model = glm::translate(model, this->position_);
		model = glm::scale(model, this->isDown_ ? glm::vec3(this->scale_.x - 0.5f, this->scale_.y - 0.5f, 1.0f) : this->scale_);
		glm::mat4 proj = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, -1.0f, 1.0f);
		// object coordinate
		glm::vec4 pos{ posX, posY, posZ, 1.0f };
		pos = glm::inverse(proj) * glm::inverse(model) * pos;

		return pos.x > -ButtonSp::HALFEDGE && pos.x < ButtonSp::HALFEDGE
			&& pos.y > -ButtonSp::HALFEDGE && pos.y < ButtonSp::HALFEDGE;
	}
	

private:
	static const std::unique_ptr <GLfloat, ButtonSp::ArrayDelete> vertices_;
	const glm::vec3 position_;
	const glm::vec3 scale_;
	GLuint VAO_;
	GLuint texture_;
	bool isDown_;
};

const std::unique_ptr <GLfloat, ButtonSp::ArrayDelete> Button::vertices_(ButtonSp::getVertices());

#endif // !BUTTON_H
