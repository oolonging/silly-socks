#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "pch.h"
#include "GraphicsTypes.hpp"

namespace Color {
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

		// Game color palette (UI Elements)
		extern const Color defaultStylePrimary;
		extern const Color defaultStyleSecondary;
		extern const Color defaultStyleStroke;
		extern const Color defaultTextStylePrimary;
		extern const Color defaultTextStyleSecondary;
	}
}

namespace Shapes {
	bool init(void); // Initialize shapes
	void exit(void); // Destroy shape objects

	// TODO: figure out what to do with square and circle
	void rectAdvanced(float x, float y, float width, float height, float rotation, SHAPE_MODE drawMode = CORNER);
	void rect(float x, float y, float width, float height, SHAPE_MODE drawMode = CORNER);
	void square(float x, float y, float size, SHAPE_MODE drawMode = CENTER);

	void ellipseAdvanced(float x, float y, float width, float height, float rotation, SHAPE_MODE drawMode = CENTER);
	void ellipse(float x, float y, float width, float height, SHAPE_MODE drawMode = CENTER);
	void circle(float x, float y, float size, SHAPE_MODE drawMode = CENTER);

	// Triangle
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void triangle(Point p1, Point p2, Point p3);

	// Line
	void line(float x1, float y1, float x2, float y2);
	void line(Point p1, Point p2);

	// Debug shapes
	void debugRect(float x, float y, float width, float height, SHAPE_MODE drawMode);

	// Other novel shapes since I have time
	void heart(float x, float y, float size, SHAPE_MODE drawMode = CENTER);
}

namespace Graphics {
	// image function
	void image(float x, float y, float width, float height, AEGfxTexture* pTex, Shapes::SHAPE_MODE drawMode = Shapes::CORNER);
	void image(Shapes::Quad quad, AEGfxTexture* pTex, Shapes::SHAPE_MODE drawMode = Shapes::CORNER);
}

namespace Text {
	// Font management
	bool createFont(const char* fontPath, int fontSize, const char* fontName = "default");
	void setFont(std::string fontName);
	void destroyFont(const char* fontName);
	void exit(void); // Clean up all font related items
	// TODO: Maybe have an init that loads multiple fonts and sets a default as well
	// Basically this just needs to be cleaned up more too, add a unified init and exit for graphics

	// Render text

	// overloaded text to manually specify alignment
	void text(const char* str, float x, float y, TEXT_ALIGN_HORIZONTAL horizontal, TEXT_ALIGN_VERTICAL vertical);
	
	// TODO: test if string type works here
	void text(const char* str, float x, float y);


	// Text styling
	void textSize(float size);
	void textAlign(TEXT_ALIGN_HORIZONTAL horizontal, TEXT_ALIGN_VERTICAL vertical);
}

namespace Animations {

	struct Indicator {
		float x = 0.0f;
		float y = 0.0f;

		float offset = 0.0f;
		float dir = 1.0f;
		bool active = true;
	};

	// Triangle, with and without offset
	void updateIndicator(Indicator& ind);
	void drawIndicator(Indicator& ind);
	void drawIndicator(float x, float y, float offset);
	void drawCoolerIndicator(float x, float y, AEGfxTexture* image);
}


namespace Entity {
    class Player;
}

namespace Clamping {
	void clamper(Entity::Player* localPlayer);
}

#endif // GRAPHICS_HPP
