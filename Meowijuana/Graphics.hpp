/**
 * @file Graphics.hpp
 * @author Saahil
 * @brief Core graphics API providing color, shape, text, image, and animation utilities.
 */

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "pch.h"
#include "GraphicsTypes.hpp"

 /**
  * @namespace Color
  * @brief Functions for creating colors and managing fill, stroke, and background colors.
  */
namespace Color {
	/**
	 * @brief Creates a Color from RGBA float values (0-255).
	 * @param red Red component.
	 * @param green Green component.
	 * @param blue Blue component.
	 * @param alpha Alpha component (defaults to 255.0f).
	 * @return The constructed Color.
	 */
	Color createColorRGB(float red, float green, float blue, float alpha = 255.0f);

	/**
	 * @brief Creates a Color from a hex value (e.g. 0xFFFF0000 for red).
	 * @param hexValue Integer hex color value.
	 * @return The constructed Color.
	 */
	Color createColorHex(int hexValue);

	/**
	 * @brief Sets the global fill color using RGBA float values.
	 * @param red Red component.
	 * @param green Green component.
	 * @param blue Blue component.
	 * @param alpha Alpha component (defaults to 255.0f).
	 */
	void fill(float red, float green, float blue, float alpha = 255.0f);

	/**
	 * @brief Sets the global fill color using a Color object.
	 * @param color The Color to use.
	 */
	void fill(Color color);

	/**
	 * @brief Disables fill for subsequent draw calls.
	 */
	void noFill(void);

	/**
	 * @brief Sets the global stroke color using RGBA float values.
	 * @param red Red component.
	 * @param green Green component.
	 * @param blue Blue component.
	 * @param alpha Alpha component (defaults to 255.0f).
	 */
	void stroke(float red, float green, float blue, float alpha = 255.0f);

	/**
	 * @brief Sets the global stroke color using a Color object.
	 * @param color The Color to use.
	 */
	void stroke(Color color);

	/**
	 * @brief Disables stroke for subsequent draw calls.
	 */
	void noStroke(void);

	/**
	 * @brief Sets the text fill color using RGBA float values.
	 * @param red Red component.
	 * @param green Green component.
	 * @param blue Blue component.
	 * @param alpha Alpha component (defaults to 255.0f).
	 */
	void textFill(float red, float green, float blue, float alpha = 255.0f);

	/**
	 * @brief Sets the text fill color using a Color object.
	 * @param color The Color to use.
	 */
	void textFill(Color color);

	/**
	 * @brief Sets the stroke weight (thickness) in pixels.
	 * @param value Stroke weight value.
	 */
	void strokeWeight(int value);

	/**
	 * @brief Clears the screen with the given RGB color.
	 * @param red Red component.
	 * @param green Green component.
	 * @param blue Blue component.
	 */
	void background(float red, float green, float blue);

	/**
	 * @brief Clears the screen with the given Color.
	 * @param color The background Color.
	 */
	void background(Color color);

	/**
	 * @namespace Preset
	 * @brief Predefined color constants for common and game-specific colors.
	 */
	namespace Preset {
		extern const Color Black;			///< Black (0, 0, 0).
		extern const Color White;			///< White (255, 255, 255).
		extern const Color Red;				///< Red (255, 0, 0).
		extern const Color Green;			///< Green (0, 255, 0).
		extern const Color Blue;			///< Blue (0, 0, 255).
		extern const Color Yellow;			///< Yellow (255, 255, 0).
		extern const Color Cyan;			///< Cyan (0, 255, 255).
		extern const Color Magenta;			///< Magenta (255, 0, 255).
		extern const Color Orange;			///< Orange.
		extern const Color Purple;			///< Purple.
		extern const Color Gray;			///< Gray.
		extern const Color DarkGray;		///< Dark gray.
		extern const Color LightGray;		///< Light gray.
		extern const Color Transparent;		///< Fully transparent.

		// Game color palette (UI Elements)
		extern const Color defaultStylePrimary;			///< Default UI primary color.
		extern const Color defaultStyleSecondary;		///< Default UI secondary color.
		extern const Color defaultStyleStroke;			///< Default UI stroke color.
		extern const Color defaultTextStylePrimary;		///< Default UI text primary color.
		extern const Color defaultTextStyleSecondary;	///< Default UI text secondary color.
	}
}

/**
 * @namespace Shapes
 * @brief Functions for drawing primitive shapes (rectangles, ellipses, triangles, lines, etc.).
 */
namespace Shapes {
	/**
	 * @brief Initializes shape meshes. Call once at startup.
	 * @return True if initialization succeeded.
	 */
	bool init(void);

	/**
	 * @brief Frees all shape meshes. Call on shutdown.
	 */
	void exit(void);

	/**
	 * @brief Draws a rotated rectangle.
	 * @param x X position.
	 * @param y Y position.
	 * @param width Width of the rectangle.
	 * @param height Height of the rectangle.
	 * @param rotation Rotation angle in radians.
	 * @param drawMode Drawing anchor mode (defaults to CORNER).
	 */
	void rectAdvanced(float x, float y, float width, float height, float rotation, SHAPE_MODE drawMode = CORNER);

	/**
	 * @brief Draws a rectangle.
	 * @param x X position.
	 * @param y Y position.
	 * @param width Width of the rectangle.
	 * @param height Height of the rectangle.
	 * @param drawMode Drawing anchor mode (defaults to CORNER).
	 */
	void rect(float x, float y, float width, float height, SHAPE_MODE drawMode = CORNER);

	/**
	 * @brief Draws a square.
	 * @param x X position.
	 * @param y Y position.
	 * @param size Side length.
	 * @param drawMode Drawing anchor mode (defaults to CENTER).
	 */
	void square(float x, float y, float size, SHAPE_MODE drawMode = CENTER);

	/**
	 * @brief Draws a rotated ellipse.
	 * @param x X position.
	 * @param y Y position.
	 * @param width Width of the ellipse.
	 * @param height Height of the ellipse.
	 * @param rotation Rotation angle in radians.
	 * @param drawMode Drawing anchor mode (defaults to CENTER).
	 */
	void ellipseAdvanced(float x, float y, float width, float height, float rotation, SHAPE_MODE drawMode = CENTER);

	/**
	 * @brief Draws an ellipse.
	 * @param x X position.
	 * @param y Y position.
	 * @param width Width of the ellipse.
	 * @param height Height of the ellipse.
	 * @param drawMode Drawing anchor mode (defaults to CENTER).
	 */
	void ellipse(float x, float y, float width, float height, SHAPE_MODE drawMode = CENTER);

	/**
	 * @brief Draws a circle.
	 * @param x X position.
	 * @param y Y position.
	 * @param size Diameter of the circle.
	 * @param drawMode Drawing anchor mode (defaults to CENTER).
	 */
	void circle(float x, float y, float size, SHAPE_MODE drawMode = CENTER);

	/**
	 * @brief Draws a triangle from three coordinate pairs.
	 * @param x1 First vertex X.
	 * @param y1 First vertex Y.
	 * @param x2 Second vertex X.
	 * @param y2 Second vertex Y.
	 * @param x3 Third vertex X.
	 * @param y3 Third vertex Y.
	 */
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);

	/**
	 * @brief Draws a triangle from three Point objects.
	 * @param p1 First vertex.
	 * @param p2 Second vertex.
	 * @param p3 Third vertex.
	 */
	void triangle(Point p1, Point p2, Point p3);

	/**
	 * @brief Draws a line between two coordinate pairs.
	 * @param x1 Start X.
	 * @param y1 Start Y.
	 * @param x2 End X.
	 * @param y2 End Y.
	 */
	void line(float x1, float y1, float x2, float y2);

	/**
	 * @brief Draws a line between two Point objects.
	 * @param p1 Start point.
	 * @param p2 End point.
	 */
	void line(Point p1, Point p2);

	/**
	 * @brief Draws a debug-only rectangle outline.
	 * @param x X position.
	 * @param y Y position.
	 * @param width Width of the rectangle.
	 * @param height Height of the rectangle.
	 * @param drawMode Drawing anchor mode.
	 */
	void debugRect(float x, float y, float width, float height, SHAPE_MODE drawMode);

	/**
	 * @brief Draws a heart shape.
	 * @param x X position.
	 * @param y Y position.
	 * @param size Size of the heart.
	 * @param drawMode Drawing anchor mode (defaults to CENTER).
	 */
	void heart(float x, float y, float size, SHAPE_MODE drawMode = CENTER);
}

/**
 * @namespace Graphics
 * @brief Functions for drawing textured images.
 */
namespace Graphics {
	/**
	 * @brief Draws a textured image at the given position and size.
	 * @param x X position.
	 * @param y Y position.
	 * @param width Width of the image.
	 * @param height Height of the image.
	 * @param pTex Pointer to the texture.
	 * @param drawMode Drawing anchor mode (defaults to CORNER).
	 */
	void image(float x, float y, float width, float height, AEGfxTexture* pTex, Shapes::SHAPE_MODE drawMode = Shapes::CORNER);

	/**
	 * @brief Draws a textured image using a Quad descriptor.
	 * @param quad The Quad defining position and size.
	 * @param pTex Pointer to the texture.
	 * @param drawMode Drawing anchor mode (defaults to CORNER).
	 */
	void image(Shapes::Quad quad, AEGfxTexture* pTex, Shapes::SHAPE_MODE drawMode = Shapes::CORNER);
}

/**
 * @namespace Text
 * @brief Functions for font management, text rendering, and text styling.
 */
namespace Text {
	/**
	 * @brief Loads a font from file and registers it with a name.
	 * @param fontPath Path to the font file.
	 * @param fontSize Size of the font in points.
	 * @param fontName Name to register the font under (defaults to "default").
	 * @return True if the font was created successfully.
	 */
	bool createFont(const char* fontPath, int fontSize, const char* fontName = "default");

	/**
	 * @brief Sets the active font by name.
	 * @param fontName Name of a previously registered font.
	 */
	void setFont(std::string fontName);

	/**
	 * @brief Destroys a font by name, freeing its resources.
	 * @param fontName Name of the font to destroy.
	 */
	void destroyFont(const char* fontName);

	/**
	 * @brief Cleans up all font-related resources. Call on shutdown.
	 */
	void exit(void);

	/**
	 * @brief Renders text at the given position with explicit alignment.
	 * @param str The string to render.
	 * @param x X position.
	 * @param y Y position.
	 * @param horizontal Horizontal alignment.
	 * @param vertical Vertical alignment.
	 */
	void text(const char* str, float x, float y, TEXT_ALIGN_HORIZONTAL horizontal, TEXT_ALIGN_VERTICAL vertical);

	/**
	 * @brief Renders text at the given position using the current global alignment.
	 * @param str The string to render.
	 * @param x X position.
	 * @param y Y position.
	 */
	void text(const char* str, float x, float y);

	/**
	 * @brief Sets the current text size.
	 * @param size Text size value.
	 */
	void textSize(float size);

	/**
	 * @brief Sets the current text alignment.
	 * @param horizontal Horizontal alignment.
	 * @param vertical Vertical alignment.
	 */
	void textAlign(TEXT_ALIGN_HORIZONTAL horizontal, TEXT_ALIGN_VERTICAL vertical);
}

/**
 * @namespace Animations
 * @brief Utilities for animated UI indicators.
 */
namespace Animations {

	/**
	 * @struct Indicator
	 * @brief Represents a bouncing indicator animation state.
	 */
	struct Indicator {
		float x = 0.0f;		///< X position.
		float y = 0.0f;		///< Y position.

		float offset = 0.0f;	///< Current vertical offset.
		float dir = 1.0f;		///< Direction of bounce (1.0 = up, -1.0 = down).
		bool active = true;		///< Whether the indicator is active.
	};

	/**
	 * @brief Updates the indicator's bounce animation.
	 * @param ind Reference to the Indicator to update.
	 */
	void updateIndicator(Indicator& ind);

	/**
	 * @brief Draws the indicator at its current position.
	 * @param ind Reference to the Indicator to draw.
	 */
	void drawIndicator(Indicator& ind);

	/**
	 * @brief Draws an indicator at the given position with a manual offset.
	 * @param x X position.
	 * @param y Y position.
	 * @param offset Vertical offset.
	 */
	void drawIndicator(float x, float y, float offset);

	/**
	 * @brief Draws a textured indicator at the given position.
	 * @param x X position.
	 * @param y Y position.
	 * @param image Pointer to the indicator texture.
	 */
	void drawCoolerIndicator(float x, float y, AEGfxTexture* image);
}


namespace Entity {
	class Player;
}

/**
 * @namespace Clamping
 * @brief Utility for clamping the player within screen bounds.
 */
namespace Clamping {
	/**
	 * @brief Clamps the player's position to stay within the visible area.
	 * @param localPlayer Pointer to the Player entity to clamp.
	 */
	void clamper(Entity::Player* localPlayer);
}

#endif // GRAPHICS_HPP