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
	// ��ʾ��ײ���磬����ά�����п���ײ����
	class CollisionWorld {
	public:
		// ������ײ���
		static void setUp() noexcept {
			isSetUp = true;
		}

		// ����ײ���������ײ����
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(const Collidable::RealCollidable &object);


		// ����ײ���������ײ����
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(Collidable::RealCollidable &&object);


		// ����ײ���������ײ����
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(const std::shared_ptr<Collidable::RealCollidable> &pobject);


		// ����ײ���������ײ����
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(std::shared_ptr<Collidable::RealCollidable> &&pobject);


		// ����ײ���������ײ����
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(const Collidable &object);

		// ����ײ���������ײ����
		std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator
			add(Collidable &&object);


		// ��ȡ��ײ�������ײ�����б�
		std::list<std::shared_ptr<Collidable::RealCollidable>>& 
			getObjList();


		// ��ȡ��ײ�����ʵ��
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

