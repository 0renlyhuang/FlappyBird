#ifndef DISPLAYBOARD_H
#define DISPLAYBOARD_H

#include <vector>
#include "board.h"
#include "GL\SOIL.h"

class DisplayBoard : public Board {
public:
	DisplayBoard(const std::vector<const char*> &texs, 
		const glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, 
		const glm::vec3 scale = { 1.0f, 1.0f, 1.0f })
		: Board(pos, scale), texs_(texs.size())
	{
		glGenTextures(texs.size(), &texs_[0]);
		for (int i = 0; i < texs.size(); ++i) {
			glBindTexture(GL_TEXTURE_2D, texs_[i]);

			int textureWidth, textureHeight;
			unsigned char* image = SOIL_load_image(texs[i], &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
			// **************
			cout << i << " " << SOIL_last_result() << endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);

			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		this->texture_ = texs_[0];
	}

	void setTexture(int index) {
		this->texture_ = this->texs_[index];
	}


private:
	std::vector<GLuint> texs_;
};

#endif // !DISPLAYBOARD_H
