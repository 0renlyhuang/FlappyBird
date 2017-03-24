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
#include "config.h"
#include "board.h"


class Button : public Board {
public:
	Button(const char *tex, const glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, const glm::vec3 scale = { 1.0f, 1.0f, 1.0f }) 
		: Board(tex, pos, scale), isDown_(false) { }

	void draw(Shader &shader) override {
		shader.use();

		glm::mat4 model;
		model = glm::translate(model, this->position_);
		model = glm::scale(model, this->isDown_ ? glm::vec3(this->scale_.x - 0.1f, this->scale_.y - 0.1f, 1.0f) : this->scale_);
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

	void down() { this->isDown_ = true; }

	void up() { this->isDown_ = false;  }

	// 判断鼠标是否在按钮上
	bool cover(const int mouseX, const int mouseY) {
		glPushMatrix();
		glTranslatef(this->position_.x, this->position_.y, this->position_.z);
		if (this->isDown_) {
			glScalef(this->scale_.x - 0.1f, this->scale_.y - 0.1f, 1.0f);
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
		model = glm::scale(model, this->isDown_ ? glm::vec3(this->scale_.x - 0.1f, this->scale_.y - 0.1f, 1.0f) : this->scale_);
		// object coordinate
		glm::vec4 pos{ posX, posY, posZ, 1.0f };
		pos = glm::inverse(PROJECTION) * glm::inverse(model) * pos;

		return pos.x > -BoardSp::HALFEDGE && pos.x < BoardSp::HALFEDGE
			&& pos.y > -BoardSp::HALFEDGE && pos.y < BoardSp::HALFEDGE;
	}
	

protected:
	bool isDown_;
};


#endif // !BUTTON_H
