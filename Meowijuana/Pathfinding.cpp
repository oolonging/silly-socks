#include "pch.h"
#include "Entity.hpp"


namespace Path {

	Vect randPoint() {

		float direction = (AERandFloat() * 360);
		float distance = (AERandFloat() * 200);

		// move in a random direction for a random distance
		return Vect{ distance * AECos(direction), distance * AESin(direction) };
	}



}

