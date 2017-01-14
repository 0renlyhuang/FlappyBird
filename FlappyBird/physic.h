#ifndef PHYSIC_H
#define PHYSIC_H


namespace utility {
	class Motion {
	public:
		// s = v0 * t - 1 / 2 * a * t * t
		static float displacement(const float v0, const float t) noexcept {
			return v0 > 0.0f ?
				static_cast<float>(v0 * t - 0.5f * aUp * t * t) :
				static_cast<float>(v0 * t - 0.5f * aDown * t * t);
		}

		// v = v0 - a * t 
		static float velocity(const float v0, const float t) noexcept {
			return v0 > 0.0f ?
				static_cast<float>(v0 - aUp * t) :
				static_cast<float>(v0 - aDown * t);
		}

		static const float aUp;
		static const float aDown;

		Motion() = delete;
		Motion(const Motion &) = delete;
		Motion(Motion &&) = delete;
		Motion& operator=(const Motion &) = delete;
		Motion& operator=(Motion &&) = delete;
		~Motion() = delete;
	};
}

const float utility::Motion::aUp = 160000.0f;
const float utility::Motion::aDown = /*0.0001f;*/150000.0f;



#endif // !PHYSIC_H

