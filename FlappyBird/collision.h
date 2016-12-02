#ifndef COLLISION_H
#define COLLISION_H

#include <memory>
#include <list>
#include <iostream>
#include <utility>


namespace utility {
	class Collidable;

	using PCollidableT = std::shared_ptr<Collidable>;

	class CollisionException {};

	// Singleton
	// 碰撞世界
	class CollisionWorld {
	public:
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

	static std::shared_ptr<CollisionWorld> pCollisionWorld(nullptr);
	std::list<std::shared_ptr<Collidable>> *CollisionWorld::pObjects_;
	bool CollisionWorld::isSetUp = false;



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



	class Rectangle;
	// Double dispatch
	class Geometry {
	protected:
		template<typename T>
		using RangeT = std::pair<T, T>;

	public:
		virtual RangeT<float> projectToX() const = 0;
		virtual RangeT<float> projectToY() const = 0;

		virtual bool CollideDetectWith(const Geometry &geometry) const = 0;
		virtual bool CollideDetectWith(const Rectangle &rectangle) const = 0;


		Geometry() = default;
		Geometry(const Geometry &) = default;
		Geometry(Geometry &&) = default;
		Geometry& operator=(const Geometry &) = default;
		Geometry& operator=(Geometry &&) = default;
		virtual ~Geometry() = default;
	};



	class Rectangle : public Geometry {
	protected:
		template<typename T>
		using PointT = std::pair<T, T>;
	public:
		Rectangle(const PointT<float> &topLeft, const PointT<float> &bottomRight)
			: topLeft_(topLeft), bottomRight_(bottomRight) {}


		Rectangle(PointT<float> &&topLeft, PointT<float> &&bottomRight)
			: topLeft_(topLeft), bottomRight_(bottomRight) {}


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
			prealCollidable = std::make_shared<RealCollidable>(t, std::forward<Args>(args)...);
			CollisionWorld::instance()->add(Collidable(*prealCollidable));
			this->itor_ = CollisionWorld::instance()->getObjList().cend() - 1;
		}


		~Collidable() {
			CollisionWorld::instance()->getObjList().erase(this->itor_);
		}



		Collidable(const Collidable &) = default;
		Collidable(Collidable &&) = default;
		Collidable& operator=(const Collidable &) = default;
		Collidable& operator=(Collidable &&) = default;


		bool collisionDetect(const Collidable &object) { return prealCollidable_->collisionDetect(object); }


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
					pBox = std::make_shared<Rectangle>(std::forward<Args>(args)...);
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

		private:
			std::shared_ptr<Geometry> pBox_;
		};


	};
}



#endif // !COLLISION_H

