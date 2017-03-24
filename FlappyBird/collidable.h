#ifndef COLLIABLE_H
#define COLLIABLE_H

#include <memory>
#include <list>
#include "geometry.h"


namespace utility {
	// ǰ������
	class CollisionWorld;

	/* Proxy (������ײ���������Ƴ�) 
	\  ���ڱ�ʾ����ײ�����壬��һ��������
	*/
	class Collidable {
		friend class CollisionWorld;
	public:
		class RealCollidable;

		enum class BoxType {
			RETENCGEL
		};
		   

		template<typename ...Args>
		Collidable(BoxType t, Args&& ... args) {
			this->itor_ = CollisionWorld::instance()->add(
				std::make_shared<RealCollidable>(t, std::forward<Args>(args)...));
		}


		~Collidable();

		friend void swap(Collidable &lhs, Collidable &rhs) {
			// Enable ADL
			using std::swap;

			swap(lhs.itor_, rhs.itor_);
		}


		Collidable(const Collidable &c);


		Collidable(Collidable &&) = default;

		// Move/Copy assignment operator
		// Copy-and-swap
		Collidable& operator=(Collidable c);


		bool collisionDetect(const Collidable &object) { return (*this->itor_)->collisionDetect(static_cast<RealCollidable>(object)); }

		glm::vec3 &position() noexcept { return (*this->itor_)->position(); }

		operator RealCollidable() { return *(*this->itor_); }
		operator const RealCollidable() const { return *(*this->itor_); }


		// Real Subject
		class RealCollidable {
		public:
			//template<typename BoxT, typename ...Args>
			//RealCollidable(BoxT t, Args&& ... args) = delete;

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

			//*****************************************************************************************
			auto pBox() { return pBox_;}
		private:
			std::shared_ptr<Geometry> pBox_;
		};
		//****************************************************************************
		auto pBox() { return itor_->get()->pBox(); }

	private:
		Collidable(const RealCollidable &realCollidable);

		Collidable(RealCollidable &&realCollidable);


		// using ItorT = std::list<std::shared_ptr<Collidable::RealCollidable>>::iterator;
		// ��ײ������ײ�����б��б�ʾ��ǰRealCollidable�ĵ�����
		std::list<std::shared_ptr<utility::Collidable::RealCollidable>>::iterator itor_;

	};

}




#endif
