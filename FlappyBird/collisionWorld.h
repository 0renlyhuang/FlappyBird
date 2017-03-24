#ifndef COLLISIONWORLD_H
#define COLLISIONWORLD_H

#include <memory>
#include <list>
#include <iostream>
#include <utility>
#include "glm\glm.hpp"
#include "collidable.h"


namespace utility {
	class CollisionException {};

	// Singleton
	// 表示碰撞世界，用于维护所有可碰撞物体
	class CollisionWorld {
	public:
		// 开启碰撞检测
		static void setUp() noexcept {
			isSetUp = true;
		}

		// 将碰撞物体加入碰撞世界
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(const Collidable::RealCollidable &object);


		// 将碰撞物体加入碰撞世界
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(Collidable::RealCollidable &&object);


		// 将碰撞物体加入碰撞世界
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(const std::shared_ptr<Collidable::RealCollidable> &pobject);


		// 将碰撞物体加入碰撞世界
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(std::shared_ptr<Collidable::RealCollidable> &&pobject);


		// 将碰撞物体加入碰撞世界
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(const Collidable &object);

		// 将碰撞物体加入碰撞世界
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(Collidable &&object);


		// 获取碰撞世界的碰撞物体列表
		std::list<std::shared_ptr<Collidable::RealCollidable>>& 
			getObjList();


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
		static std::list<std::shared_ptr<Collidable::RealCollidable>> *pObjects_;
		static bool isSetUp;
	};

}




#endif // !COLLISIONWORLD_H

