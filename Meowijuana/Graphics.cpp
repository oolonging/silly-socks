#include "Graphics.hpp"
#include "AEEngine.h"
#include "Settings.hpp"
#include <unordered_map>

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
		Settings::gStrokeWeight = value * 2;
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


		// Game color palette (UI Elements)
		const Color LightBrown = { 206,160,134,100 };
		const Color Brown = { 206,160,134,100 };
		const Color DarkBrown{ 89, 75, 66, 100 };

		const Color defaultStylePrimary = LightBrown;
		const Color defaultStyleSecondary = Brown;
		const Color defaultStyleStroke = DarkBrown;
		const Color defaultTextStylePrimary = Black;
		const Color defaultTextStyleSecondary = White;
	}
}

namespace Shapes {
	static AEGfxVertexList* sRectMesh = nullptr;
	static AEGfxVertexList* sRectStrokeMesh = nullptr;
	static AEGfxVertexList* sEllipseMesh = nullptr;
	static AEGfxVertexList* sEllipseStrokeMesh = nullptr;

	static AEGfxVertexList* sTriangleMesh = nullptr;
	static AEGfxVertexList* sTriangleStrokeMesh = nullptr;

	// Cache for stroke meshes with different weights
	static std::unordered_map<int, AEGfxVertexList*> sRectStrokeCache;
	static std::unordered_map<int, AEGfxVertexList*> sEllipseStrokeCache;

	// Globals for ellipse type
	const int NUM_SEGMENTS = 32;

	// Helper function to create a stroke mesh for a rectangle with thickness
	AEGfxVertexList* createRectStrokeMeshWithWeight(float strokeWeight) {
		AEGfxMeshStart();

		// Normalize stroke weight (convert pixel space to unit square space)
		float halfStroke = strokeWeight * 0.5f;
		float outer = 0.5f + halfStroke;
		float inner = 0.5f - halfStroke;

		// Top edge (outer to inner)
		AEGfxTriAdd(-outer, outer, 0xFFFFFFFF, 0.0f, 0.0f,
			outer, outer, 0xFFFFFFFF, 1.0f, 0.0f,
			-inner, inner, 0xFFFFFFFF, 0.0f, 0.0f);
		AEGfxTriAdd(outer, outer, 0xFFFFFFFF, 1.0f, 0.0f,
			inner, inner, 0xFFFFFFFF, 1.0f, 0.0f,
			-inner, inner, 0xFFFFFFFF, 0.0f, 0.0f);

		// Right edge
		AEGfxTriAdd(outer, outer, 0xFFFFFFFF, 1.0f, 0.0f,
			outer, -outer, 0xFFFFFFFF, 1.0f, 1.0f,
			inner, inner, 0xFFFFFFFF, 1.0f, 0.0f);
		AEGfxTriAdd(outer, -outer, 0xFFFFFFFF, 1.0f, 1.0f,
			inner, -inner, 0xFFFFFFFF, 1.0f, 1.0f,
			inner, inner, 0xFFFFFFFF, 1.0f, 0.0f);

		// Bottom edge
		AEGfxTriAdd(outer, -outer, 0xFFFFFFFF, 1.0f, 1.0f,
			-outer, -outer, 0xFFFFFFFF, 0.0f, 1.0f,
			inner, -inner, 0xFFFFFFFF, 1.0f, 1.0f);
		AEGfxTriAdd(-outer, -outer, 0xFFFFFFFF, 0.0f, 1.0f,
			-inner, -inner, 0xFFFFFFFF, 0.0f, 1.0f,
			inner, -inner, 0xFFFFFFFF, 1.0f, 1.0f);

		// Left edge
		AEGfxTriAdd(-outer, -outer, 0xFFFFFFFF, 0.0f, 1.0f,
			-outer, outer, 0xFFFFFFFF, 0.0f, 0.0f,
			-inner, -inner, 0xFFFFFFFF, 0.0f, 1.0f);
		AEGfxTriAdd(-outer, outer, 0xFFFFFFFF, 0.0f, 0.0f,
			-inner, inner, 0xFFFFFFFF, 0.0f, 0.0f,
			-inner, -inner, 0xFFFFFFFF, 0.0f, 1.0f);

		return AEGfxMeshEnd();
	}

	// Helper function to create a stroke mesh for an ellipse with thickness
	AEGfxVertexList* createEllipseStrokeMeshWithWeight(float strokeWeight) {
		AEGfxMeshStart();

		float halfStroke = strokeWeight * 0.5f;
		float outerRadius = 0.5f + halfStroke;
		float innerRadius = 0.5f - halfStroke;

		// Clamp inner radius to prevent negative values
		if (innerRadius < 0.0f) innerRadius = 0.0f;

		for (int i = 0; i < NUM_SEGMENTS; i++) {
			float angle0 = (2.0f * PI * i) / NUM_SEGMENTS;
			float angle1 = (2.0f * PI * (i + 1)) / NUM_SEGMENTS;

			float cosA0 = cosf(angle0);
			float sinA0 = sinf(angle0);
			float cosA1 = cosf(angle1);
			float sinA1 = sinf(angle1);

			float outerX0 = outerRadius * cosA0;
			float outerY0 = outerRadius * sinA0;
			float outerX1 = outerRadius * cosA1;
			float outerY1 = outerRadius * sinA1;

			float innerX0 = innerRadius * cosA0;
			float innerY0 = innerRadius * sinA0;
			float innerX1 = innerRadius * cosA1;
			float innerY1 = innerRadius * sinA1;

			// Create a quad strip segment (two triangles)
			AEGfxTriAdd(outerX0, outerY0, 0xFFFFFFFF, 0.0f, 0.0f,
				innerX0, innerY0, 0xFFFFFFFF, 0.0f, 1.0f,
				outerX1, outerY1, 0xFFFFFFFF, 1.0f, 0.0f);

			AEGfxTriAdd(outerX1, outerY1, 0xFFFFFFFF, 1.0f, 0.0f,
				innerX0, innerY0, 0xFFFFFFFF, 0.0f, 1.0f,
				innerX1, innerY1, 0xFFFFFFFF, 1.0f, 1.0f);
		}

		return AEGfxMeshEnd();
	}

	// Get or create cached rect stroke mesh
	AEGfxVertexList* getRectStrokeMesh(int strokeWeight) {
		auto it = sRectStrokeCache.find(strokeWeight);
		if (it != sRectStrokeCache.end()) {
			return it->second;
		}

		// Create and cache new mesh
		float normalizedWeight = (float)strokeWeight / 100.0f; // Normalize to reasonable range
		AEGfxVertexList* mesh = createRectStrokeMeshWithWeight(normalizedWeight);
		sRectStrokeCache[strokeWeight] = mesh;
		return mesh;
	}

	// Get or create cached ellipse stroke mesh
	AEGfxVertexList* getEllipseStrokeMesh(int strokeWeight) {
		auto it = sEllipseStrokeCache.find(strokeWeight);
		if (it != sEllipseStrokeCache.end()) {
			return it->second;
		}

		// Create and cache new mesh
		float normalizedWeight = (float)strokeWeight / 100.0f; // Normalize to reasonable range
		AEGfxVertexList* mesh = createEllipseStrokeMeshWithWeight(normalizedWeight);
		sEllipseStrokeCache[strokeWeight] = mesh;
		return mesh;
	}

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

		// Clear cached stroke meshes
		for (auto& pair : sRectStrokeCache) {
			AEGfxMeshFree(pair.second);
		}
		sRectStrokeCache.clear();

		for (auto& pair : sEllipseStrokeCache) {
			AEGfxMeshFree(pair.second);
		}
		sEllipseStrokeCache.clear();
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
		AEMtx33Rot(&rot, rotation);

		if (drawMode == CENTER) {
			AEMtx33Trans(&translation, x, y);
			AEMtx33Concat(&transformation, &rot, &scale);
			AEMtx33Concat(&transformation, &translation, &transformation);
		}
		else {
			AEMtx33 localOffset{};
			AEMtx33 temp{};

			AEMtx33Trans(&localOffset, 0.5f * width, -0.5f * height);
			AEMtx33Trans(&translation, x, y);
			AEMtx33Concat(&temp, &localOffset, &scale);

			AEMtx33Concat(&temp, &rot, &temp);
			AEMtx33Concat(&transformation, &translation, &temp);
		}

		AEGfxSetTransform(transformation.m);

		// Draw the fill
		AEGfxSetColorToMultiply(
			Settings::gFillColor.red / 255.0f,
			Settings::gFillColor.green / 255.0f,
			Settings::gFillColor.blue / 255.0f,
			Settings::gFillColor.alpha / 255.0f
		);
		AEGfxMeshDraw(sRectMesh, AE_GFX_MDM_TRIANGLES);

		// Draw the stroke with thickness
		if (drawStroke) {
			// Get cached stroke mesh
			AEGfxVertexList* strokeMesh = getRectStrokeMesh(Settings::gStrokeWeight);

			AEGfxSetTransform(transformation.m);
			AEGfxSetColorToMultiply(
				Settings::gStrokeColor.red / 255.0f,
				Settings::gStrokeColor.green / 255.0f,
				Settings::gStrokeColor.blue / 255.0f,
				Settings::gStrokeColor.alpha / 255.0f
			);
			AEGfxMeshDraw(strokeMesh, AE_GFX_MDM_TRIANGLES);
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
		AEMtx33Rot(&rot, rotation);

		if (drawMode == CENTER) {
			AEMtx33Trans(&translation, x, y);
			AEMtx33Concat(&transformation, &rot, &scale);
			AEMtx33Concat(&transformation, &translation, &transformation);
		}
		else {
			AEMtx33 localOffset{};
			AEMtx33 temp{};

			AEMtx33Trans(&localOffset, 0.5f * width, -0.5f * height);
			AEMtx33Trans(&translation, x, y);
			AEMtx33Concat(&temp, &localOffset, &scale);

			AEMtx33Concat(&temp, &rot, &temp);
			AEMtx33Concat(&transformation, &translation, &temp);
		}

		AEGfxSetTransform(transformation.m);

		// Draw the fill
		AEGfxSetColorToMultiply(
			Settings::gFillColor.red / 255.0f,
			Settings::gFillColor.green / 255.0f,
			Settings::gFillColor.blue / 255.0f,
			Settings::gFillColor.alpha / 255.0f
		);
		AEGfxMeshDraw(sEllipseMesh, AE_GFX_MDM_TRIANGLES);

		// Draw the stroke with thickness
		if (drawStroke) {
			// Get cached stroke mesh
			AEGfxVertexList* strokeMesh = getEllipseStrokeMesh(Settings::gStrokeWeight);

			AEGfxSetTransform(transformation.m);
			AEGfxSetColorToMultiply(
				Settings::gStrokeColor.red / 255.0f,
				Settings::gStrokeColor.green / 255.0f,
				Settings::gStrokeColor.blue / 255.0f,
				Settings::gStrokeColor.alpha / 255.0f
			);
			AEGfxMeshDraw(strokeMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
	void ellipse(float x, float y, float width, float height, SHAPE_MODE drawMode) {
		ellipseAdvanced(x, y, width, height, 0.0f, drawMode);
	}
	void circle(float x, float y, float size, SHAPE_MODE drawMode) {
		ellipseAdvanced(x, y, size, size, 0.0f, drawMode);
	}

	// Triangle functions
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
		// Create mesh
		AEGfxMeshStart();

		AEGfxTriAdd(x1, y1, 0xFFFFFFFF, 0.0f, 0.0f,
			x2, y2, 0xFFFFFFFF, 0.0f, 0.0f,
			x3, y3, 0xFFFFFFFF, 0.0f, 0.0f
		);
		sTriangleMesh = AEGfxMeshEnd();

		// Define transformations
		AEMtx33 scale{};
		AEMtx33 rot{};
		AEMtx33 translation{};
		AEMtx33 transformation{};

		AEMtx33Scale(&scale, 1.0f, 1.0f);
		AEMtx33Rot(&rot, 0.0f);
		AEMtx33Trans(&translation, 0.0f, 0.0f);

		AEMtx33Concat(&transformation, &rot, &scale);
		AEMtx33Concat(&transformation, &translation, &transformation);

		// Reset transformations
		AEGfxSetTransform(transformation.m);

		// set color
		AEGfxSetColorToMultiply(
			Settings::gFillColor.red / 255.0f,
			Settings::gFillColor.green / 255.0f,
			Settings::gFillColor.blue / 255.0f,
			Settings::gFillColor.alpha / 255.0f
		);
		AEGfxMeshDraw(sTriangleMesh, AE_GFX_MDM_TRIANGLES);
	}
	void triangle(Point p1, Point p2, Point p3) {
		triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
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
		if (fontId == -1) return false;

		Settings::gFonts[fontName] = fontId;

		if (Settings::gCurrentFontId == -1) {
			Settings::gCurrentFontId = fontId;
		}

		return true;
	}

	void setFont(std::string fontName) {
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

	void text(const char* str, float x, float y, TEXT_ALIGN_HORIZONTAL horizontal, TEXT_ALIGN_VERTICAL vertical) {
		if (Settings::gCurrentFontId == -1) return;

		float normalizedX = x / (AEGfxGetWindowWidth() * 0.5f);
		float normalizedY = y / (AEGfxGetWindowHeight() * 0.5f);

		float width = 0.0f, height = 0.0f;
		AEGfxGetPrintSize(Settings::gCurrentFontId, str, Settings::gTextSize, &width, &height);
		float actualWidth = (width * 0.5f) * AEGfxGetWindowWidth();
		float actualHeight = (height * 0.5f) * AEGfxGetWindowHeight();

		float offsetX = 0.0f, offsetY = 0.0f;

		if (horizontal == CENTER_H) offsetX = -width * 0.5f;
		else if (horizontal == RIGHT) offsetX = -width;

		if (vertical == CENTER_V) offsetY = -height * 0.5f;
		else if (vertical == BOTTOM) offsetY = -height;

		float worldOffsetX = 0.0f, worldOffsetY = 0.0f;

		if (horizontal == CENTER_H) worldOffsetX = 0.0f;
		else if (horizontal == RIGHT) worldOffsetX = -actualWidth * 0.5f;
		else if (horizontal == LEFT) worldOffsetX = actualWidth * 0.5f;

		if (vertical == CENTER_V) worldOffsetY = 0.0f;
		else if (vertical == BOTTOM) worldOffsetY = -actualHeight * 0.5f;
		else if (vertical == TOP) worldOffsetY = actualHeight * 0.5f;
		else if (vertical == BASELINE) worldOffsetY = -actualHeight * 0.25f;

		if (Settings::gDebugMode) {
			Color::fill(255.0f, 0.0f, 0.0f, 128.0f);
			Color::noStroke();
			Shapes::rect(x + worldOffsetX, y + worldOffsetY, actualWidth, actualHeight, Shapes::CENTER);
		}

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

	void text(const char* str, float x, float y) {
		text(str, x, y, Settings::gTextAlignHorizontal, Settings::gTextAlignVertical);
	}

	void textSize(float size) {
		Settings::gTextSize = size / Settings::gDefaultTextSize;
	}

	void textAlign(TEXT_ALIGN_HORIZONTAL horizontal, TEXT_ALIGN_VERTICAL vertical) {
		Settings::gTextAlignHorizontal = horizontal;
		Settings::gTextAlignVertical = vertical;
	}

	void exit(void) {
		for (auto& pair : Settings::gFonts) {
			AEGfxDestroyFont(pair.second);
		}
		Settings::gFonts.clear();
		Settings::gCurrentFontId = -1;
	}
}

namespace Animations {


	void updateIndicator(Indicator &ind) {
		ind.offset += 20.0f * ind.dir * (float)AEFrameRateControllerGetFrameTime();
		if (ind.offset > 8.0f) ind.dir = -1.0f;
		if (ind.offset < -10.0f) ind.dir = 1.0f;

	}

	void drawIndicator(Indicator& ind) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		Color::fill(176, 222, 88, 255);
		Shapes::triangle(ind.x, ind.y + 50 + ind.offset, ind.x - 10, ind.y + 60 + ind.offset, ind.x + 10, ind.y + 60 + ind.offset);

	}

}