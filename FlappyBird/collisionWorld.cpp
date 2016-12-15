#include "collisionWorld.h"


namespace utility {
	std::shared_ptr<CollisionWorld> CollisionWorld::pCollisionWorld(nullptr);
	std::list<std::shared_ptr<Collidable::RealCollidable>>* CollisionWorld::pObjects_;
	bool  CollisionWorld::isSetUp = false;


	// 将碰撞物体加入碰撞世界
	std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
		CollisionWorld::add(const Collidable::RealCollidable &object) {
		if (isSetUp) {
			this->getObjList().emplace_back(std::make_shared<Collidable::RealCollidable>(object));
			std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
				itor = getObjList().end();
			return --itor;
		}
		else {
			std::cerr << "ERROR: in " << __FILE__
				<< " line " << __LINE__
				<< ": CollisionWorld is not set up yet." << std::endl;
			throw CollisionException();
		}
	}


	// 将碰撞物体加入碰撞世界
	std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
		CollisionWorld::add(Collidable::RealCollidable &&object) {
		if (isSetUp) {
			this->getObjList().emplace_back(std::make_shared<Collidable::RealCollidable>(object));
			std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
				itor = getObjList().end();
			return --itor;
		}
		else {
			std::cerr << "ERROR: in " << __FILE__
				<< " line " << __LINE__
				<< ": CollisionWorld is not set up yet." << std::endl;
			throw CollisionException();
		}
	}


	// 将碰撞物体加入碰撞世界
	std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
		CollisionWorld::add(const std::shared_ptr<Collidable::RealCollidable> &pobject) {
		if (isSetUp) {
			this->getObjList().emplace_back(pobject);
			std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
				itor = getObjList().end();
			return --itor;
		}
		else {
			std::cerr << "ERROR: in " << __FILE__
				<< " line " << __LINE__
				<< ": CollisionWorld is not set up yet." << std::endl;
			throw CollisionException();
		}
	}


	// 将碰撞物体加入碰撞世界
	std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
		CollisionWorld::add(std::shared_ptr<Collidable::RealCollidable> &&pobject) {
		if (isSetUp) {
			this->getObjList().emplace_back(pobject);
			std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
				itor = getObjList().end();
			return --itor;
		}
		else {
			std::cerr << "ERROR: in " << __FILE__
				<< " line " << __LINE__
				<< ": CollisionWorld is not set up yet." << std::endl;
			throw CollisionException();
		}
	}


	// 将碰撞物体加入碰撞世界
	std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
		CollisionWorld::add(const Collidable &object) {
		return this->add(static_cast<const Collidable::RealCollidable&>(object));
	}


	// 将碰撞物体加入碰撞世界
	std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
		CollisionWorld::add(Collidable &&object) {
		return this->add(static_cast<Collidable::RealCollidable&&>(object));
	}


	// 获取碰撞世界的碰撞物体列表
	std::list<std::shared_ptr<Collidable::RealCollidable>>& 
		CollisionWorld::getObjList() {
		if (isSetUp) {
			static std::list<std::shared_ptr<Collidable::RealCollidable>> objects;
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

}