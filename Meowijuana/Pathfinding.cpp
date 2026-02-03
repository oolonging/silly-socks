#include "AEEngine.h"
#include "Entity.hpp"


namespace Path {

	Vect randPoint() {

		float direction = (AERandFloat() * 360);
		float distance = (AERandFloat() * 200);

		// sketchy ahh formula i saw
		return Vect{ distance * AECos(direction), distance * AESin(direction) };
	}



}