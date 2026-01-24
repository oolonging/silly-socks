#include "Math.hpp"
#include <cmath>

namespace Math {

	int clampInt(int value, int min, int max) {
		if (value >= min && value <= max) return value;
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}

	void clampInt(int& value, int min, int max) {
		if (value < min) value = min;
		if (value > max) value = max;
	}

	float clampFloat(float value, float min, float max) {
		if (value >= min && value <= max) return value;
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}

	float distance(float x1, float y1, float x2, float y2) {
		float dx = x2 - x1;
		float dy = y2 - y1;
		return std::sqrtf(dx * dx + dy * dy);
	}

}