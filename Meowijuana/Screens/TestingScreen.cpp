#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "../UI_Elements.hpp"
#include "TestingScreen.hpp"

// import other testing pages here
#include "Testing/testing-andrea.hpp"
#include "Testing/testing-jun.hpp"
#include "Testing/testing-saahil.hpp"
#include "Testing/testing-xuan.hpp"

// temporary routing to individual testing pages
void rerouteTesting(void) {
	
	// Routes to saahils testing page
	if (AEInputCheckTriggered(AEVK_NUMPAD1)) {
		fpLoad = Saahil_Load;
		fpInitialize = Saahil_Initialize;
		fpUpdate = Saahil_Update;
		fpDraw = Saahil_Draw;
		fpFree = Saahil_Free;
		fpUnload = Saahil_Unload;
	}

	// Routes to xuans testing page
	if(AEInputCheckTriggered(AEVK_NUMPAD2)) {
		fpLoad = Xuan_Load;
		fpInitialize = Xuan_Initialize;
		fpUpdate = Xuan_Update;
		fpDraw = Xuan_Draw;
		fpFree = Xuan_Free;
		fpUnload = Xuan_Unload;
	}

	// Routes to andreas testing page
	if(AEInputCheckTriggered(AEVK_NUMPAD3)) {
		fpLoad = Andrea_Load;
		fpInitialize = Andrea_Initialize;
		fpUpdate = Andrea_Update;
		fpDraw = Andrea_Draw;
		fpFree = Andrea_Free;
		fpUnload = Andrea_Unload;
	}

	// Routes to juns testing page
	if(AEInputCheckTriggered(AEVK_NUMPAD4)) {
		fpLoad = Jun_Load;
		fpInitialize = Jun_Initialize;
		fpUpdate = Jun_Update;
		fpDraw = Jun_Draw;
		fpFree = Jun_Free;
		fpUnload = Jun_Unload;
	}
}

// Lets say this class isnt in the file and is being imported
class Fella {
	private:
		float x;
		float y;

		float size = 50.0f;
		float speed = 2.0f;

		AEGfxTexture* image;

	public:
		// getters
		float getX() { return this->x; }
		float getY() { return this->y; }

		// Setters
		void setImage(AEGfxTexture* image) {
			this->image = image;
		}

		void setPosition(float x, float y) {
			this->x = x;
			this->y = y;
		}

		void handleMovement(void) {
			if (AEInputCheckCurr(AEVK_W))
				this->y += this->speed;

			if (AEInputCheckCurr(AEVK_S))
				this->y -= this->speed;

			if (AEInputCheckCurr(AEVK_A))
				this->x -= this->speed;

			if (AEInputCheckCurr(AEVK_D))
				this->x += this->speed;
		}

		void draw(void) {
			if(this->image)
			Graphics::image(this->x, this->y, this->size, this->size, this->image);
			else {
				// draw a blue rect instead
				Color::fill(0, 0, 255);
				Shapes::rect(this->x, this->y, this->size, this->size);
			}
		}

		// Ctors
		Fella(float x, float y, AEGfxTexture* image) : x(x), y(y), image(image) {}
		Fella() : x(0.0f), y(0.0f), image(nullptr) {}

};

// Fella
Fella fella{};

// Image
AEGfxTexture* image;

void Testing_Load() {
	image = AEGfxTextureLoad("Assets/mycat.png");
}

void Testing_Initialize() {
	fella.setImage(image);
	fella.setPosition(0.0f, 0.0f);
}

void Testing_Update() {
	fella.handleMovement();

	rerouteTesting();
}

void Testing_Draw() {
	// reset background to black
	Color::background({ 0, 0, 0, 255 });

	// set the render mode to color so shapes in front can render
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	// testing text, just thought of something
	Color::textFill(255, 255, 255);
	Text::text("Testing screen", 0.0f, 0.0f);

	// Draw fella
	fella.draw();
}

void Testing_Free() {
	fella.setImage(nullptr);
}

void Testing_Unload() {
	AEGfxTextureUnload(image);
	image = nullptr;
}
