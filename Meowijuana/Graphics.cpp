
#include "Graphics.hpp"
#include "Settings.hpp"
#include "AEEngine.h"

namespace Color {
	CL_Color CL_Color_Create(int red, int green, int blue, int alpha) {
		CL_Color color;
		color.red = red;
		color.green = green;
		color.blue = blue;
		color.alpha = alpha;
		return color;
	}
	CL_Color CL_Color_Create_Hex(int hexValue) {
		CL_Color color;
		color.alpha = (hexValue >> 24) & 0xFF;
		color.red = (hexValue >> 16) & 0xFF;
		color.green = (hexValue >> 8) & 0xFF;
		color.blue = hexValue & 0xFF;
		return color;
	}
	CL_Color CL_Color_Create_HSL(int hue, int saturation, int lightness, int alpha) {
		// TODO: Untested but should work fine
		float h = hue / 360.0f;
		float s = saturation / 100.0f;
		float l = lightness / 100.0f;

		if (s < 0.0f) s = 0.0f;
		if (s > 1.0f) s = 1.0f;
		if (l < 0.0f) l = 0.0f;
		if (l > 1.0f) l = 1.0f;

		float r, g, b;
		if (s == 0.0f) {
			r = g = b = l; // achromatic
		}
		else {
			auto hue2rgb = [](float p, float q, float t) -> float {
				if (t < 0.0f) t += 1.0f;
				if (t > 1.0f) t -= 1.0f;
				if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
				if (t < 1.0f / 2.0f) return q;
				if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
				return p;
			};
			float q = (l < 0.5f) ? (l * (1.0f + s)) : (l + s - l * s);
			float p = 2.0f * l - q;
			r = hue2rgb(p, q, h + 1.0f / 3.0f);
			g = hue2rgb(p, q, h);
			b = hue2rgb(p, q, h - 1.0f / 3.0f);
		}

		return CL_Color_Create(
			static_cast<int>(r * 255.0f),
			static_cast<int>(g * 255.0f),
			static_cast<int>(b * 255.0f),
			alpha
		);
	}


	void fill(int red, int green, int blue, int alpha) {
		Settings::gFillColor = CL_Color_Create(red, green, blue, alpha);
		AEGfxSetColorToMultiply(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
	}
	void fill(CL_Color color) {
		fill(color.red, color.green, color.blue, color.alpha);
	}
	void textFill(int red, int green, int blue, int alpha) {
		Settings::textFillColor = CL_Color_Create(red, green, blue, alpha);
		AEGfxSetColorToMultiply(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
	}


	void stroke(int red, int green, int blue, int alpha) {
		Settings::gStrokeColor = CL_Color_Create(red, green, blue, alpha);
	}
	void stroke(CL_Color color) {
		stroke(color.red, color.green, color.blue, color.alpha);
	}

	void noFill(void) {
		fill(0, 0, 0, 0);
	}
	void noStroke(void) {
		stroke(0, 0, 0, 0);
	}

	void background(int red, int green, int blue) {
		AEGfxSetBackgroundColor(red / 255.0f, green / 255.0f, blue / 255.0f);
	}
	void background(CL_Color color) {
		background(color.red, color.green, color.blue);
	}
}

namespace Shapes {
	static AEGfxVertexList* sRectMesh = nullptr;
	static AEGfxVertexList* sRectCornerMesh = nullptr;
	static AEGfxVertexList* sEllipseMesh = nullptr;
	static AEGfxVertexList* sEllipseCornerMesh = nullptr;
	static AEGfxVertexList* sTriangleMesh = nullptr;

	bool init(void) {
		if (sRectMesh && sRectCornerMesh && sEllipseMesh && sEllipseCornerMesh && sTriangleMesh) return true;

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

		if (!sRectCornerMesh) {
			AEGfxMeshStart();

			AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
				1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f,
				0.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f);
			AEGfxTriAdd(1.0f, -1.0f, 0xFFFFFFFF, 1.0f, 1.0f,
				1.0f, 0.0f, 0xFFFFFFFF, 1.0f, 0.0f,
				0.0f, -1.0f, 0xFFFFFFFF, 0.0f, 1.0f);

			sRectCornerMesh = AEGfxMeshEnd();
		}

		if (!sEllipseMesh) {
			const int NUM_SEGMENTS = 32;
			const float RADIUS = 0.5f;

			AEGfxMeshStart();

			for (int i = 0; i < NUM_SEGMENTS; i++) {
				float angle0 = (2.0f * PI * i) / NUM_SEGMENTS;
				float angle1 = (2.0f * PI * (i + 1)) / NUM_SEGMENTS;

				float x0 = RADIUS * cosf(angle0);
				float y0 = RADIUS * sinf(angle0);
				float x1 = RADIUS * cosf(angle1);
				float y1 = RADIUS * sinf(angle1);

				// with texture mapping
				AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.5f, 0.5f,
					x0, y0, 0xFFFFFFFF, (x0 / RADIUS + 1.0f) * 0.5f, (y0 / RADIUS + 1.0f) * 0.5f,
					x1, y1, 0xFFFFFFFF, (x1 / RADIUS + 1.0f) * 0.5f, (y1 / RADIUS + 1.0f) * 0.5f);
			}

			sEllipseMesh = AEGfxMeshEnd();
		}

		if (!sEllipseCornerMesh) {
			const int NUM_SEGMENTS = 32;
			const float RADIUS = 0.5f;

			AEGfxMeshStart();

			for (int i = 0; i < NUM_SEGMENTS; i++) {
				float angle0 = (2.0f * PI * i) / NUM_SEGMENTS;
				float angle1 = (2.0f * PI * (i + 1)) / NUM_SEGMENTS;

				float x0 = RADIUS * cosf(angle0);
				float y0 = RADIUS * sinf(angle0);
				float x1 = RADIUS * cosf(angle1);
				float y1 = RADIUS * sinf(angle1);

				AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
					x0, y0, 0xFFFFFFFF, (x0 / RADIUS + 1.0f) * 0.5f, (y0 / RADIUS + 1.0f) * 0.5f,
					x1, y1, 0xFFFFFFFF, (x1 / RADIUS + 1.0f) * 0.5f, (y1 / RADIUS + 1.0f) * 0.5f);
			}

			sEllipseCornerMesh = AEGfxMeshEnd();
		}

		if (!sTriangleMesh) {
			AEGfxMeshStart();

			AEGfxVertexAdd(0.0f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
			AEGfxVertexAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
			AEGfxVertexAdd(0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
			sTriangleMesh = AEGfxMeshEnd();
		}

		return (sRectMesh != nullptr) && (sEllipseMesh != nullptr) && (sTriangleMesh != nullptr);
	}

	void exit(void) {
		if (sRectMesh) {
			AEGfxMeshFree(sRectMesh);
			sRectMesh = nullptr;
		}

		if (sRectCornerMesh) {
			AEGfxMeshFree(sRectCornerMesh);
			sRectCornerMesh = nullptr;
		}

		if (sEllipseMesh) {
			AEGfxMeshFree(sEllipseMesh);
			sEllipseMesh = nullptr;
		}

		if (sEllipseCornerMesh) {
			AEGfxMeshFree(sEllipseCornerMesh);
			sRectCornerMesh = nullptr;
		}

		if (sTriangleMesh) {
			AEGfxMeshFree(sTriangleMesh);
			sTriangleMesh = nullptr;
		}
	}

	void rectDraw(float x, float y, float width, float height, SHAPE_MODE mode) {
		if (!sRectMesh || !sRectCornerMesh) return;

		AEMtx33 scale = { 0 };
		AEMtx33 rotation = { 0 };
		AEMtx33 translation = { 0 };
		AEMtx33 transformation = { 0 };


		AEMtx33Scale(&scale, width, height);
		AEMtx33Rot(&rotation, 0.0f);
		AEMtx33Trans(&translation, x, y);

		AEMtx33Concat(&transformation, &rotation, &scale);
		AEMtx33Concat(&transformation, &translation, &transformation);

		AEVec2 center = { 0 };
		AEVec2Set(&center, 0.0f, 0.0f);

		AEGfxSetTransform(transformation.m);

		if (mode == CENTER) { AEGfxMeshDraw(sRectMesh, AE_GFX_MDM_TRIANGLES); }
		else if(mode == CORNER) { AEGfxMeshDraw(sRectCornerMesh, AE_GFX_MDM_TRIANGLES); }
		else { AEGfxMeshDraw(sRectCornerMesh, AE_GFX_MDM_TRIANGLES); }
	}
	void rect(float x, float y, float width, float height, SHAPE_MODE mode) {
		bool drawStroke = (Settings::gStrokeColor.alpha > 0 && Settings::gStrokeWeight > 0.0f);

		if (drawStroke) {
			float halfStroke = Settings::gStrokeWeight / 2.0f;
			float strokeWidth = width + Settings::gStrokeWeight;
			float strokeHeight = height + Settings::gStrokeWeight;

			AEGfxSetColorToMultiply(Settings::gStrokeColor.red / 255.0f,
									Settings::gStrokeColor.green / 255.0f,
									Settings::gStrokeColor.blue / 255.0f,
									Settings::gStrokeColor.alpha / 255.0f);
			if (mode == CENTER) rectDraw(x, y, strokeWidth, strokeHeight, CENTER);
			else rectDraw(x - halfStroke, y + halfStroke, strokeWidth, strokeHeight, CORNER);
		}
		
		AEGfxSetColorToMultiply(Settings::gFillColor.red / 255.0f,
								Settings::gFillColor.green / 255.0f,
								Settings::gFillColor.blue / 255.0f,
								Settings::gFillColor.alpha / 255.0f);
		if (mode == CENTER) rectDraw(x, y, width, height, CENTER);
		else rectDraw(x, y, width, height, CORNER);
	}
	void rect(Quad quad, SHAPE_MODE mode) {
		rect(quad.position.x, quad.position.y, quad.width, quad.height, mode);
	}


	void ellipseDraw(float x, float y, float width, float height, SHAPE_MODE mode) {
		if (!sEllipseMesh || !sEllipseCornerMesh) return;

		AEMtx33 scale = { 0 };
		AEMtx33 rotation = { 0 };
		AEMtx33 translation = { 0 };
		AEMtx33 transformation = { 0 };

		AEMtx33Scale(&scale, width, height);
		AEMtx33Rot(&rotation, 0.0f);
		AEMtx33Trans(&translation, x, y);

		AEMtx33Concat(&transformation, &rotation, &scale);
		AEMtx33Concat(&transformation, &translation, &transformation);

		AEVec2 center = { 0 };
		AEVec2Set(&center, 0.0f, 0.0f);

		AEGfxSetTransform(transformation.m);

		if (mode == CENTER) AEGfxMeshDraw(sEllipseMesh, AE_GFX_MDM_TRIANGLES);
		else AEGfxMeshDraw(sEllipseCornerMesh, AE_GFX_MDM_TRIANGLES);
	}
	void ellipse(float x, float y, float width, float height, SHAPE_MODE mode) {
		bool drawStroke = (Settings::gStrokeColor.alpha > 0 && Settings::gStrokeWeight > 0.0f);
		if (drawStroke) {
			float halfStroke = Settings::gStrokeWeight / 2.0f;
			float strokeWidth = width + Settings::gStrokeWeight * 2;
			float strokeHeight = height + Settings::gStrokeWeight * 2;
			AEGfxSetColorToMultiply(Settings::gStrokeColor.red / 255.0f,
									Settings::gStrokeColor.green / 255.0f,
									Settings::gStrokeColor.blue / 255.0f,
									Settings::gStrokeColor.alpha / 255.0f);
			if (mode == CENTER) ellipseDraw(x, y, strokeWidth, strokeHeight, CENTER);
			else ellipseDraw(x - halfStroke, y + halfStroke, strokeWidth, strokeHeight, CORNER);
		}
		AEGfxSetColorToMultiply(Settings::gFillColor.red / 255.0f,
								Settings::gFillColor.green / 255.0f,
								Settings::gFillColor.blue / 255.0f,
								Settings::gFillColor.alpha / 255.0f);
		if (mode == CENTER) ellipseDraw(x, y, width, height, CENTER);
		else ellipseDraw(x, y, width, height, CORNER);
	}
	void ellipse(Circle circle, SHAPE_MODE mode) {
		ellipse(circle.position.x, circle.position.y, circle.radius * 2.0f, circle.radius * 2.0f, mode);
	}


	void square(float x, float y, float size, SHAPE_MODE mode) {
		rect(x, y, size, size, mode);
	}
	void circle(float x, float y, float size, SHAPE_MODE mode) {
		ellipse(x, y, size, size, mode);
	}

	void triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
		// TODO: Bruh i still cant figure this out
	}
}

namespace Graphics {
	void image(float x, float y, float width, float height, AEGfxTexture* texture, Shapes::SHAPE_MODE mode) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		Color::fill(255, 255, 255);
		Color::noStroke();
		AEGfxTextureSet(texture, 0, 0);
		Shapes::rect(x, y, width, height, mode);
	}
	void image(Shapes::Quad quad, AEGfxTexture* texture, Shapes::SHAPE_MODE mode) {
		image(quad.position.x, quad.position.y, quad.width, quad.height, texture, mode);
	}
}

namespace Text {
	
	// This one is pretty much good to go
	void setFont(char const* fontPath, int fontSize) {
		if (Settings::pCurrentFont) {
			AEGfxDestroyFont(Settings::pCurrentFont);
			Settings::pCurrentFont = 0;
		}
		Settings::pCurrentFont = AEGfxCreateFont(fontPath, fontSize);
	}

	// REEEEEEEEEEEEE
	void text(char const* pText, float x, float y, TEXT_ALIGN align) {
		if (!Settings::pCurrentFont) return;
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		

		Color::CL_Color textFillColor = Settings::textFillColor; // change for now

		// convert x and y to normalized device coordinates
		float wx = x / (AEGfxGetWindowWidth() / 2.0f);
		float wy = y / (AEGfxGetWindowHeight() / 2.0f);

		float padW{};
		float padH{};
		
		AEGfxGetPrintSize(Settings::pCurrentFont, pText, 1.0f, &padW, &padH);

		if (align == ALIGN_CENTER) {
			AEGfxPrint(Settings::pCurrentFont, pText, wx -= (padW * 0.5f), wy, 1.0f,
				(float)textFillColor.red, (float)textFillColor.green, (float)textFillColor.blue, (float)textFillColor.alpha);
		}

		// thanks for debug text eee 
		//AEGfxPrint(Settings::pCurrentFont, "10", 0.5, 0, 1.0f,
		//	textFillColor.red, textFillColor.green, textFillColor.blue, textFillColor.alpha);
		//AEGfxPrint(Settings::pCurrentFont, "01", 0, 0.5, 1.0f,
		//	textFillColor.red, textFillColor.green, textFillColor.blue, textFillColor.alpha);
	}


	// This is also good to go
	void unloadFont(void) {
		if (Settings::pCurrentFont) {
			AEGfxDestroyFont(Settings::pCurrentFont);
			Settings::pCurrentFont = 0;
		}
	}
}
