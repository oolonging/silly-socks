#include "Graphics.hpp"
#include "AEEngine.h"

#include "Settings.hpp"

namespace Color {
	// TODO: missing
	// Create color with HSL
	// separate text color (decide if its needed or not)

	// Create colors
	Color createColorRGB(float red, float green, float blue, float alpha) {
		return { red, green, blue, alpha };
	}
	Color createColorHex(int hexValue) {
		// Color looks like: AARRGGBB
		Color color;
		color.alpha = (float)((hexValue >> 24) & 0xFF);
		color.red = (float)((hexValue >> 16) & 0xFF);
		color.green = (float)((hexValue >> 8) & 0xFF);
		color.blue = (float)(hexValue & 0xFF);
		return color;
	}

	// Fill
	void fill(float red, float green, float blue, float alpha) {
		Settings::gFillColor = { red, green, blue, alpha };
	}
	void fill(Color color) {
		Settings::gFillColor = color;
	}
	void noFill(void) { fill(0.0f, 0.0f, 0.0f, 0.0f); }

	// Stroke
	void stroke(float red, float green, float blue, float alpha) {
		Settings::gStrokeColor = { red, green, blue, alpha };
	}
	void stroke(Color color) {
		Settings::gStrokeColor = color;
	}
	void noStroke(void) { stroke(0.0f, 0.0f, 0.0f, 0.0f); }

	// Text Fill
	void textFill(float red, float green, float blue, float alpha) {
		Settings::gTextFillColor = { red, green, blue, alpha };
	}
	void textFill(Color color) {
		Settings::gTextFillColor = color;
	}

	// Stroke weight
	void strokeWeight(int value) {
		Settings::gStrokeWeight = value;
	}

	// background
	void background(float red, float green, float blue) {
		AEGfxSetBackgroundColor(red / 255.0f, green / 255.0f, blue / 255.0f);
	}
	void background(Color color) {
		background(color.red, color.green, color.blue);
	}

	// Preset colors
	namespace Preset {
		const Color Black = { 0,   0,   0, 255 };
		const Color White = { 255, 255, 255, 255 };
		const Color Red = { 255,   0,   0, 255 };
		const Color Green = { 0, 255,   0, 255 };
		const Color Blue = { 0,   0, 255, 255 };
		const Color Yellow = { 255, 255,   0, 255 };
		const Color Cyan = { 0, 255, 255, 255 };
		const Color Magenta = { 255,   0, 255, 255 };
		const Color Orange = { 255, 165,   0, 255 };
		const Color Purple = { 128,   0, 128, 255 };
		const Color Gray = { 128, 128, 128, 255 };
		const Color DarkGray = { 64,  64,  64, 255 };
		const Color LightGray = { 192, 192, 192, 255 };
		const Color Transparent = { 0,   0,   0,   0 };
	}
}

namespace Shapes {
	static AEGfxVertexList* sRectMesh = nullptr;
	static AEGfxVertexList* sRectStrokeMesh = nullptr;
	static AEGfxVertexList* sEllipseMesh = nullptr;
	static AEGfxVertexList* sEllipseStrokeMesh = nullptr;

	// Globals for ellipse type
	const int NUM_SEGMENTS = 32;

	bool init(void) {
		if (sRectMesh && sRectStrokeMesh && sEllipseMesh && sEllipseStrokeMesh) return true;

		if (!sRectMesh) {
			AEGfxMeshStart();

			AEGfxTriAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
				0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
				-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
			AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
				0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
				-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

			sRectMesh = AEGfxMeshEnd();
		}

		if (!sRectStrokeMesh) {
			AEGfxMeshStart();

			// Define the outline
			AEGfxVertexAdd(-0.5, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);		// Top-left
			AEGfxVertexAdd(0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);		// Top-right
			AEGfxVertexAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f);	// Bottom-right
			AEGfxVertexAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f);	// Bottom-left
			AEGfxVertexAdd(-0.5, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);		// Top-left


			sRectStrokeMesh = AEGfxMeshEnd();
		}

		if (!sEllipseMesh) {
			AEGfxMeshStart();

			for (int i = 0; i < NUM_SEGMENTS; i++) {
				float angle0 = (2.0f * PI * i) / NUM_SEGMENTS;
				float angle1 = (2.0f * PI * (i + 1)) / NUM_SEGMENTS;

				float x0 = 0.5f * cosf(angle0);
				float y0 = 0.5f * sinf(angle0);
				float x1 = 0.5f * cosf(angle1);
				float y1 = 0.5f * sinf(angle1);

				// with texture mapping
				AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.5f, 0.5f,
					x0, y0, 0xFFFFFFFF, (x0 / 0.5f + 1.0f) * 0.5f, (y0 / 0.5f + 1.0f) * 0.5f,
					x1, y1, 0xFFFFFFFF, (x1 / 0.5f + 1.0f) * 0.5f, (y1 / 0.5f + 1.0f) * 0.5f);
			}

			sEllipseMesh = AEGfxMeshEnd();
		}

		if (!sEllipseStrokeMesh) {
			AEGfxMeshStart();

			for (int i = 0; i < NUM_SEGMENTS; i++) {
				float angle = (2.0f * PI * i) / NUM_SEGMENTS;
				float x = 0.5f * cosf(angle);
				float y = 0.5f * sinf(angle);
				AEGfxVertexAdd(x, y, 0xFFFFFFFF, (x / 0.5f + 1.0f) * 0.5f, (y / 0.5f + 1.0f) * 0.5f);
			}

			sEllipseStrokeMesh = AEGfxMeshEnd();
		}

		return (sRectMesh && sRectStrokeMesh && sEllipseMesh && sEllipseStrokeMesh);
	}

	void exit(void) {
		if (sRectMesh) {
			AEGfxMeshFree(sRectMesh);
			sRectMesh = nullptr;
		}

		if (sRectStrokeMesh) {
			AEGfxMeshFree(sRectStrokeMesh);
			sRectStrokeMesh = nullptr;
		}

		if (sEllipseMesh) {
			AEGfxMeshFree(sEllipseMesh);
			sEllipseMesh = nullptr;
		}

		if (sEllipseStrokeMesh) {
			AEGfxMeshFree(sEllipseStrokeMesh);
			sEllipseStrokeMesh = nullptr;
		}
	}

	void rectAdvanced(float x, float y, float width, float height, float rotation, SHAPE_MODE drawMode) {
		// 1. safety: Ensure the mesh is defined before drawing
		if (!sRectMesh || !sRectStrokeMesh) return;

		bool drawStroke = (Settings::gStrokeColor.alpha > 0.0f) && (Settings::gStrokeWeight > 0);

		// 2. Define transformations
		AEMtx33 scale{};
		AEMtx33 rot{};
		AEMtx33 translation{};
		AEMtx33 transformation{};

		AEMtx33Scale(&scale, width, height);
		AEMtx33Rot(&rot, rotation); // TODO: set rotation at some point

		if (drawMode == CENTER) {
			AEMtx33Trans(&translation, x, y);
			AEMtx33Concat(&transformation, &rot, &scale);
			AEMtx33Concat(&transformation, &translation, &transformation);
		}
		else {
			AEMtx33 localOffset{};
			AEMtx33 temp{};

			AEMtx33Trans(&localOffset, 0.5f * width, -0.5f * height);	// Move top-left to origin in local space
			AEMtx33Trans(&translation, x, y);							// World position
			AEMtx33Concat(&temp, &localOffset, &scale);

			AEMtx33Concat(&temp, &rot, &temp);
			AEMtx33Concat(&transformation, &translation, &temp);
		}

		AEGfxSetTransform(transformation.m);

		// Draw the shape
		AEGfxSetColorToMultiply(
			Settings::gFillColor.red / 255.0f,
			Settings::gFillColor.green / 255.0f,
			Settings::gFillColor.blue / 255.0f,
			Settings::gFillColor.alpha / 255.0f
		);
		AEGfxMeshDraw(sRectMesh, AE_GFX_MDM_TRIANGLES);

		// Draw the stroke
		if (drawStroke) {
			AEGfxSetColorToMultiply(
				Settings::gStrokeColor.red / 255.0f,
				Settings::gStrokeColor.green / 255.0f,
				Settings::gStrokeColor.blue / 255.0f,
				Settings::gStrokeColor.alpha / 255.0f
			);
			AEGfxMeshDraw(sRectStrokeMesh, AE_GFX_MDM_LINES_STRIP);
		}
	}
	void rect(float x, float y, float width, float height, SHAPE_MODE drawMode) {
		rectAdvanced(x, y, width, height, 0.0f, drawMode);
	}
	void square(float x, float y, float size, SHAPE_MODE drawMode) {
		rectAdvanced(x, y, size, size, 0.0f, drawMode);
	}

	void ellipseAdvanced(float x, float y, float width, float height, float rotation, SHAPE_MODE drawMode) {
		// 1. safety: Ensure the mesh is defined before drawing
		if (!sEllipseMesh || !sEllipseStrokeMesh) return;

		bool drawStroke = (Settings::gStrokeColor.alpha > 0.0f) && (Settings::gStrokeWeight > 0);

		// 2. Define transformations
		AEMtx33 scale{};
		AEMtx33 rot{};
		AEMtx33 translation{};
		AEMtx33 transformation{};

		AEMtx33Scale(&scale, width, height);
		AEMtx33Rot(&rot, rotation); // TODO: set rotation at some point

		if (drawMode == CENTER) {
			AEMtx33Trans(&translation, x, y);
			AEMtx33Concat(&transformation, &rot, &scale);
			AEMtx33Concat(&transformation, &translation, &transformation);
		}
		else {
			AEMtx33 localOffset{};
			AEMtx33 temp{};

			AEMtx33Trans(&localOffset, 0.5f * width, -0.5f * height);	// Move top-left to origin in local space
			AEMtx33Trans(&translation, x, y);							// World position
			AEMtx33Concat(&temp, &localOffset, &scale);

			AEMtx33Concat(&temp, &rot, &temp);
			AEMtx33Concat(&transformation, &translation, &temp);
		}

		AEGfxSetTransform(transformation.m);

		// Draw the shape
		AEGfxSetColorToMultiply(
			Settings::gFillColor.red / 255.0f,
			Settings::gFillColor.green / 255.0f,
			Settings::gFillColor.blue / 255.0f,
			Settings::gFillColor.alpha / 255.0f
		);
		AEGfxMeshDraw(sEllipseMesh, AE_GFX_MDM_TRIANGLES);

		// Draw the stroke
		if (drawStroke) {
			AEGfxSetColorToMultiply(
				Settings::gStrokeColor.red / 255.0f,
				Settings::gStrokeColor.green / 255.0f,
				Settings::gStrokeColor.blue / 255.0f,
				Settings::gStrokeColor.alpha / 255.0f
			);
			AEGfxMeshDraw(sEllipseStrokeMesh, AE_GFX_MDM_LINES_STRIP);
		}
	}
	void ellipse(float x, float y, float width, float height, SHAPE_MODE drawMode) {
		ellipseAdvanced(x, y, width, height, 0.0f, drawMode);
	}
	void circle(float x, float y, float size, SHAPE_MODE drawMode) {
		ellipseAdvanced(x, y, size, size, 0.0f, drawMode);
	}
}

namespace Graphics {
	void image(float x, float y, float width, float height, AEGfxTexture* pTex, Shapes::SHAPE_MODE drawMode) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		Color::fill(255, 255, 255);
		Color::noStroke();
		AEGfxTextureSet(pTex, 0, 0);
		Shapes::rect(x, y, width, height, drawMode);

	}
}

namespace Text {
	// creating a font
	bool createFont(const char* fontPath, int fontSize, const char* fontName) {
		s8 fontId = AEGfxCreateFont(fontPath, fontSize);
		if (fontId == -1) return false; // Font failed to load

		Settings::gFonts[fontName] = fontId;

		// Set as current font if its the first one
		if (Settings::gCurrentFontId == -1) {
			Settings::gCurrentFontId = fontId;
		}

		return true;
	}

	// TODO: understand how this and the destroyfont work
	void setFont(const char* fontName) {
		// figure out the different types that could be returned here
		auto it = Settings::gFonts.find(fontName);
		if (it != Settings::gFonts.end()) {
			Settings::gCurrentFontId = it->second;
		}
	}

	void destroyFont(const char* fontName) {
		auto it = Settings::gFonts.find(fontName);
		if (it != Settings::gFonts.end()) {
			AEGfxDestroyFont(it->second);
			Settings::gFonts.erase(it);
		}
	}

	// Render text
	void text(const char* str, float x, float y) {
		if (Settings::gCurrentFontId == -1) return; // No font has been set

		// TODO: interesting question that came up is that is there a difference between multiplying a float by 0.5f or dividing it by 2.0f
		// I was wondering if the machine code generated by the compiler would be identical or not
		// Convert workd coordinates to normalized screen coordinates
		float normalizedX = x / (AEGfxGetWindowWidth() * 0.5f);
		float normalizedY = y / (AEGfxGetWindowHeight() * 0.5f);

		// Calculate the offset based on alignment
		float width = 0.0f, height = 0.0f;
		AEGfxGetPrintSize(Settings::gCurrentFontId, str, Settings::gTextSize, &width, &height);

		float offsetX = 0.0f, offsetY = 0.0f;

		// Horizontal alignment
		if (Settings::gTextAlignHorizontal == CENTER_H) offsetX = -width * 0.5f;
		else if (Settings::gTextAlignHorizontal == RIGHT) offsetX = -width;

		// Vertical alignment
		if (Settings::gTextAlignVertical == CENTER_V) offsetY = -height * 0.5f;
		else if (Settings::gTextAlignVertical == BOTTOM) offsetY = -height;
		// TODO: add the one more i forgot what I called it

		// Set blend mode for text
		// TODO: see if this is optional or not
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		AEGfxPrint(Settings::gCurrentFontId, str,
			normalizedX + offsetX, normalizedY + offsetY,
			Settings::gTextSize,
			Settings::gTextFillColor.red / 255.0f,
			Settings::gTextFillColor.green / 255.0f,
			Settings::gTextFillColor.blue / 255.0f,
			Settings::gTextFillColor.alpha / 255.0f
		);
	}

	void textSize(float size) {
		Settings::gTextSize = size;
	}

	// TODO: Cheap hack, figure out how to implement properly
	void textPixelSize(int pixelSize) {
		Settings::gTextSize = (float)pixelSize / 24.0f;
	}

	void textAlign(TEXT_ALIGN_HORIZONTAL horizontal, TEXT_ALIGN_VERTICAL vertical) {
		Settings::gTextAlignHorizontal = horizontal;
		Settings::gTextAlignVertical = vertical;
	}

	// goodbye function
	void exit(void) {
		// Destroy all loaded fonts
		for (auto& pair : Settings::gFonts) {
			AEGfxDestroyFont(pair.second);
		}
		Settings::gFonts.clear();
		Settings::gCurrentFontId = -1;
	}
}
