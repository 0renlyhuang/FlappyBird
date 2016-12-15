#include "collidable.h"
#include "collisionWorld.h"


namespace utility {

	Collidable::~Collidable() {
		CollisionWorld::instance()->getObjList().erase(this->itor_);
	}


	Collidable::Collidable(const Collidable &c) {
		this->itor_ = CollisionWorld::instance()->add(c);
	}


	Collidable::Collidable(const RealCollidable &realCollidable) {
		this->itor_ = CollisionWorld::instance()->add(
			std::make_shared<RealCollidable>(realCollidable));
	}


	Collidable::Collidable(RealCollidable &&realCollidable) {
		this->itor_ = CollisionWorld::instance()->add(
			std::make_shared<RealCollidable>(realCollidable));
	}



	// Move/Copy assignment operator
	// Copy-and-swap
	Collidable& Collidable::operator=(Collidable c) {
		swap(*this, c);
		return *this;
	}

}