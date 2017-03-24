#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <utility>
#include "glm\glm.hpp"


namespace utility {
	template<typename T>
	using PointT = std::pair<T, T>;

	template<typename T>
	using RangeT = std::pair<T, T>;


	class Rectangle;


	bool CollideDetect(const Rectangle &r1, const Rectangle &r2);


	// 表示几何，用于实现由几何类型确定的碰撞检测算法
	// 观察者模式（双分发）
	// Double dispatch
	class Geometry {
	public:

		virtual RangeT<float> projectToX() const = 0;
		virtual RangeT<float> projectToY() const = 0;

		virtual bool CollideDetectWith(const Geometry &geometry) const = 0;
		virtual bool CollideDetectWith(const Rectangle &rectangle) const = 0;

		glm::vec3 &position() noexcept { return this->position_; }


		Geometry(const glm::vec3 &pos) : position_(pos) {};
		Geometry(glm::vec3 &&pos) : position_(pos) {};
		Geometry(const Geometry &) = default;
		Geometry(Geometry &&) = default;
		Geometry& operator=(const Geometry &) = default;
		Geometry& operator=(Geometry &&) = default;
		virtual ~Geometry() = default;

	protected:
		virtual PointT<float> topLeft() const noexcept = 0;
		virtual PointT<float> bottomRight() const noexcept = 0;
		glm::vec3 position() const noexcept { return this->position_; }
		glm::vec3 position_;
	};



	class Rectangle : public Geometry {
	public:
		Rectangle(const glm::vec3 &pos, float width, float height)
			: Geometry(pos), width_(width), height_(height) { }


		Rectangle(glm::vec3 &&pos, float width, float height)
			: Geometry(pos), width_(width), height_(height) { }


		Rectangle(const Rectangle &) = default;
		Rectangle(Rectangle &&) = default;
		Rectangle& operator=(const Rectangle &) = default;
		Rectangle& operator=(Rectangle &&) = default;
		virtual ~Rectangle() = default;


		// 投影到X轴
		RangeT<float> projectToX() const override {
			return
				this->topLeft().first <= this->bottomRight().first ?
				std::make_pair(this->topLeft().first, this->bottomRight().first) :
				std::make_pair(this->bottomRight().first, this->topLeft().first);
		}


		// 投影到Y轴
		RangeT<float> projectToY() const override {
			return
				this->bottomRight().second <= this->topLeft().second ?
				std::make_pair(this->bottomRight().second, this->topLeft().second) :
				std::make_pair(this->topLeft().second, this->bottomRight().second);
		}


		bool CollideDetectWith(const Geometry &geometry) const override {
			return geometry.CollideDetectWith(*this);
		}


		bool CollideDetectWith(const Rectangle &rectangle) const override {
			return CollideDetect(*this, rectangle);
		}


	private:
		PointT<float> topLeft() const noexcept {
			return PointT<float>(this->position().x - 0.5f * this->width_,
				this->position().y + 0.5f * this->height_);
		}

		PointT<float> bottomRight() const noexcept {
			return PointT<float>(this->position().x + 0.5f * this->width_,
				this->position().y - 0.5f * this->height_);
		}

		float width_;
		float height_;
	};

}

#endif // !GEOMETRY_H
