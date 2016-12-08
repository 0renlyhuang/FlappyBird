#ifndef COLLISION_H
#define COLLISION_H

#include <memory>
#include <list>
#include <iostream>
#include <utility>


namespace utility {
	template<typename T>
	using PointT = std::pair<T, T>;

	template<typename T>
	using RangeT = std::pair<T, T>;

	class Collidable;

	// using PCollidableT = std::shared_ptr<Collidable>;

	class CollisionException {};

	// Singleton
	// 碰撞世界
	class CollisionWorld {
	public:
		using PCollidableT = std::shared_ptr<Collidable>;

		// 开启碰撞检测
		static void setUp() noexcept {
			isSetUp = true;
		}

		// 将碰撞物体加入碰撞世界
		void add(const Collidable &object) {
			if (isSetUp) {
				this->getObjList().emplace_back(std::make_shared<Collidable>(object));
			}
			else {
				std::cerr << "ERROR: in " << __FILE__
					<< " line " << __LINE__
					<< ": CollisionWorld is not set up yet." << std::endl;
				throw CollisionException();
			}
		}


		// 将碰撞物体加入碰撞世界
		void add(Collidable &&object) {
			if (isSetUp) {
				this->getObjList().emplace_back(std::make_shared<Collidable>(object));
			}
			else {
				std::cerr << "ERROR: in " << __FILE__
					<< " line " << __LINE__
					<< ": CollisionWorld is not set up yet." << std::endl;
				throw CollisionException();
			}
		}


		// 将碰撞物体加入碰撞世界
		void add(const PCollidableT &pobject) {
			if (isSetUp) {
				this->getObjList().emplace_back(pobject);
			}
			else {
				std::cerr << "ERROR: in " << __FILE__
					<< " line " << __LINE__
					<< ": CollisionWorld is not set up yet." << std::endl;
				throw CollisionException();
			}
		}


		// 将碰撞物体加入碰撞世界
		void add(PCollidableT &&pobject) {
			if (isSetUp) {
				this->getObjList().emplace_back(pobject);
			}
			else {
				std::cerr << "ERROR: in " << __FILE__
					<< " line " << __LINE__
					<< ": CollisionWorld is not set up yet." << std::endl;
				throw CollisionException();
			}
		}


		// 获取碰撞世界的碰撞物体列表
		std::list<PCollidableT>& getObjList() {
			if (isSetUp) {
				static std::list<PCollidableT> objects;
				static char processOnce = (CollisionWorld::pObjects_ = &objects, ' ');
				return *CollisionWorld::pObjects_;
			}
			else {
				std::cerr << "ERROR: in " << __FILE__
					<< " line " << __LINE__
					<< ": CollisionWorld is not set up yet." << std::endl;
				throw CollisionException();
			}
		}


		// 获取碰撞世界的实例
		static std::shared_ptr<CollisionWorld> instance() {
			if (pCollisionWorld)
				return pCollisionWorld;

			static char processOnce = (pCollisionWorld = std::shared_ptr<CollisionWorld>(new CollisionWorld()), ' ');
			return pCollisionWorld;
		}


		CollisionWorld(const CollisionWorld &) = delete;
		CollisionWorld(CollisionWorld &&) = delete;
		CollisionWorld& operator=(const CollisionWorld &) = delete;
		CollisionWorld& operator=(CollisionWorld &&) = delete;
		~CollisionWorld() = default;


	private:
		CollisionWorld() = default;

		static std::shared_ptr<CollisionWorld> pCollisionWorld;
		static std::list<PCollidableT> *pObjects_;
		static bool isSetUp;
	};

	std::shared_ptr<CollisionWorld> CollisionWorld::pCollisionWorld(nullptr);
	std::list<std::shared_ptr<Collidable>> *CollisionWorld::pObjects_;
	bool CollisionWorld::isSetUp = false;



	class Rectangle;
	bool CollideDetect(const Rectangle &r1, const Rectangle &r2);
	// Double dispatch
	class Geometry {
	public:
		virtual RangeT<float> projectToX() const = 0;
		virtual RangeT<float> projectToY() const = 0;

		virtual bool CollideDetectWith(const Geometry &geometry) const = 0;
		virtual bool CollideDetectWith(const Rectangle &rectangle) const = 0;

		glm::vec3 &position() noexcept {return this->position_; }

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






	// 矩形间的碰撞检测
	bool CollideDetect(const Rectangle &r1, const Rectangle &r2) {
		// 判断X上的投影是否有重叠
		bool isXOverlap =
			!(r1.projectToX().second < r2.projectToX().first ||
				r1.projectToX().first > r2.projectToX().second);
		if (!isXOverlap)
			return false;

		// 判断Y上的投影是否有重叠
		bool isYOverlap =
			!(r1.projectToY().second < r2.projectToY().first ||
				r1.projectToY().first > r2.projectToY().second);

		return isYOverlap;
	}




	// Proxy
	class Collidable {
	public:
		class RealCollidable;

		enum class BoxType {
			RETENCGEL
		};

		template<typename BoxT, typename ...Args>
		Collidable(BoxT t, Args&& ... args) = delete;

		template<typename ...Args>
		Collidable(BoxType t, Args&& ... args) {
			prealCollidable_ = std::make_shared<RealCollidable>(t, std::forward<Args>(args)...);
			CollisionWorld::instance()->add(Collidable(*this->prealCollidable_));
			this->itor_ = CollisionWorld::instance()->getObjList().cend();
			// --this->itor_;
		}


		~Collidable() {
			CollisionWorld::instance()->getObjList().erase(this->itor_);
		}

		friend void swap(Collidable &lhs, Collidable &rhs) {
			// Enable ADL
			using std::swap;

			swap(lhs.prealCollidable_, rhs.prealCollidable_);
			swap(lhs.itor_, rhs.itor_);
		}

		Collidable(const Collidable &c) {
			this->prealCollidable_ = c.prealCollidable_;
			// CollisionWorld::instance()->add(*this->prealCollidable_);
			this->itor_ = CollisionWorld::instance()->getObjList().cend();
			auto a = CollisionWorld::instance()->getObjList();
			auto size = a.size();
			--this->itor_;
		}


		Collidable(Collidable &&) = default;

		// Move/Copy assignment operator
		// Copy-and-swap
		Collidable& operator=(Collidable c) {
			swap(*this, c);
			return *this;
		}


		bool collisionDetect(const Collidable &object) { return prealCollidable_->collisionDetect(object); }

		auto &position() noexcept { return this->prealCollidable_->position(); }


	private:
		operator RealCollidable() const { return *prealCollidable_; }

		Collidable(const RealCollidable &realCollidable) {
			prealCollidable_ = std::make_shared<RealCollidable>(realCollidable);
		}

		Collidable(RealCollidable &&realCollidable) {
			prealCollidable_ = std::make_shared<RealCollidable>(realCollidable);
		}

		std::list<PCollidableT>::const_iterator itor_;
		std::shared_ptr<RealCollidable> prealCollidable_;

		// Real Subject
		class RealCollidable {
		public:
			template<typename BoxT, typename ...Args>
			RealCollidable(BoxT t, Args&& ... args) = delete;

			template<typename ...Args>
			RealCollidable(BoxType t, Args&& ... args) {
				switch (t) {
				case BoxType::RETENCGEL:
					this->pBox_ = std::make_shared<Rectangle>(std::forward<Args>(args)...);
					break;

				default:
					std::cerr << "ERROR: in " << __FILE__
						<< " line " << __LINE__
						<< ": wrong Collidable box type." << std::endl;
					throw CollisionException();
					break;
				}
			}

			RealCollidable(const RealCollidable &) = default;
			RealCollidable(RealCollidable &&) = default;
			RealCollidable& operator=(const RealCollidable &) = default;
			RealCollidable& operator=(RealCollidable &&) = default;
			virtual ~RealCollidable() = default;


			bool collisionDetect(const RealCollidable &object) const {
				return this->pBox_->CollideDetectWith(*object.pBox_);
			}

			glm::vec3 &position() noexcept { return this->pBox_->position(); }

		private:
			std::shared_ptr<Geometry> pBox_;
		};


	};
}



#endif // !COLLISION_H

