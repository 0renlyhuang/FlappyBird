#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "shader.h"

class DrawAble {
public:
	DrawAble() = default;
	DrawAble(const DrawAble &) = default;
	DrawAble(DrawAble &&) = default;

	DrawAble &operator=(const DrawAble &) = default;
	DrawAble &operator=(DrawAble &&) = default;

	virtual ~DrawAble() = default;
	
	virtual void draw(Shader &shader) = 0;
};

#endif // !DRAWABLE_H
