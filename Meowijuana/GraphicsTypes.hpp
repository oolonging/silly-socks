#ifndef GRAPHICS_TYPES_HPP
#define GRAPHICS_TYPES_HPP

namespace Color {
	struct Color {
		float red;
		float green;
		float blue;
		float alpha;

		// Ctor with default values
		Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 255.0f) :
			red(red), green(green), blue(blue), alpha(alpha) {
		}
	};
}

namespace Shapes {
	typedef struct Point {
		float x;
		float y;
	} Point;

	typedef struct Quad {
		Shapes::Point position;
		float width;
		float height;

		// Ctors
		Quad() : position({ 0.0f, 0.0f }), width(0.0f), height(0.0f) {}
		Quad(Shapes::Point position, float width, float height) : position(position), width(width), height(height) {}
		Quad(float x, float y, float width, float height) : position({ x, y }), width(width), height(height) {}
	} Quad;

	typedef struct Circle {
		Point position;
		float radius;
	} Circle;

	enum SHAPE_MODE {
		CENTER = 15,
		CORNER = 16
	};
}

namespace Text {
	enum TEXT_ALIGN_HORIZONTAL {
		LEFT = 0,
		CENTER_H = 1,
		RIGHT = 2,
	};

	enum TEXT_ALIGN_VERTICAL {
		TOP = 0,
		CENTER_V = 1,
		BOTTOM = 2,
		BASELINE = 3,
	};
}

#endif // GRAPHICS_TYPES_HPP
