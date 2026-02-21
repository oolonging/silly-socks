#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "AEEngine.h"

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

	// Create colors
	Color createColorRGB(float red, float green, float blue, float alpha = 255.0f);
	Color createColorHex(int hexValue);

	// Fill
	void fill(float red, float green, float blue, float alpha = 255.0f);
	void fill(Color color);
	void noFill(void);

	// Stroke
	void stroke(float red, float green, float blue, float alpha = 255.0f);
	void stroke(Color color);
	void noStroke(void);

	// Text fill
	void textFill(float red, float green, float blue, float alpha = 255.0f);
	void textFill(Color color);

	// Stroke weight
	void strokeWeight(int value);

	// background functions
	void background(float red, float green, float blue);
	void background(Color color);

	// Preset colors
	namespace Preset {
		extern const Color Black;
		extern const Color White;
		extern const Color Red;
		extern const Color Green;
		extern const Color Blue;
		extern const Color Yellow;
		extern const Color Cyan;
		extern const Color Magenta;
		extern const Color Orange;
		extern const Color Purple;
		extern const Color Gray;
		extern const Color DarkGray;
		extern const Color LightGray;
		extern const Color Transparent;
	}

}

namespace Shapes {
	// TODO: missing
	// Removed the enums for now since they werent doign much good
	// removed the triangle function since it wasnt working so ill add it back when it is
	// removed square and circle functions cause they were pretty useless. Might add it back if i see the need for them when I fix strokes

	// TODO: temporary added back the structs to avoid conflicts with other files
	typedef struct Point {
		float x;
		float y;

		Point(float x = 0.0f, float y = 0.0f)
			: x(x), y(y) {
		}
	} Point;

	typedef struct Quad {
		Shapes::Point position;
		float width;
		float height;

		Quad(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f)
			: position(x, y), width(width), height(height) {
		}
	} Quad;

	typedef struct Circle {
		Point position;
		float radius;

		Circle(float x = 0.0f, float y = 0.0f, float radius = 0.0f)
			: position(x, y), radius(radius) {
		}
	} Circle;

	enum SHAPE_MODE {
		CENTER = 15,
		CORNER = 16
	};

	bool init(void); // Initialize shapes
	void exit(void); // Destroy shape objects

	// TODO: figure out what to do with square and circle
	void rectAdvanced(float x, float y, float width, float height, float rotation, SHAPE_MODE drawMode = CORNER);
	void rect(float x, float y, float width, float height, SHAPE_MODE drawMode = CORNER);
	void square(float x, float y, float size, SHAPE_MODE drawMode = CENTER);

	void ellipseAdvanced(float x, float y, float width, float height, float rotation, SHAPE_MODE drawMode = CENTER);
	void ellipse(float x, float y, float width, float height, SHAPE_MODE drawMode = CENTER);
	void circle(float x, float y, float size, SHAPE_MODE drawMode = CENTER);
}

namespace Graphics {
	// image function
	void image(float x, float y, float width, float height, AEGfxTexture* pTex, Shapes::SHAPE_MODE drawMode = Shapes::CORNER);
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

	// Font management
	bool createFont(const char* fontPath, int fontSize, const char* fontName = "default");
	void setFont(const char* fontName);
	void destroyFont(const char* fontName);
	void exit(void); // Clean up all font related items
	// TODO: Maybe have an init that loads multiple fonts and sets a default as well
	// Basically this just needs to be cleaned up more too, add a unified init and exit for graphics

	// Render text
	// TODO: test if string type works here
	void text(const char* str, float x, float y);

	// Text styling
	void textSize(float size);
	void textAlign(TEXT_ALIGN_HORIZONTAL horizontal, TEXT_ALIGN_VERTICAL vertical);
}

#endif // GRAPHICS_HPP
