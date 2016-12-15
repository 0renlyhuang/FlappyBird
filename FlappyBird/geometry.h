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


	// Double dispatch
	class Geometry {
	public:

		virtual RangeT<float> projectToX() const = 0;
		virtual RangeT<float> projectToY() const = 0;

		virtual bool CollideDetectWith(const Geometry &geometry) const = 0;
		virtual bool CollideDetectWith(const Rectangle &rectangle) const = 0;

		glm::vec3 &position() noexcept { return this->position_; }

		Geometry() = default;
		Geometry(const Geometry &) = default;
		Geometry(Geometry &&) = default;
		Geometry& operator=(const Geometry &) = default;
		Geometry& operator=(Geometry &&) = default;
		virtual ~Geometry() = default;

	protected:
		glm::vec3 position_;
	};



	class Rectangle : public Geometry {
	public:
		Rectangle(const PointT<float> &topLeft, const PointT<float> &bottomRight)
			: topLeft_(topLeft), bottomRight_(bottomRight) {
			this->position_ = {
				(this->topLeft_.first + this->bottomRight_.first) * 0.5f,
				(this->topLeft_.second + this->bottomRight_.second) * 0.5f,
				0.0f
			};
		}


		Rectangle(PointT<float> &&topLeft, PointT<float> &&bottomRight)
			: topLeft_(topLeft), bottomRight_(bottomRight) {
			this->position_ = {
				(this->topLeft_.first + this->bottomRight_.first) * 0.5f,
				(this->topLeft_.second + this->bottomRight_.second) * 0.5f,
				0.0f
			};
		}


		Rectangle(const Rectangle &) = default;
		Rectangle(Rectangle &&) = default;
		Rectangle& operator=(const Rectangle &) = default;
		Rectangle& operator=(Rectangle &&) = default;
		virtual ~Rectangle() = default;


		// 投影到X轴
		RangeT<float> projectToX() const override {
			return
				this->topLeft_.first <= this->bottomRight_.first ?
				std::make_pair(this->topLeft_.first, this->bottomRight_.first) :
				std::make_pair(this->bottomRight_.first, this->topLeft_.first);
		}


		// 投影到Y轴
		RangeT<float> projectToY() const override {
			return
				this->bottomRight_.second <= this->topLeft_.second ?
				std::make_pair(this->bottomRight_.second, this->topLeft_.second) :
				std::make_pair(this->topLeft_.second, this->bottomRight_.second);
		}


		bool CollideDetectWith(const Geometry &geometry) const override {
			return geometry.CollideDetectWith(*this);
		}


		bool CollideDetectWith(const Rectangle &rectangle) const override {
			return CollideDetect(*this, rectangle);
		}


	private:
		PointT<float> topLeft_;
		PointT<float> bottomRight_;
	};

}

#endif // !GEOMETRY_H
