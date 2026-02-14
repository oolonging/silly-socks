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

	// Color creation functions
	CL_Color CL_Color_Create(int red, int green, int blue, int alpha = 255);
	CL_Color CL_Color_Create_Hex(int hexValue);
	CL_Color CL_Color_Create_HSL(int hue, int saturation, int lightness, int alpha = 255);

	// Fill and stroke functions
	void fill(int red, int green, int blue, int alpha = 255);
	void fill(CL_Color color);

	void textFill(int red, int green, int blue, int alpha = 255);
	
	void stroke(int red, int green, int blue, int alpha = 255);
	void stroke(CL_Color color);

	void strokeWeight(float weight);

	void noFill(void);
	void noStroke(void);

	// Background function
	void background(int red, int green, int blue);
	void background(CL_Color color);
}

namespace Shapes {
	typedef struct {
		float x;
		float y;
	} Point;

	typedef struct {
		Shapes::Point position;
		float width;
		float height;
	} Quad;

	typedef struct {
		Point position;
		float radius;
	} Circle;

	enum SHAPE_MODE {
		CENTER = 15,
		CORNER = 16,
	};

	bool init(void);
	void exit(void);

	void rect(float x, float y, float width, float height, SHAPE_MODE mode = CORNER);
	void rect(Quad quad, SHAPE_MODE mode = CORNER);

	void ellipse(float x, float y, float width, float height, SHAPE_MODE mode = CENTER);
	void ellipse(Circle circle, SHAPE_MODE mode = CENTER);

	void square(float x, float y, float size, SHAPE_MODE mode = CENTER);
	void circle(float x, float y, float size, SHAPE_MODE mode = CENTER);
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
}

namespace Graphics {
	void image(float x, float y, float width, float height, AEGfxTexture* texture, Shapes::SHAPE_MODE mode = Shapes::CORNER);
	void image(Shapes::Quad quad, AEGfxTexture* texture, Shapes::SHAPE_MODE mode);
}

namespace Text {
	// Still WIP, nothing works yet

	enum TEXT_ALIGN {
		ALIGN_LEFT = 25,
		ALIGN_CENTER = 26,
		ALIGN_RIGHT = 27,
	};

	void setFont(char const* fontPath, int fontSize);
	void text(const char* pText, float x, float y, TEXT_ALIGN align = ALIGN_LEFT);
	void unloadFont(void);
}

#endif // GRAPHICS_HPP
