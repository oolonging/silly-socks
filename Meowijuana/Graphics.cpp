
#include "Graphics.hpp"

namespace Color {
	CL_Color CL_Color_Create(int red, int green, int blue, int alpha = 255) {
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
	CL_Color CL_Color_Create_HSL(int hue, int saturation, int lightness, int alpha = 255) {
		CL_Color color;

		// TODO: implement HSL to RGB conversion
		color.red = 255;
		color.green = 255;
		color.blue = 255;
		color.alpha = alpha;

		return color;

	}


	void fill(int red, int green, int blue, int alpha) {
		AEGfxSetColorToMultiply(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
	}


	void stroke(int red, int green, int blue, int alpha) {}
	
	void noFill(void) {
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	void noStroke(void) {
		// TODO: implement
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

			AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
						0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
						-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
			AEGfxTriAdd(0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
						0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
						-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

			sRectMesh = AEGfxMeshEnd();
		}

		if (!sRectCornerMesh) {
			AEGfxMeshStart();

			AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
				1.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
				0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);
			AEGfxTriAdd(1.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f,
				1.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
				0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f);

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

				AEGfxTriAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f,
							x0, y0, 0xFFFFFFFF, 0.0f, 0.0f,
							x1, y1, 0xFFFFFFFF, 0.0f, 0.0f);
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
					x0, y0, 0xFFFFFFFF, 0.0f, 0.0f,
					x1, y1, 0xFFFFFFFF, 0.0f, 0.0f);
			}

			sEllipseCornerMesh = AEGfxMeshEnd();
		}

		if(!sTriangleMesh) {
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

		if (sRectCornerMesh) {
			AEGfxMeshFree(sRectCornerMesh);
			sRectCornerMesh = nullptr;
		}

		if (sTriangleMesh) {
			AEGfxMeshFree(sTriangleMesh);
			sTriangleMesh = nullptr;
		}
	}

	void rect(float x, float y, float width, float height, SHAPE_MODE mode) {
		if (!sRectMesh) return;

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

		if (mode == CENTER) {
			AEGfxMeshDraw(sRectMesh, AE_GFX_MDM_TRIANGLES);
		}
		else {
			AEGfxMeshDraw(sRectCornerMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	void ellipse(float x, float y, float width, float height, SHAPE_MODE mode) {
		if (!sEllipseMesh) return;

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

		if (mode == CENTER) {
			AEGfxMeshDraw(sEllipseMesh, AE_GFX_MDM_TRIANGLES);
		}
		else {
			AEGfxMeshDraw(sEllipseCornerMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	void square(float x, float y, float size, SHAPE_MODE mode) {
		rect(x, y, size, size, mode);
	}

	void circle(float x, float y, float size, SHAPE_MODE mode) {
		ellipse(x, y, size, size, mode);
	}

	void triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
		/*if (!sTriangleMesh) return;

		AEMtx33 translation = { 0 };
		AEMtx33 transformation = { 0 };

		AEMtx33Trans(&translation, x1, y1);
		AEMtx33Concat(&transformation, &translation, &translation);
		AEGfxSetTransform(transformation.m);
		AEGfxMeshDraw(sTriangleMesh, AE_GFX_MDM_TRIANGLES);*/

		// nvm ill redo this another time
	}
}
