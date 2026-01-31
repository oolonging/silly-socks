#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "AEEngine.h"

namespace Color {
	typedef struct CL_Color {
		int red;
		int green;
		int blue;
		int alpha;
	} CL_Color;

	static CL_Color gCurrentFillColor = { 255, 255, 255, 255 };
	static CL_Color gCurrentStrokeColor = { 0, 0, 0, 255 };

	CL_Color CL_Color_Create(int red, int green, int blue, int alpha = 255);
	CL_Color CL_Color_Create_Hex(int hexValue);
	CL_Color CL_Color_Create_HSL(int hue, int saturation, int lightness, int alpha = 255);

	void fill(int red, int green, int blue, int alpha = 255);

	void stroke(int red, int green, int blue, int alpha = 255);
	void noFill(void);
	void noStroke(void);
	void background(CL_Color color);
}

namespace Shapes {
	typedef struct Point {
		float x;
		float y;
	};

	typedef struct Quad {
		Shapes::Point position;
		float width;
		float height;
	};

	typedef struct Circle {
		Point position;
		float radius;
	};

	enum SHAPE_MODE {
		CENTER = 15,
		CORNER = 16,
	};

	bool init(void);
	void exit(void);

	void rect(float x, float y, float width, float height, SHAPE_MODE mode = CORNER);
	void ellipse(float x, float y, float width, float height, SHAPE_MODE mode = CORNER);
	void square(float x, float y, float size, SHAPE_MODE mode = CENTER);
	void circle(float x, float y, float size, SHAPE_MODE mode = CENTER);
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
}

#endif // GRAPHICS_HPP
