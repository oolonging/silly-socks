#ifndef MATH_HPP
#define MATH_HPP

namespace Math {
	int clampInt(int value, int min, int max);
	void clampInt(int& value, int min, int max);
	float clampFloat(float value, float min, float max);
	float distance(float x1, float y1, float x2, float y2);
}

#endif // MATH_HPP
