#include "pch.h"
#include "Pathfinding.hpp"

namespace Path {
	Vect randPoint() {
		float direction = static_cast<float>(AERandFloat() * 360.0f);
		float distance = static_cast<float>(AERandFloat() * 200.0f);

		// move in a random direction for a random distance
		return Vect{ distance * AECos(direction), distance * AESin(direction) };
	}
}
